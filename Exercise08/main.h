#ifndef __main__
#define __main__

#include "../random/random.h"
#include <armadillo>

using namespace arma;
//Random numbers
int seed[4];
Random rnd;

//Global variables
int N, L;
double sumE = 0;
double E = 0, E2 = 0;
//function parameter
double x = 1,sigma = 1, mu = 1;

//statistics
int accepted = 0,attempted = 0, SA;
double delta; double beta = 1;



//*********Functions*************************************

void Input();
//calcola il modulo
double Module(vec&);
//funzioni algebriche
double Gauss1D(double);
double GS2(double);
double V(double);
double Energy(double);
//metodi montecarlo
double Mstep(double);
//block statistics
double Error(double,double,int);
void PrintE(double,double,int);
void CAverage();
#endif
