#include <cmath>
#include <cstdlib>
#include "block.h"
#include <armadillo>
#include <string>
using namespace arma;
using namespace std;

Block :: Block(int n, vec& R){
  m_N = n;
  m_M = R.n_elem;
  vec av2(n); vec ave(n);
  m_av2 = av2; m_ave =ave;
  m_R = R;
};
Block :: Block(int n){
  m_N = n;
};
Block :: ~Block(){};

double Block :: error(vec &AV,vec &AV2, int n){
  if (n==0){
    return 0;
  }
  else{
    return sqrt((AV2[n] - AV[n]*AV[n])/n);
  }
}

void Block :: set_vec(vec &R){
  m_R = R;
  m_M = R.n_elem;
  vec av2(m_N); vec ave(m_N);
  m_av2 = av2; m_ave =ave;
};
//calcolo l'averages di ogni blocco

vec& Block :: Average(){
  double L=m_M/m_N;

  for(int i=0; i<m_N; i++){
   float sum_ave = 0;
   for(int j=0; j<L; j++){
     int k = j+i*L;
     sum_ave += m_R(k);
   }
   m_ave(i) = sum_ave/L;
   m_av2[i] = m_ave[i]*m_ave[i];
 }
 return m_ave;
}

//calcolo l'varianza di ogni blocco

void Block :: Varianza(double teo){
  double L=m_M/m_N;
  for(int i=0; i<m_N; i++){
   float sum_var = 0;
   for(int j=0; j<L; j++){
     int k = j+i*L;
     sum_var += (m_R[k]-teo)*(m_R[k]-teo);
   }
   m_ave(i) = sum_var/L;
   m_av2[i] = m_ave[i]*m_ave[i];
 }
 //m_ave = m_var;  //in questo modo applico la funzione Average_cumulative direttamente sulla varianza
}

//calcolo avereages comulativo

void Block :: Average_cumulative(string link){
  mat ave_mat(m_N,2);
  m_ave_mat = ave_mat;
  vec ave_cum(m_N); vec av2_cum(m_N);
  for(int i=0; i<m_N; i++){
   for(int j=0; j<i+1; j++){
     ave_cum(i) += m_ave(j);//sommo con i precedenti
     av2_cum(i) += m_av2(j);
   }
   ave_cum(i)/=(i+1); //divido per il numero di elementi per avere la media
   av2_cum(i)/=(i+1); //Cumulative square average
   m_ave_mat(i,1)= error(ave_cum,av2_cum,i);
   m_ave_mat(i,0)= ave_cum(i);
  }
  m_ave_mat.save(link, csv_ascii);
}

void Block :: Average_blocks(string link){
  double ave_sum = 0; double av2_sum = 0; double ave_sum_err;
  for(int i=0; i<m_N; i++){
    ave_sum += sqrt(m_ave(i));//sommo con i precedenti
    av2_sum += sqrt(m_av2(i));
  }
  ave_sum = ave_sum/m_N;
  av2_sum = av2_sum/m_N;
  ave_sum_err= sqrt((av2_sum - ave_sum*ave_sum)/m_N);
  m_mean = {ave_sum,ave_sum_err};
  m_mean.save(link, csv_ascii);
}
