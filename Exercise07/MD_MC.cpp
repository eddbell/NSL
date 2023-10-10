#include <iostream>
#include <fstream>
#include <ostream>
#include <cmath>
#include <iomanip>
#include "MD_MC.h"
#include "../Libs/random/random.h"

using namespace std;

int main()
{
  Input(); //Inizialization
  if (eqtemp != 0) for(int i= 0;i<eqtemp;i++) Move();
  //int nconf = 1;
  for(int iblk=1; iblk <= nblk; iblk++) //Simulation
  {
    Reset(iblk);   //Reset block averages
      for(int istep=1; istep <= nstep; istep++)
      {
        Move();
        Measure();
        Accumulate(); //Update block averages
      }
    Averages(iblk);
  }
  ConfFinal(); //Write final configuration

  return 0;
}


void Input(void)
{
  ifstream ReadInput, ReadConf, ReadVelocity, Primes, Seed;

//Read seed for random numbers
  int p1, p2;
  Primes.open("./Config/Primes");
  Primes >> p1 >> p2 ;
  Primes.close();

//Read input informations
  ReadInput.open("./Config/input.in");

  ReadInput >> iNVET;
  ReadInput >> restart;

  if(restart) Seed.open("./Config/seed.out");
  else Seed.open("./Config/seed.in");
  Seed >> seed[0] >> seed[1] >> seed[2] >> seed[3];
  rnd.SetRandom(seed,p1,p2);
  Seed.close();

  ReadInput >> temp;
  beta = 1.0/temp;
  cout << "Temperature = " << temp << endl;

  ReadInput >> npart;
  //cout << "Number of particles = " << npart << endl;

  ReadInput >> rho;
  //cout << "Density of particles = " << rho << endl;
  vol = (double)npart/rho;
  box = pow(vol,1.0/3.0);
  //cout << "Volume of the simulation box = " << vol << endl;
  //cout << "Edge of the simulation box = " << box << endl;

  ReadInput >> rcut;
  cout << "Cutoff of the interatomic potential = " << rcut << endl << endl;
  vtail = 8*pi*rho/3*(1/3*pow(rcut,-9)-pow(rcut,-3));
  ptail = 32*pi*rho*(1/9*pow(rcut,-9)-1/6*pow(rcut,-3));

  ReadInput >> delta;
  ReadInput >> nblk;

  ReadInput >> nstep;

  ReadInput >> eqtemp;
  if(eqtemp!=0) cout<<"Set equilibration start after :  "<< eqtemp<< " steps."<<endl;

  ReadInput.close();

//Prepare arrays for measurements
  iv = 0; //Potential energy
  it = 1; //Temperature
  ik = 2; //Kinetic energy
  ie = 3; //Total energy
  ip = 4; //presion
  n_props = 5; //Number of observables

//Read initial configuration
  //cout << "Read initial configuration" << endl << endl;
  if(restart)
  {
    ReadConf.open("./Config/config.out");
    ReadVelocity.open("./Config/velocity.out");
    for (int i=0; i<npart; ++i) ReadVelocity >> vx[i] >> vy[i] >> vz[i];
  }
  else
  {
    ReadConf.open("./Config/config.in");
    //cout << "Prepare velocities with center of mass velocity equal to zero " << endl;
    double sumv[3] = {0.0, 0.0, 0.0};
    for (int i=0; i<npart; ++i)
    {
      vx[i] = rnd.Gauss(0.,sqrt(temp));
      vy[i] = rnd.Gauss(0.,sqrt(temp));
      vz[i] = rnd.Gauss(0.,sqrt(temp));
      sumv[0] += vx[i];   //calcolo le velocià di drift
      sumv[1] += vy[i];
      sumv[2] += vz[i];
    }
    for (int idim=0; idim<3; ++idim) sumv[idim] /= (double)npart;
    double sumv2 = 0.0, fs;
    for (int i=0; i<npart; ++i)
    {
      vx[i] = vx[i] - sumv[0];  //sottraggo il drift a tutte le velocità
      vy[i] = vy[i] - sumv[1];
      vz[i] = vz[i] - sumv[2];
      sumv2 += vx[i]*vx[i] + vy[i]*vy[i] + vz[i]*vz[i];
    }
    sumv2 /= (double)npart;
    fs = sqrt(3 * temp / sumv2);   // fs = velocity scale factor
    //cout << "velocity scale factor: " << fs << endl << endl;
    for (int i=0; i<npart; ++i)
    {
      vx[i] *= fs;
      vy[i] *= fs;
      vz[i] *= fs;
    }
  }

  for (int i=0; i<npart; ++i)
  {
    ReadConf >> x[i] >> y[i] >> z[i];
    x[i] = Pbc( x[i] * box ); //trasformo le cordinate da unità di misura del lato a unità di misura naturali
    y[i] = Pbc( y[i] * box );
    z[i] = Pbc( z[i] * box );
  }
  ReadConf.close();

  for (int i=0; i<npart; ++i)
  {
    if(iNVET)//è 1 se vale MC
    {
      xold[i] = x[i];
      yold[i] = y[i];
      zold[i] = z[i];
    }
    else
    {//periodi boundary condition delta = time step. Mi ricreo le traiettorie precedenti
      xold[i] = Pbc(x[i] - vx[i] * delta);
      yold[i] = Pbc(y[i] - vy[i] * delta);
      zold[i] = Pbc(z[i] - vz[i] * delta);
    }
  }

//Evaluate properties of the initial configuration
  Measure();

//Print initial values for measured properties
/*
  cout << "Initial potential energy = " << walker[iv]/(double)npart << endl;
  cout << "Initial temperature      = " << walker[it] << endl;
  cout << "Initial kinetic energy   = " << walker[ik]/(double)npart << endl;
  cout << "Initial total energy     = " << walker[ie]/(double)npart << endl;
*/
  return;
}


void Move()
{
  int o;
  double p, energy_old, energy_new;
  double xnew, ynew, znew;

  if(iNVET) // Monte Carlo (NVT) move
  {
    for(int i=0; i<npart; ++i)
    {
    //Select randomly a particle (for C++ syntax, 0 <= o <= npart-1)
      o = (int)(rnd.Rannyu()*npart);

    //Old
      energy_old = Boltzmann(x[o],y[o],z[o],o);

    //New
      x[o] = Pbc( x[o] + delta*(rnd.Rannyu() - 0.5) );
      y[o] = Pbc( y[o] + delta*(rnd.Rannyu() - 0.5) );
      z[o] = Pbc( z[o] + delta*(rnd.Rannyu() - 0.5) );

      energy_new = Boltzmann(x[o],y[o],z[o],o);

    //Metropolis test
      p = exp(beta*(energy_old-energy_new));
      if(p >= rnd.Rannyu())
      {
      //Update
        xold[o] = x[o];
        yold[o] = y[o];
        zold[o] = z[o];
        accepted = accepted + 1.0;
      } else {
        x[o] = xold[o];
        y[o] = yold[o];
        z[o] = zold[o];
      }
      attempted = attempted + 1.0;
    }
  } else // Molecular Dynamics (NVE) move
  {
    double fx[m_part], fy[m_part], fz[m_part];

    for(int i=0; i<npart; ++i){ //Force acting on particle i
      fx[i] = Force(i,0);
      fy[i] = Force(i,1);
      fz[i] = Force(i,2);
    }

    for(int i=0; i<npart; ++i){ //Verlet integration scheme

      xnew = Pbc( 2.0 * x[i] - xold[i] + fx[i] * pow(delta,2) );
      ynew = Pbc( 2.0 * y[i] - yold[i] + fy[i] * pow(delta,2) );
      znew = Pbc( 2.0 * z[i] - zold[i] + fz[i] * pow(delta,2) );

      vx[i] = Pbc(xnew - xold[i])/(2.0 * delta);
      vy[i] = Pbc(ynew - yold[i])/(2.0 * delta);
      vz[i] = Pbc(znew - zold[i])/(2.0 * delta);

      xold[i] = x[i];
      yold[i] = y[i];
      zold[i] = z[i];

      x[i] = xnew;
      y[i] = ynew;
      z[i] = znew;

      accepted = accepted + 1.0;
      attempted = attempted + 1.0;
    }
  }
  return;
}

double Boltzmann(double xx, double yy, double zz, int ip)
{
  double ene=0.0;
  double dx, dy, dz, dr;

  for (int i=0; i<npart; ++i)
  {
    if(i != ip)
    {
// distance ip-i in pbc
      dx = Pbc(xx - x[i]);
      dy = Pbc(yy - y[i]);
      dz = Pbc(zz - z[i]);

      dr = dx*dx + dy*dy + dz*dz;
      dr = sqrt(dr);

      if(dr < rcut)
      {
        ene += 1.0/pow(dr,12) - 1.0/pow(dr,6);
      }
    }
  }

  return 4.0*ene;
}

double Force(int ip, int idir){ //Compute forces as -Grad_ip V(r)
  double f=0.0;
  double dvec[3], dr;

  for (int i=0; i<npart; ++i){
    if(i != ip){
      dvec[0] = Pbc( x[ip] - x[i] );  // distance ip-i in pbc
      dvec[1] = Pbc( y[ip] - y[i] );
      dvec[2] = Pbc( z[ip] - z[i] );

      dr = dvec[0]*dvec[0] + dvec[1]*dvec[1] + dvec[2]*dvec[2];
      dr = sqrt(dr);

      if(dr < rcut){
        f += dvec[idir] * (48.0/pow(dr,14) - 24.0/pow(dr,8)); // -Grad_ip V(r)
      }
    }
  }

  return f;
}

void Measure() //Properties measurement
{
  double v = 0.0, w = 0.0, kin=0.0;
  double vij, wij;
  double dx, dy, dz;

//cycle over pairs of particles
  for (int i=0; i<npart-1; ++i)
  {
    for (int j=i+1; j<npart; ++j)
    {
// distance i-j in pbc
      dx = Pbc(x[i] - x[j]);
      dy = Pbc(y[i] - y[j]);
      dz = Pbc(z[i] - z[j]);

      dr = dx*dx + dy*dy + dz*dz;
      dr = sqrt(dr);

      if(dr < box/2){
        int bin = nbins*(dr/(0.5*box));
        blk_g[bin] +=2;
      }
      if(dr < rcut)
      {
        vij = 1.0/pow(dr,12) - 1.0/pow(dr,6);
        v += vij;

        wij = 1.0/pow(dr,12) - 0.5/pow(dr,6);
        w += wij;
      }
    }
  }

  for (int i=0; i<npart; ++i) kin += 0.5 * (vx[i]*vx[i] + vy[i]*vy[i] + vz[i]*vz[i]);

  walker[iv] = 4.0 * v; // Potential energy
  walker[ik] = kin; // Kinetic energy
  walker[it] = (2.0 / 3.0) * kin/(double)npart; // Temperature
  walker[ie] = 4.0 * v + kin;  // Total energy;
  walker[ip] = rho*walker[it] + 16.0/vol * w; // pression

  return;
}


void Reset(int iblk) //Reset block averages
{

   if(iblk == 1)
   {
       for(int i=0; i<n_props; ++i)
       {
           glob_av[i] = 0;
           glob_av2[i] = 0;
       }

       for(int i=0; i<100; i++){ //inizializzo a 0 gli accumulatori per G(r)
          blk_g[i]=0;
          glob_g[i]=0;
          glob_av_g[i]=0;
          glob_av2_g[i]=0;
       }
   }


   for(int i=0; i<n_props; ++i)
   {
     blk_av[i] = 0;
   }
   blk_norm = 0;
   attempted = 0;
   accepted = 0;
}


void Accumulate(void) //Update block averages
{

   for(int i=0; i<n_props; ++i)
   {
     blk_av[i] = blk_av[i] + walker[i];
   }
   blk_norm = blk_norm + 1.0;

   for (int i=0; i<nbins; i++){
      glob_g[i]=glob_g[i]+blk_g[i];
      blk_g[i]=0;
   }
}


void Averages(int iblk) //Print results for current block
{
  ofstream Epot, Pres;
  const int wd=12;

  if(iblk%(nblk/10)==0) cout<<iblk<< " of "<<nblk<<" blocks"<<setw(12)<<"rate : "<<accepted/attempted<<endl;
  Epot.open("output_epot.dat",ios::app);
  Pres.open("output_pres.dat",ios::app);

  stima_pot = blk_av[iv]/blk_norm/(double)npart; //Potential energy
  glob_av[iv] += stima_pot;
  glob_av2[iv] += stima_pot*stima_pot;
  err_pot=Error(glob_av[iv],glob_av2[iv],iblk);

  stima_pres = blk_av[ip]/blk_norm/(double)npart; //Pressure
  glob_av[ip] += stima_pres;
  glob_av2[ip] += stima_pres*stima_pres;
  err_pres=Error(glob_av[ip],glob_av2[ip],iblk);

//Potential energy per particle
  Epot << setw(wd) << iblk <<  setw(wd) << stima_pot << setw(wd) << glob_av[iv]/(double)iblk << setw(wd) << err_pot << endl;
//pression
  Pres << setw(wd) << iblk <<  setw(wd) << stima_pres << setw(wd) << glob_av[ip]/(double)iblk << setw(wd) << err_pres << endl;

  Epot.close();
  Pres.close();

  ofstream GR, GR_ERR;

  GR.open("output_g.dat",ios::app);
  GR_ERR.open("output_g_err.dat",ios::app);
  GR<<iblk;
  for (int i=0; i<nbins; i++){
    norm_g=rho*npart*4/3*M_PI*(pow(i*box/200 + box/200,3)-pow(i*box/200 ,3));
    stima_g = glob_g[i]/blk_norm/norm_g; //G(r)
    glob_av_g[i] += stima_g;
    glob_av2_g[i] += stima_g*stima_g;
    err_g=Error(glob_av_g[i],glob_av2_g[i],iblk);

    GR<< stima_g <<setw(wd);
    GR_ERR<< err_g <<setw(wd);
  }
  GR<<endl; GR_ERR<<endl;
  GR.close();
  GR_ERR.close();
}

void PrintEq(int iblk){
  ofstream Epot;
  const int wd=12;
  Epot.open("output_epot.dat",ios::app);

  if(iblk%(nblk/10)==0) cout<<iblk<< " of "<<nblk<<" blocks"<<setw(12)<<"rate : "<<accepted/attempted<<endl;

  stima_pot = blk_av[iv]/blk_norm/(double)npart+vtail; //Potential energy
  glob_av[iv] += stima_pot;
  glob_av2[iv] += stima_pot*stima_pot;
  err_pot=Error(glob_av[iv],glob_av2[iv],iblk);
  Epot << setw(wd) << iblk <<  setw(wd) << stima_pot << setw(wd) << err_pot << endl;
  Epot.close();
}

void ConfFinal(void)
{
  ofstream WriteConf, WriteVelocity, WriteSeed;

  //cout << "Print final configuration to file config.out" << endl << endl;
  WriteConf.open("./Config/config.out");
  WriteVelocity.open("./Config/velocity.out");
  for (int i=0; i<npart; ++i)
  {
    WriteConf << x[i]/box << "   " <<  y[i]/box << "   " << z[i]/box << endl;
    WriteVelocity << vx[i] << "   " <<  vy[i] << "   " << vz[i] << endl;
  }
  WriteConf.close();
  WriteVelocity.close();

  rnd.SaveSeed();
}

void ConfXYZ(int nconf){ //Write configuration in .xyz format
  ofstream WriteXYZ;

  WriteXYZ.open("frames/config_" + to_string(nconf) + ".xyz");
  WriteXYZ << npart << endl;
  WriteXYZ << "This is only a comment!" << endl;
  for (int i=0; i<npart; ++i){
    WriteXYZ << "LJ  " << Pbc(x[i]) << "   " <<  Pbc(y[i]) << "   " << Pbc(z[i]) << endl;
  }
  WriteXYZ.close();
}

double Pbc(double r)  //Algorithm for periodic boundary conditions with side L=box
{
    return r - box * rint(r/box);
}

double Error(double sum, double sum2, int iblk)
{
    return sqrt(fabs(sum2/(double)iblk - pow(sum/(double)iblk,2))/(double)iblk);
}

/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/