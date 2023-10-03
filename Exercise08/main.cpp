#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
#include "main.h"
#include "../Libs/random/random.h"
using namespace std;



int main(int argc, char *argv[])
{

  //eseguo la simulazione
  Input();

  cout<<"\n<INFO> Computing the simulated annealing for mu and sigma estimation: \n";

  if(SA != 0) {
      SimulatedAnnealing();
      FindBestParameters();
      cout<<" new mu ="<<mu<<"\t new sigma = "<<sig<<endl;
  }

  ofstream H, xpsi;
  H.open("output_H.dat",ios::app);
  xpsi.open("output_x_psi2.dat",ios::app);
  double cum_av2 = 0;  double cum_ave = 0;  double cum_err = 0;

  for(int i = 0;i<N;i++){
    double sum_ave = 0;
    ResetBlock();

    for(int j = 0;j< L;j++){
      sum_ave += Hamiltonian(x); //compute Hamiltonian
      x = metropsi2(x); //new step
      xpsi<<x<<endl;

    }
    cum_ave = (cum_ave*i + sum_ave/L)/(i+1);
    cum_av2 = (cum_av2*i + sum_ave*sum_ave/L/L)/(i+1);
    cum_err = Error(cum_ave,cum_av2,i+1);

    if ( (i+1)% ((int)N/10)== 0)
      cout<<"Block "<<i+1<<"   Acceptance rate: "<<(double)accepted/attempted<<endl;
    H<<i<<setw(15)<< cum_ave <<setw(15)<< cum_err <<endl;

  }
  H.close();
  xpsi.close();
  cout<<"H ave = "<< cum_ave <<" +- "<< cum_err <<endl;
  return 0;
}

void Input(void)
{
  ifstream ReadInput;

  rnd.init_random();

//Read input informations
  ReadInput.open("input.in");

  ReadInput >> SA;

  ReadInput >> mu;

  ReadInput >> sig;  //Eigenstate

  ReadInput >> delta;//delta;

  ReadInput >> delta_opt;//delta;

  ReadInput >> x_in;
  x = x_in;

  ReadInput >> M; //steps

  ReadInput >> N; //blocks
  L = M/N;
  ReadInput.close();
}

void ResetBlock(void){
  x = x_in;
  attempted = 0;
  accepted = 0;
}

double Gauss(double x) {
  return exp(-(x-mu)*(x-mu)/sig/sig/2.);
}


double Psi(double x) {
 return Gauss(x) + Gauss(-x);
}

double Psi2(double x){
  return Psi(x)*Psi(x);
}

double Kinetic(double x) {
    double a = (x-mu)*(x-mu)/(sig*sig);
    double b = (x+mu)*(x+mu)/(sig*sig);
    return 0.5/(sig*sig)*( 1. -( a*Gauss(x) + b*Gauss(-x) )/Psi(x) );
}

double Potenzial(double x) {
    return pow(x,4) -5./2.*pow(x,2);
}
double Hamiltonian(double x) {
    return Potenzial(x) + Kinetic(x);
}

void Print(int istep){
  ofstream Position;
  const int wd=30;
  Position.open("output_psi2.dat",ios::app);
  Position << istep;
  Position<<setw(wd)<<x;
  Position << endl;
  Position.close();
}

double Error(double sum, double sum2, int n)
{
    return (n==0 ? 0. : sqrt((sum2 - sum*sum)/n));
}

//metodi montecarlo
double metropsi2(double x){
  double x1, x0;
  double p_new = 0;
  double p_old = 0;

  x0 = x;
  x1 = x + rnd.Rannyu(-delta/2.,+delta/2.);

  p_new = Psi2(x1);
  p_old = Psi2(x0);

  attempted++;

  if (p_new > p_old){
    accepted++;
    return x1;
  }
  else{
    if (rnd.Rannyu()<p_new/p_old){
      accepted++;
      return x1;
    }
    return x0;
  }
};

void SimulatedAnnealing(void){

    const int n_opt_steps = 10000;
    int wd = 20;
    int beta_max = 1000;
    double b_step = 10;
    unsigned int temp_chill = 100;
    double H_old, H_new, H_err, mu_new, sig_new, mu_old, sig_old;
    double x_old = x;
    double x_new;

    ofstream opt_out;
    opt_out.open("out_opt.dat");

    beta = 0;
    while (beta < beta_max){
      if (beta < beta_max*0.1) beta+=b_step/10;
      else if (beta < beta_max*0.5) beta+=b_step;
      else beta+=b_step*5;
      SA_accepted=0;
      SA_attempted=0;

      for (unsigned int s{}; s< temp_chill;s++){

        H_old = 0;
        H_new = 0;

        x = x_old;

        for(int i{}; i< n_opt_steps; ++i){
            x = metropsi2(x);
            H_old += Hamiltonian(x);
        }
        H_old /= n_opt_steps;
        mu_old = mu;
        sig_old = sig;
        //x + rnd.Rannyu()
        mu_new =fabs( mu + delta_opt*rnd.Rannyu(-1/2.,+1/2.) );
        sig_new =fabs( sig + delta_opt*rnd.Rannyu(-1/2.,+1/2.) );
        //in our situation the sign of mu and sigma is equivalent
        mu = mu_new;
        sig = sig_new;


        x = x_old; //Reset of the starting position

        for(int i{}; i< n_opt_steps; ++i){
            x = metropsi2(x);
            H_new += Hamiltonian(x);
        }
        H_new /= n_opt_steps;
        x_new = x;
        //calculate H with mu and sigma, then evolve mu and sigma and recalculate H with the new parameters
        //then use Metropolis with Boltzmann weight, if accept, use mu and sigma as new parameters
        //start each time from x0
        //every few cycles lower the temperature

        if(rnd.Rannyu() <= exp( beta*(H_old-H_new)) ){
            SA_accepted++;
        }
        else{
          sig = sig_old;
          mu = mu_old;
        }
        SA_attempted++;
      }


      opt_out<< beta<<setw(wd) << mu << setw(wd)<< sig << setw(wd) << H_new <<endl;
      cout<<"beta = "<< beta<<setw(wd)<<" SA acceptation rate = "<<SA_accepted/SA_attempted<<endl;
    }


    opt_out.close();
}

void FindBestParameters(void) {
    ifstream in;
    double beta_dummy, m_dummy , s_dummy, h_dummy;
    in.open("out_opt.dat");

    in>>beta_dummy>>mu>>sig>>h;
    string riga;
    while (std::getline(in, riga)){
        in>>beta_dummy>>m_dummy>>s_dummy>>h_dummy;
        if(h_dummy < h) {
            mu = m_dummy;
            sig = s_dummy;
            h = h_dummy;
        }
    }
    in.close();
}
