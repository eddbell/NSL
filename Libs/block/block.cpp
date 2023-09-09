#include <cmath>
#include <iostream>
#include <fstream>
#include <vector> // Includi la libreria per std::vector
#include <string>
#include <utility> // for std::pair
#include "block.h"

using namespace std;

Block::Block(int n, vector<double>& R) {
    m_N = n;  //number of blocks
    m_M = R.size();
    vector<double> av2(n);
    vector<double> ave(n);
    m_av2 = av2;
    m_ave = ave;
    m_R = R;
}

Block::Block(int n) {
    m_N = n;
}

Block::~Block() {}

double Block::error(vector<double>& AV, vector<double>& AV2, int n) {
  return ( n == 0 ? 0 : sqrt((AV2[n] - AV[n] * AV[n]) / n));
}

//declare the initial vector
void Block::set_vec(vector<double>& R) {
    m_R = R;
    m_M = R.size();
    vector<double> av2(m_N);
    vector<double> ave(m_N);
    m_av2 = av2;
    m_ave = ave;
}

//modifica(crea)  m_ave =  medie a blocchi, m_av2 = medie a blocchi^2 (utile per varianza)
//modifica(crea)  m_av2 includendo le medie a blocchi

vector<double>& Block::Average() {
    double L = static_cast<double> (m_M) / m_N;
    cout<<"<INFO> Computing Blocks average: ";
    for (int i = 0; i < m_N; i++) {
      double sum_ave = 0;
      for (int j = 0; j < L; j++) {
          int k = j + i * static_cast<int>(L);
          sum_ave += m_R[k];
      }
      m_ave[i] = sum_ave / L;
      m_av2[i] = m_ave[i] * m_ave[i];
      if (i%10==0) cout<<".";
    }
    cout<<" DONE!"<<endl;

    return m_ave;
}

//compute varianza ave and var ave^2 per ogni blocco
void Block::Varianza(double teo) {
    cout<<"<INFO> Computing Blocks varianza: ";
    double L = static_cast<double>(m_M) / m_N;
    for (int i = 0; i < m_N; i++) {
      double sum_var = 0;
      for (int j = 0; j < L; j++) {
          int k = j + i * static_cast<int>(L);
          sum_var += (m_R[k] - teo) * (m_R[k] - teo);
      }
      m_ave[i] = sum_var / L;
      m_av2[i] = m_ave[i] * m_ave[i];
      if (i%10==0) cout<<".";
    }
    cout<<" DONE!"<<endl;
}

//once I have executed Varianza or Average, one can find the comulative...
void Block::Average_cumulative(string filename) {

    cout<<"<INFO> Computing the comulative averages of the blocks: ";
    vector<double> ave_cum(m_N);
    vector<double> av2_cum(m_N);

    ofstream outputFile(filename);    //print it in output file

    for (int i = 0; i < m_N; i++) {
        for (int j = 0; j < i + 1; j++) {
            ave_cum[i] += m_ave[j];
            av2_cum[i] += m_av2[j];
        }
        ave_cum[i] /= (i + 1);
        av2_cum[i] /= (i + 1);

        if (i%10==0) cout<<".";
        outputFile << ave_cum[i];
        outputFile << ",";
        outputFile << error(ave_cum, av2_cum, i);
        outputFile << "\n";
    }
    cout<<" DONE!"<<endl;
    cout<<"<INFO> Saving output file in: "<<filename<<endl;

}

pair<double, double> Block::RW_Ray_Average() {

    double ave_sum = 0;
    double av2_sum = 0;

    for (int i = 0; i < m_N; i++) {
        ave_sum += sqrt(m_ave[i]);
        av2_sum += sqrt(m_av2[i]);
    }
    ave_sum /= m_N;
    av2_sum /= m_N;

    return make_pair(ave_sum,sqrt((av2_sum - ave_sum*ave_sum)/m_N));
}
