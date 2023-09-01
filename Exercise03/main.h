#ifndef __main__
#define __main__
#include <vector>

using namespace std;

//local variables
int M=10000;              //Total number of throws
int N=100;                 //Number of blocks

Random rnd;
double S0 = 100, K = 100, r = 0.1, ave = 0.0, sigma = 0.25, T = 1, dt = 0.01;
vector<double> put_vec, call_vec;
double S_final;

#endif // __funzioni__
