#ifndef CHIRAL_H
#define CHIRAL_H

#include "master.h"
#include "system.h"
#include <math.h>
#include <cmath>
#include <complex>
#include "eigen3/Eigen/Dense"
//#include "NLO2opt/chipot_f90_wrapper_"

class CHIRAL : public System
{
private:
    bool vecDelta(Eigen::VectorXi v1, Eigen::VectorXi v2);
public:
    const double    pi     = M_PI;
    double          m_m    = 0;
    double          m_L3   = 0;
    double          m_L2   = 0;
    double          m_L1   = 0;

    int             m_Nh   = 0;
    int             m_Nb   = 0;
    int             m_Ns   = 0;
    int             m_dk   = 0;

    Eigen::VectorXi below_fermi;
    Eigen::VectorXi above_fermi;
    Eigen::MatrixXi m_states;

    CHIRAL(class Master* master, double m, double L3, double L2, double L1);
    class   Master* m_master = nullptr;
    void    makeStateSpace  ();
    double  assym           (int p, int q, int r, int s);
    double  assym_single    (int p, int q);
    double  h0              (int p);
    double  f               (int p);
    int     kUnique1        (int p, int s1);
    int     kUnique2        (int p, int q, int s1, int s2);
    int     kUnique3        (int p, int q, int r, int s1, int s2, int s3);
    int     kUnique4        (int p, int q, int r, int s, int s1, int s2, int s3, int s4);
    int     kUnique5        (int p, int q, int r, int s, int t, int s1, int s2, int s3, int s4, int s5);
};

#endif // CHIRAL_H
