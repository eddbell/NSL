#ifndef __main__
#define __main__
#include "../Libs/random/random.h"
#include <vector>
using namespace std;

//Random numbers
int seed[4];
Random rnd;

//Global variables

//blocks statistic constants
int M, N, L;
//metropolis psi2
double delta, accepted = 0, attempted = 0;

//Simulation Annealing
bool SA;
double delta_opt, SA_accepted = 0, SA_attempted = 0;
double beta, temp; //boltz

//configuration
double x_in, x, sig, mu, h;



//*********Functions*************************************


void Input(void);
double Error(double,double,int);
void ResetBlock(void);

double Psi(double);
double Psi2(double);
double Gauss(double);

double Kinetic(double);
double Potenzial(double);
double Hamiltonian(double);

void SimulatedAnnealing(void);
double metropsi2(double);
void PrintRate(int);

void FindBestParameters(void);
#endif // __funzioni__
