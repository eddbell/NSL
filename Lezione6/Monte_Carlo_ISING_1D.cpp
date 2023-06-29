/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <ostream>
#include <cmath>
#include <iomanip>
#include "Monte_Carlo_ISING_1D.h"

using namespace std;

int main()
{
  Input(); //Inizialization
  //Equilibration(nblk_eq);
  for(int iblk=1; iblk <= nblk; ++iblk) //Simulation
  {
    Reset(iblk);   //Reset block averages
    for(int istep=1; istep <= nstep; ++istep)
    {
      Move(metro);
      Measure();
      Accumulate(); //Update block averages
    }
    Averages(iblk);   //Print results for current block
    if (equilibration == 1) PrintEq(iblk);
  }
  cout << "Temperature = " << temp << "      Accuracy = "<<accepted/attempted <<endl;
  ConfFinal(); //Write final configuration
  if (equilibration == 0) Print();  //Write final averages and errors
  return 0;
}

void Input(void)
{
  ifstream ReadInput;
  ifstream ReadConf;
  ifstream Seed;
  /*
  cout << "Classic 1D Ising model             " << endl;
  cout << "Monte Carlo simulation             " << endl << endl;
  cout << "Nearest neighbour interaction      " << endl << endl;
  cout << "Boltzmann weight exp(- beta * H ), beta = 1/T " << endl << endl;
  cout << "The program uses k_B=1 and mu_B=1 units " << endl;
  */
//Read seed for random numbers
   int p1, p2;
   ifstream Primes("Primes");
   Primes >> p1 >> p2 ;
   Primes.close();

//Read input informations
  ReadInput.open("input.dat");

  ReadInput >> temp;
  beta = 1.0/temp;

  ReadInput >> nspin;
  //cout << "Number of spins = " << nspin << endl;

  ReadInput >> J;
  //cout << "Exchange interaction = " << J << endl;

  ReadInput >> h;
  //cout << "External field = " << h << endl << endl;

  ReadInput >> metro; // if=1 Metropolis else Gibbs

  ReadInput >> restart;//if=1 initial configuration is random

  ReadInput >> equilibration;

  ReadInput >> nblk;

  ReadInput >> nstep;

  nblk_eq = 10000;
  /*
  if(metro==1) cout << "The program perform Metropolis moves" << endl;
  else cout << "The program perform Gibbs moves" << endl;
  cout << "Number of blocks = " << nblk << endl;
  cout << "Number of steps in one block = " << nstep << endl << endl;*/
  ReadInput.close();

  if(restart) Seed.open("seed.in");
  else Seed.open("seed.out");
  Seed >> seed[0] >> seed[1] >> seed[2] >> seed[3];
  rnd.SetRandom(seed,p1,p2);
  Seed.close();


//Prepare arrays for measurements
  iu = 0; //Energy
  ic = 1; //Heat capacity
  im = 2; //Magnetization
  ix = 3; //Magnetic susceptibility

  n_props = 4; //Number of observables

//initial configuration
  if(restart){
    for (int i=0; i<nspin; ++i){
      if(rnd.Rannyu() >= 0.5) s[i] = 1;
      else s[i] = -1;
    }
  }else{
    cout<<"Without restart."<<endl;
    ReadConf.open("config.final");
    for (int i=0; i<nspin; ++i) ReadConf >> s[i];
    ReadConf.close();
  }



//Evaluate energy etc. of the initial configuration
  Measure();

//Print initial values for the potential energy and virial
  /*
  cout << "Initial energy = " << walker[iu]/(double)nspin << endl;
  cout << "Initial Magnitization = " << walker[im]/(double)nspin << endl;
  cout << "Initial magnetic susceptibility = " << walker[ix]/(double)nspin << endl;
  cout << "Initial Heat capacity = " << walker[ic]/(double)nspin << endl;*/
}


void Move(int metro)
{
  int o;
  double p, energy_old, energy_new;
  //double energy_up, energy_down;

  for(int i=0; i<nspin; ++i)
  {
  //Select randomly a particle (for C++ syntax, 0 <= o <= nspin-1)
    o = (int)(rnd.Rannyu()*nspin);

    if(metro==1) //Metropolis
    {
      //cambio lo spin di una particella random
      energy_old = Boltzmann(s[o],o);
      //s[o]= -s[o];//if(rnd.Rannyu()>0.9)
      energy_new = Boltzmann(-s[o],o);
      //Metropolis test
      p = exp(beta*(energy_old-energy_new));
      if(p>1){
        s[o]=-s[o];
        accepted++;
      }
      else if(rnd.Rannyu()<p){
        s[o]=-s[o];
        accepted++;
      }
      attempted++;

    }
    else //Gibbs sampling
    {
      delta=-2*Boltzmann(1, o);
      if(rnd.Rannyu() < 1.0/(1.0+exp(-beta*delta))){
        s[o]=1;
        accepted++;
      }else{
        s[o]=-1;
      }
      attempted++;
    }
  }
}

double Boltzmann(int sm, int ip)
{
  double ene = -J * sm * ( s[Pbc(ip-1)] + s[Pbc(ip+1)] ) - h * sm;
  return ene;
}

void Measure()
{
  double u = 0.0, m = 0.0;

//cycle over spins
  for (int i=0; i<nspin; ++i)
  {
     u += -J * s[i] * s[Pbc(i+1)] - 0.5 * h * (s[i] + s[Pbc(i+1)]);
     m += s[i];
  }
  walker[iu] = u;
  walker[ic] = u*u;
  walker[im] = m;
  walker[ix] = m*m;
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
}


void Averages(int iblk) //Print results for current block
{

    //ofstream Ene, Heat, Mag, Chi;
    //const int wd=30;

    //cout << "Block number " << iblk << endl;
    //cout << "Acceptance rate " << accepted/attempted << endl << endl;
    //Ene.open("output_ene_h_prova.dat",ios::app);
    stima_u = blk_av[iu]/blk_norm/(double)nspin;
    glob_av[iu]  += stima_u;
    glob_av2[iu] += stima_u*stima_u;
    err_u=Error(glob_av[iu],glob_av2[iu],iblk);
    //Ene << setw(wd) << iblk <<  setw(wd) << stima_u << setw(wd) << glob_av[iu]/(double)iblk << setw(wd) << err_u << endl;
    //Ene.close();

    //Heat.open("output_heat_h.dat",ios::app);
    double mean_h2 = blk_av[ic]/blk_norm;
    stima_c = beta*beta*(mean_h2-(stima_u*stima_u)*(double)nspin*(double)nspin)/(double)nspin;
    glob_av[ic]  += stima_c;
    glob_av2[ic] += stima_c*stima_c;
    err_c=Error(glob_av[ic],glob_av2[ic],iblk);
    //Heat << setw(wd) << iblk <<  setw(wd) << stima_c << setw(wd) << glob_av[ic]/(double)iblk << setw(wd) << err_c << endl;
    //Heat.close();

    //Mag.open("output_mag_h.dat",ios::app);
    stima_m = blk_av[im]/blk_norm/(double)nspin;
    glob_av[im]  += stima_m;
    glob_av2[im] += stima_m*stima_m;
    err_m=Error(glob_av[im],glob_av2[im],iblk);
    //Mag << setw(wd) << iblk <<  setw(wd) << stima_m << setw(wd) << glob_av[im]/(double)iblk << setw(wd) << err_m << endl;
    //Mag.close();

    //Chi.open("output_chi_h.dat",ios::app);
    if(h != 0)  stima_x = beta*blk_av[ix]/blk_norm/(double)nspin-stima_m*stima_m*(double)nspin;
    else  stima_x = beta*blk_av[ix]/blk_norm/(double)nspin;
    glob_av[ix]  += stima_x;
    glob_av2[ix] += stima_x*stima_x;
    err_x=Error(glob_av[ix],glob_av2[ix],iblk);
    //Chi << setw(wd) << iblk <<  setw(wd) << stima_x << setw(wd) << glob_av[ix]/(double)iblk << setw(wd) << err_x << endl;
    //Chi.close();
    //cout << "----------------------------" << endl << endl;
}

void PrintEq(int iblk){
  ofstream Ene;
  const int wd=30;

  stima_u = blk_av[iu]/blk_norm/(double)nspin;
  glob_av[iu]  += stima_u;
  glob_av2[iu] += stima_u*stima_u;
  err_u=Error(glob_av[iu],glob_av2[iu],iblk);

  Ene.open("output_eq_ene_m_t.dat",ios::app);
  Ene << setw(wd) << iblk <<  setw(wd)<< stima_u<<setw(wd) << setw(wd) << err_u << endl;
  Ene.close();

}
void Print(void){
  ofstream Ene, Heat, Mag, Chi;
  const int wd=30;

  Ene.open("output_ene_h_m.dat",ios::app);
  Ene << setw(wd) << temp <<  setw(wd) << glob_av[iu]/(double)nblk << setw(wd) << err_u << endl;
  Ene.close();

  Heat.open("output_heat_h_m.dat",ios::app);
  Heat << setw(wd) << temp <<  setw(wd) << glob_av[ic]/(double)nblk << setw(wd) << err_c << endl;
  Heat.close();

  Chi.open("output_chi_h_m.dat",ios::app);
  Chi << setw(wd) << temp <<  setw(wd) << glob_av[ix]/(double)nblk << setw(wd) << err_x << endl;
  Chi.close();

  Mag.open("output_mag_h_m.dat",ios::app);
  Mag << setw(wd) << temp <<  setw(wd) << glob_av[im]/(double)nblk << setw(wd) << err_m << endl;
  Mag.close();
}
void Equilibration(int nblk_eq){
  for(int i = 0;i<nblk_eq;i++) move(metro);
}

void ConfFinal(void)
{
  ofstream WriteConf;

  //cout << "Print final configuration to file config.final " << endl << endl;
  WriteConf.open("config.final");
  for (int i=0; i<nspin; ++i)
  {
    WriteConf << s[i] << endl;
  }
  WriteConf.close();

  rnd.SaveSeed();
}

int Pbc(int i)  //Algorithm for periodic boundary conditions
{
    if(i >= nspin) i = i - nspin;
    else if(i < 0) i = i + nspin;
    return i;
}

double Error(double sum, double sum2, int iblk)
{
    if(iblk==1) return 0.0;
    else return sqrt((sum2/(double)iblk - pow(sum/(double)iblk,2))/(double)(iblk-1));
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
