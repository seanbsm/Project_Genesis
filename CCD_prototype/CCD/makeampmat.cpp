#include "makeampmat.h"
#include <iostream>

MakeAmpMat::MakeAmpMat()
{
}

void MakeAmpMat::setIntClass(class MakeIntMat* intClass){
    m_intClass = intClass;
}

void MakeAmpMat::setSystem(class System* system){
    m_system = system;
}

void MakeAmpMat::setElements(){
    Amplitudes = m_intClass->Vhhpp;
    T_elements = m_intClass->Vhhpp_elements;
}

// "index" is the index for kUnique of Vhhpp[index],
// thus we use makeBlockMat to reconstruct block "index" for some kUnique from T_elements
Eigen::MatrixXd MakeAmpMat::makeBlockMat(int index){
    int range_lower_hh = m_intClass->boundsHolder_hhpp_hh(0,index);
    int range_upper_hh = m_intClass->boundsHolder_hhpp_hh(1,index);
    int range_lower_pp = m_intClass->boundsHolder_hhpp_pp(0,index);
    int range_upper_pp = m_intClass->boundsHolder_hhpp_pp(1,index);

    int dim_hh = range_upper_hh - range_lower_hh;
    int dim_pp = range_upper_pp - range_lower_pp;
    Eigen::MatrixXd returnMat;
    returnMat.conservativeResize(dim_hh, dim_pp);
    for (int i = range_lower_hh; i<range_upper_hh; i++){
        for (int j = range_lower_pp; j<range_upper_pp; j++){
            returnMat(i-range_lower_hh, j-range_lower_pp) = m_intClass->Vhhpp_elements[m_intClass->Identity((m_intClass->blockArrays_hh)(1,i),
                                                                                                            (m_intClass->blockArrays_hh)(2,i),
                                                                                                            (m_intClass->blockArrays_pp)(1,j),
                                                                                                            (m_intClass->blockArrays_pp)(2,j))];
        }
    }
    return returnMat;
}

//for now we store the Fock matrix, but perhaps later it would be better to calculate it "on the fly"
//I deliberately declared a lot of ints here, but merely for easier debugging and readability
void MakeAmpMat::makeDenomMat(){
    //for (int i=0; i<m_intClass->sortVec_hh.size(); i++){   //remember, Vhhpp_i holds all kUnique for Vhhpp
    for (int i=0; i<m_intClass->numOfKu; i++){
        int lowBound_hh  = m_intClass->boundsHolder_hhpp_hh(0,i);
        int highBound_hh = m_intClass->boundsHolder_hhpp_hh(1,i);
        int lowBound_pp  = m_intClass->boundsHolder_hhpp_pp(0,i);
        int highBound_pp = m_intClass->boundsHolder_hhpp_pp(1,i);

        int dim_hh = highBound_hh - lowBound_hh;
        int dim_pp = highBound_pp - lowBound_pp;
        Eigen::MatrixXd newMat;
        newMat.conservativeResize(dim_hh, dim_pp);

        for (int hh=lowBound_hh; hh<highBound_hh; hh++){
            for (int pp=lowBound_pp; pp<highBound_pp; pp++){
                int ii = m_intClass->blockArrays_hh(1,hh);
                int jj = m_intClass->blockArrays_hh(2,hh);
                int aa = m_intClass->blockArrays_pp(1,pp);
                int bb = m_intClass->blockArrays_pp(2,pp);
                newMat(hh-lowBound_hh, pp-lowBound_pp) = 1/( (double)(m_system->f(ii) + m_system->f(jj) - m_system->f(aa) - m_system->f(bb)) );
            }
        }
        denomMat.push_back( newMat );
    }
}

void MakeAmpMat::make3x1Block_inverse(Eigen::MatrixXd inMat, int ku, int i1, int i2, int i3, int i4, std::map<int, double>& T_list){

}

void MakeAmpMat::make2x2Block_inverse(Eigen::MatrixXd inMat, int ku, int i1, int i2, int i3, int i4, std::map<int, double>& T_list){

    bool cond_hh1 = (i1 == 0 && i2 == 0);
    bool cond_hp1 = (i1 == 0 && i2 == 1);
    bool cond_pp1 = (i1 == 1 && i2 == 1);

    bool cond_hh2 = (i3 == 0 && i4 == 0);
    bool cond_hp2 = (i3 == 0 && i4 == 1);
    bool cond_pp2 = (i3 == 1 && i4 == 1);


    Eigen::MatrixXi blockArrays1_pointer;
    Eigen::MatrixXi blockArrays2_pointer;

    std::vector<int> sortVec1;
    std::vector<int> sortVec2;

    Eigen::MatrixXi indexHolder1_pointer;
    Eigen::MatrixXi indexHolder2_pointer;


    // 0 0
    if (cond_hh1){
        blockArrays1_pointer = m_intClass->blockArrays_hh;
        sortVec1             = m_intClass->sortVec_hh;
        indexHolder1_pointer = m_intClass->indexHolder_hh;
    }
    // 0 1
    else if (cond_hp1){
        blockArrays1_pointer = m_intClass->blockArrays_hp;
        sortVec1             = m_intClass->sortVec_hp;
        indexHolder1_pointer = m_intClass->indexHolder_hp;
    }
    // 1 1
    else {
        blockArrays1_pointer = m_intClass->blockArrays_pp;
        sortVec1             = m_intClass->sortVec_pp;
        indexHolder1_pointer = m_intClass->indexHolder_pp;
    }

    // 0 0
    if (cond_hh2){
        blockArrays2_pointer = m_intClass->blockArrays_hh;
        sortVec2             = m_intClass->sortVec_hh;
        indexHolder2_pointer = m_intClass->indexHolder_hh;
    }
    // 0 1
    else if (cond_hp2){
        blockArrays2_pointer = m_intClass->blockArrays_hp;
        sortVec2             = m_intClass->sortVec_hp;
        indexHolder2_pointer = m_intClass->indexHolder_hp;
    }
    // 1 1
    else {
        blockArrays2_pointer = m_intClass->blockArrays_pp;
        sortVec2             = m_intClass->sortVec_pp;
        indexHolder2_pointer = m_intClass->indexHolder_pp;
    }

    int length1 = indexHolder1_pointer.cols();
    int length2 = indexHolder2_pointer.cols();

    int index1; int index2;

    auto it1 = std::find(sortVec1.begin(), sortVec1.end(), ku);
    if (it1 == sortVec1.end()){
        std::cout << "make2x2Block_inverse in MakeAmpMat, kUnique not found for rows" << std::endl;
        std::exit;
    }
    else{
      index1 = distance(sortVec1.begin(), it1);
    }

    auto it2 = std::find(sortVec2.begin(), sortVec2.end(), ku);
    if (it2 == sortVec2.end()){
        std::cout << "make2x2Block_inverse in MakeAmpMat, kUnique not found for columns" << std::endl;
        std::exit;
    }
    else{
      index2 = distance(sortVec2.begin(), it2);
    }

    int range_lower1 = indexHolder1_pointer(0,index1);
    int range_upper1 = indexHolder1_pointer(1,index1);
    int range_lower2 = indexHolder2_pointer(0,index2);
    int range_upper2 = indexHolder2_pointer(1,index2);

    for (int i = range_lower1; i<range_upper1; i++){
        for (int j = range_lower2; j<range_upper2; j++){
            T_list[m_intClass->Identity((blockArrays1_pointer)(1,i), (blockArrays1_pointer)(2,i), (blockArrays2_pointer)(1,j), (blockArrays2_pointer)(2,j))] += inMat(i-range_lower1,j-range_lower2);
        }
    }
}

//returns a block matrix of dimensions 3x1, currently only made for Vhhpp
// i1,i2,i3,i4 specify whether there is a hole or particle (by a 0 or 1)  index at index ij, for j=1-4
Eigen::MatrixXd MakeAmpMat::make3x1Block(int ku, int i1, int i2, int i3, int i4, std::map<int, double>& T_list){

    bool cond_hhh = (i1 == 0 && i2 == 0 && i3==0);
    bool cond_hhp = (i1 == 0 && i2 == 0 && i3==1);
    bool cond_hpp = (i1 == 0 && i2 == 1 && i3==1);
    bool cond_ppp = (i1 == 1 && i2 == 1 && i3==1);

    bool cond_h   = (i4 == 0);
    bool cond_p   = (i4 == 1);


    Eigen::MatrixXi blockArrays1_pointer;
    Eigen::MatrixXi blockArrays2_pointer;

    std::vector<int> sortVec1;
    std::vector<int> sortVec2;

    Eigen::MatrixXi indexHolder1_pointer;
    Eigen::MatrixXi indexHolder2_pointer;


    // 0 0 0
    if (cond_hhh){
        blockArrays1_pointer = m_intClass->blockArrays_hhh;
        sortVec1             = m_intClass->sortVec_hhh;
        indexHolder1_pointer = m_intClass->indexHolder_hhh;
    }
    // 0 0 1
    else if (cond_hhp){
        blockArrays1_pointer = m_intClass->blockArrays_hhp;
        sortVec1             = m_intClass->sortVec_hhp;
        indexHolder1_pointer = m_intClass->indexHolder_hhp;
    }
    // 0 1 1
    else if (cond_hpp){
        blockArrays1_pointer = m_intClass->blockArrays_hpp;
        sortVec1             = m_intClass->sortVec_hpp;
        indexHolder1_pointer = m_intClass->indexHolder_hpp;
    }
    // 1 1 1
    else{
        blockArrays1_pointer = m_intClass->blockArrays_ppp;
        sortVec1             = m_intClass->sortVec_ppp;
        indexHolder1_pointer = m_intClass->indexHolder_ppp;
    }

    // 0
    if (cond_h){
        blockArrays2_pointer = m_intClass->blockArrays_h;
        sortVec2             = m_intClass->sortVec_h;
        indexHolder2_pointer = m_intClass->indexHolder_h;
    }
    // 1
    else if (cond_p){
        blockArrays2_pointer = m_intClass->blockArrays_p;
        sortVec2             = m_intClass->sortVec_p;
        indexHolder2_pointer = m_intClass->indexHolder_p;
    }

    int length1 = indexHolder1_pointer.cols();
    int length2 = indexHolder2_pointer.cols();

    int index1; int index2;

    Eigen::MatrixXd returnMat;

    auto it1 = std::find(sortVec1.begin(), sortVec1.end(), ku);
    if (it1 == sortVec1.end()){
        returnMat.conservativeResize(1,1);
        returnMat(0,0) = 0;
        std::cout << "make3x1Block in MakeAmpMat, kUnique not found for rows" << std::endl;
      return returnMat;
    }
    else{
      index1 = distance(sortVec1.begin(), it1);
    }

    auto it2 = std::find(sortVec2.begin(), sortVec2.end(), ku);
    if (it2 == sortVec2.end()){
        returnMat.conservativeResize(1,1);
        returnMat(0,0) = 0;
        std::cout << "make3x1Block in MakeAmpMat, kUnique not found for columns" << std::endl;
      return returnMat;
    }
    else{
      index2 = distance(sortVec2.begin(), it2);
    }

    int range_lower1 = indexHolder1_pointer(0,index1);
    int range_upper1 = indexHolder1_pointer(1,index1);
    int range_lower2 = indexHolder2_pointer(0,index2);
    int range_upper2 = indexHolder2_pointer(1,index2);

    int dim1 = range_upper1 - range_lower1;
    int dim2 = range_upper2 - range_lower2;

    returnMat.conservativeResize(dim1, dim2);
    for (int i = range_lower1; i<range_upper1; i++){
        for (int j = range_lower2; j<range_upper2; j++){
            returnMat(i-range_lower1, j-range_lower2) = T_list[m_intClass->Identity((blockArrays1_pointer)(1,i), (blockArrays1_pointer)(2,i), (blockArrays1_pointer)(3,i), (blockArrays2_pointer)(1,j))];
        }
    }
    return returnMat;
}

//returns a block matrix of dimensions 2x2, currently only made for Vhhpp
// i1,i2,i3,i4 specify whether there is a hole or particle (by a 0 or 1) index at index ij, for j=1-4
Eigen::MatrixXd MakeAmpMat::make2x2Block(int ku, int i1, int i2, int i3, int i4, std::map<int, double>& T_list){

    //std::cout << "hey" << std::endl;
    bool cond_hh1 = (i1 == 0 && i2 == 0);
    bool cond_hp1 = (i1 == 0 && i2 == 1);
    bool cond_pp1 = (i1 == 1 && i2 == 1);

    bool cond_hh2 = (i3 == 0 && i4 == 0);
    bool cond_hp2 = (i3 == 0 && i4 == 1);
    bool cond_pp2 = (i3 == 1 && i4 == 1);


    Eigen::MatrixXi blockArrays1_pointer;
    Eigen::MatrixXi blockArrays2_pointer;

    std::vector<int> sortVec1;
    std::vector<int> sortVec2;

    Eigen::MatrixXi indexHolder1_pointer;
    Eigen::MatrixXi indexHolder2_pointer;


    // 0 0
    if (cond_hh1){
        blockArrays1_pointer = m_intClass->blockArrays_hh;
        sortVec1             = m_intClass->sortVec_hh;
        indexHolder1_pointer = m_intClass->indexHolder_hh;
    }
    // 0 1
    else if (cond_hp1){
        blockArrays1_pointer = m_intClass->blockArrays_hp;
        sortVec1             = m_intClass->sortVec_hp;
        indexHolder1_pointer = m_intClass->indexHolder_hp;
    }
    // 1 1
    else {
        blockArrays1_pointer = m_intClass->blockArrays_pp;
        sortVec1             = m_intClass->sortVec_pp;
        indexHolder1_pointer = m_intClass->indexHolder_pp;
    }

    // 0 0
    if (cond_hh2){
        blockArrays2_pointer = m_intClass->blockArrays_hh;
        sortVec2             = m_intClass->sortVec_hh;
        indexHolder2_pointer = m_intClass->indexHolder_hh;
    }
    // 0 1
    else if (cond_hp2){
        blockArrays2_pointer = m_intClass->blockArrays_hp;
        sortVec2             = m_intClass->sortVec_hp;
        indexHolder2_pointer = m_intClass->indexHolder_hp;
    }
    // 1 1
    else {
        blockArrays2_pointer = m_intClass->blockArrays_pp;
        sortVec2             = m_intClass->sortVec_pp;
        indexHolder2_pointer = m_intClass->indexHolder_pp;
    }

    int length1 = indexHolder1_pointer.cols();
    int length2 = indexHolder2_pointer.cols();

    int index1; int index2;

    Eigen::MatrixXd returnMat;

    auto it1 = std::find(sortVec1.begin(), sortVec1.end(), ku);
    if (it1 == sortVec1.end()){
        returnMat.conservativeResize(1,1);
        returnMat(0,0) = 0;
        std::cout << "make2x2Block in MakeAmpMat, kUnique not found for rows" << std::endl;
      return returnMat;
    }
    else{
      index1 = distance(sortVec1.begin(), it1);
    }

    auto it2 = std::find(sortVec2.begin(), sortVec2.end(), ku);
    if (it2 == sortVec2.end()){
        returnMat.conservativeResize(1,1);
        returnMat(0,0) = 0;
        std::cout << "make2x2Block in MakeAmpMat, kUnique not found for columns" << std::endl;
      return returnMat;
    }
    else{
      index2 = distance(sortVec2.begin(), it2);
    }

    int range_lower1 = indexHolder1_pointer(0,index1);
    int range_upper1 = indexHolder1_pointer(1,index1);
    int range_lower2 = indexHolder2_pointer(0,index2);
    int range_upper2 = indexHolder2_pointer(1,index2);

    int dim1 = range_upper1 - range_lower1;
    int dim2 = range_upper2 - range_lower2;

    returnMat.conservativeResize(dim1, dim2);
    for (int i = range_lower1; i<range_upper1; i++){
        for (int j = range_lower2; j<range_upper2; j++){
            returnMat(i-range_lower1, j-range_lower2) = T_list[m_intClass->Identity((blockArrays1_pointer)(1,i), (blockArrays1_pointer)(2,i), (blockArrays2_pointer)(1,j), (blockArrays2_pointer)(2,j))];
        }
    }
    return returnMat;
}
