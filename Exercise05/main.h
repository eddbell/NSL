#ifndef __main__
#define __main__
#include "../Libs/random/random.h"
#include <vector>
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
vector<double> x(Ndim);
// simulation
int iD,iGS;
//*********Functions*************************************

void Input(void);
double module(vector<double>&);
double Error(double,double,int);
double GaussND(double);
double ESH1(double);
double ESH2(vector<double>&);
vector<double> NDmetropolis(vector<double>&);
void PrintRate(int);
#endif // __funzioni__
