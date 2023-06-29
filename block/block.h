#ifndef __Block__
#define __Block__

#include <armadillo>
using namespace arma;
using namespace std;

class Block{

  private:
    int m_M; int m_N;
    vec m_av2; vec m_ave;
    vec m_R;  mat m_ave_mat;
    rowvec m_mean;
  protected:

  public:
    // constructors
    Block(int);
    Block(int, vec&);
    // destructor
    ~Block();
    
    // methods
    void set_vec(vec&); //introduco elementi singoli del vettore

    double error(vec&,vec&,int);

    vec& Average();

    void Varianza(double);

    void Average_cumulative(const string); 

    void Average_blocks(const string);
};

#endif // __Block__
