#include <cmath>
#include <cstdlib>
#include <armadillo>
#include "funzioni.h"

using namespace arma;
using namespace std;

int accept_reject(double x,double y){
  if(x>y) return(1);
  else  return(0);
};

double integranda(double x){//g*p
  return M_PI/2*cos(M_PI/2*x);
};
double retta(double x){//retta normalizzata
  return(M_PI/2-x*M_PI/2)*(4/M_PI);
  //return (M_PI/2-x*x*M_PI/2)*(3/M_PI);
};
double parabola(double x){//parabola normalizzata
  return (M_PI/2-x*x*M_PI/2)*(3/M_PI);
};
double retta_cum(double x){//inversa della cumulativa della retta
  return 1-sqrt(1-x);//-1/2 + sqrt(8*x + M_PI)/(2*sqrt(M_PI));
};

vec& polar(double phi, double theta,vec& r){
  r(0) = r(0) + sin(phi)*cos(theta);
  r(1) = r(1) + sin(phi)*sin(theta);
  r(2) = r(2) + cos(phi);
  return r;
};

double norma(vec& r){
  double n = 0;
  for(int i = 0; i<r.n_elem;i++){
    n = n+r(i)*r(i);
  }
  return n;
};
