#include "diagrams.h"

#include <eigen3/Eigen/Dense>

Diagrams::Diagrams()
{
}

/*The makeIxJBlock(ku,i1,i2,i3,i4) functions work as follows:
 * ku:      this is the channel with which we need to perform the diagram product
 * i1, i2:  these are the indices of the rows
 * i3, i4:  these are the indices of the columns
 */

void Diagrams::setIntClass(class MakeIntMat* intClass){
    m_intClass = intClass;
}

void Diagrams::setAmpClass(class MakeAmpMat* ampClass){
    m_ampClass = ampClass;
}

void Diagrams::La(int ku){
    //Eigen::MatrixXd product = m_ampClass->Amplitudes[index]*m_intClass->Vpppp[index];
    //return 0.5*product;
}

void Diagrams::Lb(int ku){
    //Eigen::MatrixXd product = m_intClass->Vhhhh[index]*m_ampClass->Amplitudes[index];
    //return 0.5*product;
}

void Diagrams::Lc(int ku){
    //int ku = m_intClass->sortVec_hh[index];
    //Eigen::MatrixXd product;// = m_intClass->Vhphp[index]*m_ampClass->Amplitudes;

    //return -product;
}

void Diagrams::Qa(int ku){
    Eigen::MatrixXd mat1 = m_ampClass->make2x2Block(ku,0,0,1,1, m_ampClass->T_elements);    // t_ij^ab
    Eigen::MatrixXd mat2 = m_intClass->make2x2Block(ku,0,0,1,1);    // v_ij^ab
    Eigen::MatrixXd mat3 = m_ampClass->make2x2Block(ku,0,0,1,1, m_ampClass->T_elements);    // t_ij^ab
    Eigen::MatrixXd product = 0.25*mat1*mat2.transpose()*mat3;                  // (t_ij^cd)(v_cd^kl)(t_kl^ab)

    m_ampClass->make2x2Block_inverse(product,ku,0,0,1,1, m_ampClass->T_elements_new);
    //return 0.25*product;
}

void Diagrams::Qb(int ku){
    Eigen::MatrixXd mat1 = m_ampClass->make2x2Block(ku,0,1,0,1, m_ampClass->T_elements);
    Eigen::MatrixXd mat2 = m_intClass->make2x2Block(ku,0,1,0,1);
    Eigen::MatrixXd mat3 = m_ampClass->make2x2Block(ku,0,1,0,1, m_ampClass->T_elements);
    Eigen::MatrixXd product= 0.5*mat1*mat2*mat3;
    m_ampClass->make2x2Block_inverse(product, ku, 0,1,0,1, m_ampClass->T_elements_new);
    //return 0.5*product;
}

void Diagrams::Qc(int ku){
    Eigen::MatrixXd product;
    //return -0.5*product;
}

void Diagrams::Qd(int ku){
    Eigen::MatrixXd product;
    //return -0.5*product;
}
