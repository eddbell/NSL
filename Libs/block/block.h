#ifndef __Block__
#define __Block__
#include <vector>
#include <utility> // for std::pair
using namespace std;

class Block {

private:
    int m_M;
    int m_N;
    vector<double> m_av2;
    vector<double> m_ave;
    vector<double> m_R;
    vector<vector<double>> m_ave_mat;

protected:

public:
    // constructors
    Block(int);
    Block(int, vector<double>&);

    // destructor
    ~Block();

    // methods
    void set_vec(vector<double>&);

    double error(vector<double>&, vector<double>&, int);

    vector<double>& Average();

    void Varianza(double);

    void Average_cumulative(const string);

    pair<double, double> RW_Ray_Average();
};

#endif // __Block__
