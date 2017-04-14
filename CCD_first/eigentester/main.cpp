#include <iostream>
#include <math.h>
#include <cmath>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Core>
#include <map>
#include <unordered_map>
#include <chrono>

#include <iomanip>
#include <cstdlib>
#include <string>
#include <fstream>

//#include <unsupported/Eigen/CXX11/Tensor>

//#include <mpi.h>
#include <omp.h>
//#include <stdio.h>
#include "testclass.h"

typedef std::chrono::high_resolution_clock Clock;   //needed for timing

using namespace std;

template <typename T> //credit: http://stackoverflow.com/questions/1577475/c-sorting-and-keeping-track-of-indexes#12399290
vector<size_t> sort_indexes(const vector<T> &v) {

    // initialize original index locations
    vector<size_t> idx(v.size());
    iota(idx.begin(), idx.end(), 0);

    // sort indexes based on comparing values in v
    sort(idx.begin(), idx.end(),
         [&v](size_t i1, size_t i2) {return v[i1] < v[i2];});

    return idx;
}

/*bool myfunction (int i, int j) {
  return (i==j);
}*/
bool contractor(int i, int j){ return i==j; }

int* returnInt(int i){
    return &i;
}

bool vecDelta(Eigen::VectorXi v1, Eigen::VectorXi v2){
    int dim1 = v1.rows();
    int dim2 = v2.rows();
    if (dim1 != dim2){
        cout << "dimensional error" << endl;
        return 0;
    }
    else{
        bool returnInt = 1;
        for (int i =0; i<dim1; i++){
            returnInt *= ( v1(i)==v2(i) );
        }
        return returnInt;
    }
}



int main(int argc, char** argv)
{

    //std::cout << 14 % 6 << std::endl;

    /*ofstream myfile;
    ostringstream os;
    os << "verytemp.txt";
    string s = os.str();

    for (int i=0; i<20; i++){
        myfile.open(s, ios::app);
        myfile <<  i << "\n" << std::flush;
        myfile.close();
    }*/


    std::unordered_map<int, double> T;
    int size = 1e6;

    for (int i = 0; i<size; i++){
        T[i] = (double)i*3;
    }

    std::vector<double> T2;
    T2.resize(size);


    #pragma omp parallel for num_threads(4)
    for (int j=0; j<size; j++){
        int val = j*3;
        T2[j] = T[val];
    }

    cout << "sup" << endl;

    int sum_of_elems = 0;
    std::for_each(T2.begin(), T2.end(), [&] (int n) {
        sum_of_elems += n;
    });

    cout << sum_of_elems << endl;

    /*for (int m = 8; m<9; m++){
        Eigen::initParallel();

        omp_set_num_threads(m);
        Eigen::setNbThreads(m);

        int n = 4000;

        auto t1 = Clock::now();
        //Eigen::MatrixXd A = Eigen::MatrixXd::Random(n,n);
        //Eigen::MatrixXd B = Eigen::MatrixXd::Random(n,n);
        //Eigen::MatrixXd C;

        std::cout << "start" << std::endl;

        //C = A*B;
        //Eigen::MatrixXd C = A*B;
        Eigen::MatrixXd C = Eigen::MatrixXd::Random(n,n)*Eigen::MatrixXd::Random(n,n);
        auto t2 = Clock::now();


        std::cout << "end. time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " milliseconds" << std::endl;
    }*/


    /*std::unordered_map<int, double> T;
    std::vector<int> D1;
    std::vector<int> D2;
    std::vector<int> D3;

    int limit = 1e5;

    for (int i = 0; i<limit; i++){
        T[i] = limit-1-i;
        D1.push_back(i);
        D2.push_back(limit-1-i);
        D3.push_back(i+1);
    }

    testClass* Tester = new testClass;

    Tester->D1 = D1;
    Tester->D2 = D2;

    auto t1 = Clock::now();

    for (int j=0; j<limit; j++){
        //std::cout << D3[T[j]] << std::endl;
        std::cout << D3[Tester->testy( j )] << std::endl;
    }
    //std::cout << "wut" << std::endl;
    auto t2 = Clock::now();

    std::cout << "end. time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
              << " milliseconds" << std::endl;*/

    //MPI_Init (&argc, &argv);	/* starts MPI */
    //int rank, size;
    //MPI_Comm_rank (MPI_COMM_WORLD, &rank);	/* get current process id */
    //MPI_Comm_size (MPI_COMM_WORLD, &size);	/* get number of processes */

    //cout << size << endl;
    //MPI_Finalize();
    /*Eigen::initParallel();
    Eigen::setNbThreads(4);

    int nthreads = Eigen::nbThreads( );
    std::cout << "THREADS = " << nthreads << std::endl; // returns '1'

    Eigen::Matrix<int, 3, 10> m1;
    Eigen::Matrix<int, 3, 4> m2;

    Eigen::Vector4i v1(1,2,3,4);
    Eigen::Vector4i v2(1,2,3,4);

    //Eigen::VectorXi v = Eigen::VectorXi(1,2,3);

    Eigen::Matrix<int, 5, 1> vec;
    vec << 1,2,3,4,5;


    m1 << 1,2,3,3,4,5,2,3,1,4,
                4,5,6,2,4,6,1,3,6,3,
                7,8,9,4,5,2,6,3,2,4;*/


    /*int xLim = 2e3;
    int yLim = 2e3;
    Eigen::MatrixXi M;
    std::vector<int> V;
    map<int, int> Map;
    M.conservativeResize(xLim,yLim);

    //#pragma omp parallel for
    for (int x=0; x<xLim; x++){
        for (int y=0; y<yLim; y++){
            //V.push_back( x+y );
            Map[x+y] = x+y;
        }
    }

    int a = 2147483647;
    std::cout << a << std::endl;

    auto t1 = Clock::now();

    //int omp_get_num_threads( );
    //cout << omp_get_num_threads() << endl;
    #pragma omp parallel for
    for (int x=0; x<xLim; x++){
        for (int y=0; y<yLim; y++){
            M(x,y) = Map[y*x];
            M(x,y) += Map[x+y];
        }
    }

    auto t2 = Clock::now();

    std::cout << "Total time used: "
              << std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count()
              << " " << std::endl;*/

    //MPI_Finalize();

    /*for (int i=0; i<1e3; i++){
        Vec.push_back(i+1);
    }
    int val = 9;
    int index;
    auto it = std::find(Vec.begin(), Vec.end(), val);
    if (it == Vec.end()){
      cout << "value not in map" << endl;// name not in vector
    }
    else{
      index = distance(Vec.begin(), it);
    }

    cout << Vec[index] << endl;

    Vec[index] = 12;

    cout << Vec[index] << endl;

*/

    /*map<int, double> places1;
    map<int, double> places2;

    for (int i=0; i<1e7; i++){
        places1[i] = (double) i;
        places2[i] = (double) 1e3-i;
    }

    cout << "done" << endl;
    cout << places1[1e6] << endl;*/
/*
    places1 = places2;
    //places1[9999] += 2;
    cout << places1[9999] << endl;


    std::vector<int> v3_temp;
    for (int i=0; i<1e3; i++){
        v3_temp.push_back(i);
    }
    cout << v3_temp[0] << endl;

    //VectorXf::Map(pointer, size)
      //  double* ptr = &v[0];
    //Eigen::VectorXi v3;

    int* ptr = &v3_temp[0];
    Eigen::Map<Eigen::VectorXi> v3(ptr, v3_temp.size());
    //cout << v3 << endl;


    std::vector<int> array2 = { 9, 7, 5, 3, 1 };*/



    /*Eigen::VectorXi vec1;
    vec1 << 1,1,1,1,1,1,2,2,2,2,2,2,2,3,3,3,3,3,3,4,4,4,4,5,5,5,5,5,5,5,5,6,6,6,6;
    Eigen::VectorXi vec2;
    vec2 << -1,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,0,1,1,1,2,2,2,2,3,3,3,3,3,4,4,4,5,5,5,5,5,5,5,5,6,6,6,6,7,7,7;*/

    //cout << vecDelta(v1+v1,v2+v2) << endl;

    /*for (int i= 0; i<1; i++){
        cout << i << endl;
    }*/

    /*m1 << 1,2,3,3,4,5,2,3,1,4,
            4,5,6,2,4,6,1,3,6,3,
            7,8,9,4,5,2,6,3,2,4;

    std::vector<int> v = {1,2,3,4,5,6,7,8};

    //Eigen::Vector4f v(1,2,3,4);
    v.erase( unique( v.begin(), v.end() ), v.end() );

    //cout << v.size() << endl;
    for (int i; i<v.size(); i++){
        cout << v[i] << endl;
    }
    int a = 5;
    cout << returnInt(a) << endl;

    m2 << 1,1,1,1,
            1,1,1,1,
            1,1,1,1;

    cout << ((m2.transpose())*m2).trace() << endl;*/
}
