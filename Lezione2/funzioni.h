#ifndef __funzioni__
#define __funzioni__
#include <armadillo>

//esercizio 1
int accept_reject(double,double);
double integranda(double), retta(double), parabola(double), retta_cum(double);
//esercizio 2
arma::vec& polar(double, double,arma::vec&);
double norma(arma::vec&);

#endif // __funzioni__
