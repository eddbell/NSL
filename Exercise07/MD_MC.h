#ifndef __fluid__
#define __fluid__

//Random numbers
#include "../Libs/random/random.h"
int seed[4];
Random rnd;

//parameters, observables
const int m_props=1000, nbins = 100;
int n_props, iv, ik, it, ie, ip, iw, igr;
double vtail, ptail, bin_size, sd, dr;
double walker[m_props];

// averages
double blk_av[m_props], blk_norm, norm_g, accepted, attempted;
double glob_av[m_props], glob_av2[m_props];
double glob_av_g[nbins], glob_av2_g[nbins];
double stima_pot, stima_pres, stima_kin, stima_etot, stima_temp, stima_g;
double err_pot, err_pres, err_kin, err_etot, err_temp, err_g;
int nblk, nstep, eqtemp;

//configuration
const int m_part=108;
double x[m_part],    y[m_part],    z[m_part];
double xold[m_part], yold[m_part], zold[m_part];
double vx[m_part],  vy[m_part],   vz[m_part];

double blk_g[nbins];
double glob_g[nbins];
// thermodynamical state
int npart;
double beta,temp,energy,vol,rho,box,rcut,pres, delta;
int iNVET, restart;


//pigreco
const double pi=3.1415927;

//functions
void Input(void);
void Reset(int);
void Accumulate(void);
void Averages(int);
void Move(void);
void ConfFinal(void);
void ConfXYZ(int);
void Measure(void);
double Boltzmann(double, double, double, int);
double Pbc(double);
double Error(double,double,int);
double Force(int, int);
void PrintEq(int);

#endif