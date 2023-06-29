#ifndef __main__
#define __main__
#include <armadillo>
#include "../random/random.h"

using namespace arma;
using namespace std;

//Random numbers
int seed[4];
Random rnd;

//Global variables

//constant
double a0 = 1;//0.0529e-9;
int Ndim = 3, M, N, L;
double delta,accepted = 0,attempted = 0;
//configuration
vec x(Ndim);
// simulation
int iD,iGS;
//*********Functions*************************************

void Input(void);
double module(vec&);
double Error(double,double,int);
double GaussND(vec&);
double ESH1(vec&);
double ESH2(vec&);
vec NDmetropolis(vec&);
void PrintRate(int);
#endif // __funzioni__
