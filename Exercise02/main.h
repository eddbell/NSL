#ifndef __main__
#define __main__

//esercizio 2.1
unsigned int M=10000;              //Total number of throws
unsigned int N=100;                 //Number of blocks
vector<double> r;
Random rnd;
Block MCb(N);

//esercizio 2.2

int asse;
double verso, ran1, ran2;
vector<double> r_vettore_d(3,0), r_vettore_c(3,0), r_vec_temp(M);
vector<vector<double>> r_con(M, vector<double>(N)); //continuum scenario
vector<vector<double>> r_dis(M, vector<double>(N)); //discrete scenario


#endif // __main__
