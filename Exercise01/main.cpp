#include <iostream>
#include <fstream>
#include <string>
#include "../Libs/random/random.h"
#include "../Libs/block/block.h"
#include "main.h"
#include <cmath>
#include <vector>

using namespace std;

//functions
void save_matrix(const vector<vector<double>>&, const string&);

int main(int argc, char *argv[]) {

    Random rnd;
    rnd.init_random();

    //###############ESERCIZIO 1.1.1########################################
    int M = 10000; // Numero totale di lanci
    int N = 100;   // Numero di blocchi

    vector<double> r(M);

    cout <<endl<< "ESERCIZIO 1.1.1 - AVERAGE calculation\n" << endl;

    for (int i = 0; i < M; i++) {
        r[i] = rnd.Rannyu();
    }
    Block esercizio1(N, r);
    esercizio1.Average(); //compute the average (and the ave^2) for all the blocks
    esercizio1.Average_cumulative("./data/dati1.1.1.dat"); //compute the average (and the ave^2) for block 0; 0,1; 0,1,2; ...

    cout << "___________________________________________________________________________________" << endl<< endl;
    //###############ESERCIZIO 1.1.2########################################
    cout << "ESERCIZIO 1.1.2 - VARIANZA calculation\n" << endl;
    esercizio1.Varianza(1/2.);
    esercizio1.Average_cumulative("./data/dati1.1.2.dat");

    cout << "___________________________________________________________________________________" << endl<< endl;
    //###############ESERCIZIO 1.1.3########################################

    M = 100;
    double n = 10000;
    vector<double> chi_cum(M);

    cout << "ESERCIZIO 1.1.3 - CHI2 calculation\n" << endl;
    ofstream outputFile( "./data/chi2.dat");    //print it in output file
    cout<<"<INFO> Computing the CHI2 comulative averages: ";

    auto chi = [] (double ni , int n, int M ) { //funzione per il calcolo del chi^2
       return (n == 0 ? 0 : ( pow(ni - n/M , 2 ) / ( n / M )) );
    };

    for(int j=0; j<100; j++){ //n of chi2
      float chi_val = 0;
      for(int i=0; i<M; i++){ //n of blocks
        unsigned int count = 0;
        for(int k=0; k<n; k++){
          double ran = rnd.Rannyu();
          if(ran>=i/double(M) && ran<(i+1)/double(M)){
            count++;
          }
        }
        chi_cum[i]=chi(count,n,M);
        chi_val += chi_cum[i];
      }

      outputFile << chi_val;
      outputFile << "\n";
    }

    cout<<" DONE!"<<endl;
    cout << "___________________________________________________________________________________" << endl<< endl;

    //###############ESERCIZIO 1.2########################################

    M = 10000; // Numero totale di lanci
    N = 100;   // Numero di blocchi

    vector<double> N_e = {1, 2, 10, 100}; // Numero di elementi per blocco
    float t_n, t_e, t_l;
    vector<vector<double>> nor(M, vector<double>(N_e.size()));
    vector<vector<double>> esp(M, vector<double>(N_e.size()));
    vector<vector<double>> lor(M, vector<double>(N_e.size()));

    cout << "ESERCIZIO 1.2 - NORMAL, EXPONENTIAL, LORENTIAN distribution\n" << endl;
    cout<<"<INFO> Average for N = ";
    for (int i = 0; i < (int) N_e.size(); i++) {
      cout<<N_e[i]<<",";
        for (int j = 0; j < M; j++) {
            t_n = 0;
            t_e = 0;
            t_l = 0;
            for (int k = 0; k < N_e[i]; k++) {
                t_n += rnd.Rannyu() / N_e[i];
                t_e += rnd.Exp(1) / N_e[i];
                t_l += rnd.Lorentz(1) / N_e[i];
            }
            nor[j][i] = t_n;
            esp[j][i] = t_e;
            lor[j][i] = t_l;
        }
    }
    cout<<"... DONE!"<<endl;
    save_matrix(nor, "./data/normal.dat");
    save_matrix(esp, "./data/exponential.dat");
    save_matrix(lor, "./data/lorentian.dat");

    cout << "___________________________________________________________________________________" << endl<< endl;
    //###############ESERCIZIO 1.3########################################

    double n_pi = M;
    double l = 0.5;
    int lspazio = 100;
    double x_1, x_2, y_2, m, y;
    double y_1;
    vector<double> pi_value(M);

    cout<<"ESERCIZIO 1.3 - PI GRECO calculation\n"<<endl;

    for(int i = 0; i<M;i++){
     unsigned int cou = 0;
      for(int j = 0; j<n_pi;j++){
        //simulo un bastone con rotazione random in un punto random
        x_1 = rnd.Rannyu(0,lspazio);
        y_1 = rnd.Rannyu(0,lspazio);
        x_2 = rnd.Rannyu(0,lspazio);
        y_2 = rnd.Rannyu(0,lspazio);
        if(x_1==x_2) y = y_1+l;
        else if(y_1==y_2) y = y_1;
        else{
          m = abs((y_2-y_1)/(x_2-x_1));
          y = y_1 + sqrt(m*m*l*l/(1+m*m));
        }
        //verifico se il bastone interseca le linee orizzontali distanti 1
        if(trunc(y) != trunc(y_1) ){
          cou++;
        }
      }
      pi_value[i]= 2.*l/(cou/n_pi);
    }

    Block pigreco(N,pi_value);
    pigreco.Average();
    pigreco.Average_cumulative("./data/pigreco.dat");

    cout << "___________________________________________________________________________________" << endl<< endl;
    return 0;
}

// Funzione ausiliaria per salvare la matrice in formato CSV
void save_matrix(const vector<vector<double>>& data, const string& filename) {
    ofstream outputFile(filename);
    for (const auto& row : data) {
        for (size_t j = 0; j < row.size(); j++) {
            outputFile << row[j];
            if (j < row.size() - 1) {
                outputFile << ",";
            }
        }
        outputFile << "\n";
    }
    cout<<"<INFO> Saving output file in: "<<filename<<endl;
}
