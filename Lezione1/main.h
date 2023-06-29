#ifndef __main__
#define __main__

Random rnd;
int M=10000;              //Total number of throws
int N=100;                 //Number of blocks

//esercizio 1.1

vec r(M);mat mat_cum(N,2);

double n = 10000; int N_times = 100;
vec chi_cum(N_times); uvec n_vec(M);
unsigned int cou;double ran;


//esercizio 1.2
vec N_e = {1,2,10,100}; //numero elementi per blocco
int sizeN = N_e.n_rows;
float t_n,t_e,t_l;
mat nor(M,sizeN); mat esp(M,sizeN); mat lor(M,sizeN);


//esercizio 1.3
double n_pi = M;      //numero lanci di bastone per pi
double l = 0.5;
int lspazio = 100;
double x_1, x_2, y_2, m, y;
double y_1;
vec pi_value(M);

#endif // __main__
