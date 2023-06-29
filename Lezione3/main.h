#ifndef __main__
#define __main__
#include <armadillo>

using namespace arma;
using namespace std;

//local variables
int M=10000;              //Total number of throws
int N=100;                 //Number of blocks

Random rnd;
double S0 = 100, K = 100, r = 0.1, ave = 0.0, sigma = 0.25, T = 1, dt = 0.01;
vec put_vec(M), call_vec(M); double S_final;
mat output_mat(N,2);



//Functions

double S_t(double,double,double,double,double);

#endif // __funzioni__
