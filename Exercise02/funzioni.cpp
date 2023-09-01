#include <cmath>
#include <cstdlib>
#include <vector>
#include "funzioni.h"

using namespace std;

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

void polar(double phi, double theta, vector<double>& ray){
  ray[0] += sin(phi)*cos(theta);
  ray[1] += sin(phi)*sin(theta);
  ray[2] += cos(phi);
};

double norma(vector<double>& r){
  double n = 0;
  for(long unsigned int i = 0; i<r.size();i++){
    n = n+r[i]*r[i];
  }
  return n;
};
