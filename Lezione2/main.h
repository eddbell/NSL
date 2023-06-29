#ifndef __main__
#define __main__

//esercizio 2.1
int M=10000;              //Total number of throws
int N=100;                 //Number of blocks
vec r(M);  mat MC_ave_mat(N,2); vec ran(M);
Random rnd;
Block MCb(N);

//esercizio 2.2
int asse, co = 0;
double verso,ran1, ran2;
mat r_dis(M,N), r_con(M,N), MW_mat(N,2);
vec r_vettore_d(3,fill::zeros), r_vettore_c(3,fill::zeros), r2(M*N), r_vec_temp(N);
Block MWb(N);
#endif // __main__
