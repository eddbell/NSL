#include <iostream>
#include <fstream>
#include <string>
#include <armadillo>
#include <iomanip>
#include <cmath>
#include "main.h"

using namespace std;
using namespace arma;

int main(int argc, char *argv[])
{
  Input(); //Inizialization
  int N_init = 0;//2000 for solid... 5k for liquid
  for(int i= 0;i<N_init;i++) Move();

  for(int iblk=1; iblk <= nblk; iblk++) //Simulation
  {
    Reset(iblk);   //Reset block averages
    for(int istep=1; istep <= nstep; istep++)
    {
      Move();
      Measure();
      Accumulate(); //Update block averages
      Print(istep); //print coordinate
    }
    Averages(iblk);   //Print results for current block
  }
  ConfFinal(); //Write final configuration

  return 0;
}

void Input(void)
{
  ifstream ReadInput, Primes, Seed;
  double x_temp;
//Read seed for random numbers
  int p1, p2;
  Primes.open("Primes");
  Primes >> p1 >> p2 ;
  Primes.close();

//Read input informations
  ReadInput.open("input.in");

  ReadInput >> iD;  //Gauss o uniform?

  ReadInput >> iGS;  //Eigenstate?

  ReadInput >> restart;

  ReadInput >> delta;

  ReadInput >> x_temp;
  x.fill(x_temp);

  ReadInput >> nblk;//N

  ReadInput >> nstep;//M/N
  ReadInput.close();

//Read seed input informations
  if(restart) Seed.open("seed.out");
  else Seed.open("seed.in");
  Seed >> seed[0] >> seed[1] >> seed[2] >> seed[3];
  rnd.SetRandom(seed,p1,p2);
  Seed.close();

  Measure();
}

void Move()
{

  double p;
  vec x1(Ndim);

  for(int i = 0;i<Ndim;i++){
    if(iD==0) x1[i] = rnd.Rannyu(x[i]-delta,x[i]+delta);
    else x1[i] = rnd.Gauss(x[i],delta);

    if(iGS==0) p = GaussND(x1)/GaussND(x);  //N dimension gaussian
    else if(iGS == 1) p = ESH1(x1)/ESH1(x);  //first eigenstate hidrogen atom
    else p = ESH2(x1)/ESH2(x);  //second eigenstate hidrogen atom

    if(p>1 or rnd.Rannyu()<p){
        x = x1;
        accepted++;
      }
    attempted++;
    }
  return;
}

void Measure() //Properties measurement
{
  r_ave = module(x);
  return;
}

void Reset(int iblk) //Reset block averages
{

   if(iblk == 1)
   {
     glob_av = 0;
     glob_av2 = 0;
   }
   blk_av = 0;
   blk_norm = 0;
   attempted = 0;
   accepted = 0;
}

void Accumulate(void) //Update block averages
{
  blk_av = blk_av + r_ave;
  blk_norm++;
}

void Averages(int iblk) //Print results for current block
{

  ofstream Radius;
  const int wd=30;

  cout << "Block number " << iblk << endl;
  cout << "Acceptance rate " << accepted/attempted << endl << endl;

  Radius.open("output_rad_iD_iGS.dat",ios::app);
  stima_radius = blk_av/blk_norm/(double)nstep; //Potential energy
  glob_av += stima_radius;
  glob_av2 += stima_radius*stima_radius;
  err_radius=Error(glob_av,glob_av2,iblk);

  //Write
  Radius << setw(wd) << iblk <<  setw(wd) << stima_radius << setw(wd) << glob_av/(double)iblk << setw(wd) << err_radius << endl;
  Radius.close();
}

void ConfFinal(void){

}

double Error(double sum, double sum2, int iblk)
{
    return sqrt(fabs(sum2/(double)iblk - pow(sum/(double)iblk,2))/(double)iblk);
}

//algebric functions

double ESH1(vec& x1){
  return exp(-2*module(x1));
};

double ESH2(vec& x1){
  return module(x1)*exp(-2*module(x1));//pow(a0,-3.)
};
double GaussND(vec& x1){
  return exp(-(module(x1)-5)*(module(x1)-5));
};

//calcola il modulo
double module(vec& v){
  double mod = 0;
  for(int i=0;i<Ndim;i++){
    mod += v[i]*v[i];
  }
  return sqrt(mod);
};

void Print(int istep){
  ofstream Position;
  const int wd=30;
  Position.open("output_pos_iD_iGS.dat",ios::app);
  Position << istep;
  for(int i = 0; i< Ndim;i++) Position<<setw(wd)<<x[i];
  Position << endl;
  Position.close();
}
/*
double Error(double sum, double sum2, int iblk)
{
    return sqrt(fabs(sum2/(double)iblk - pow(sum/(double)iblk,2))/(double)iblk);
}
  //Inizializzo la classe random
  rnd.init_random(rnd);

  //imposto i valori iniziali per la simulazione MC
  a0 = 0.0529e-9;  //raggio dell'atomo
  x.fill(a0);  //valori iniziali del vettore cordinata
  delta = a0;//a0;   //varianza della distribuzione nota

  //eseguo la simulazione
  for(int i=0;i<M;i++){
    x = NDim_unif(x);
    for(int j=0;j<Ndim;j++){
      x_mat(i,j) = x[j];
    }
    x_mat(i,Ndim)= eigenstateh2(x);
    psi[i]  = -a0*log(x_mat(i,Ndim)*M_PI*a0*a0*a0);
  }

  x_mat.save("../dati/dati5/cordinate_cartesiane.dat", csv_ascii);

  MTb.set_vec(psi);
  MTb.Average();
  MTb.Average_cumulative("../dati/dati5/psi.dat");
}*/
