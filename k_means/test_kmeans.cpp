#include "kmeans.h"
#include <iostream>

using namespace std;
using namespace ca;

void test_genT()    {

    genT<> igen(10, -10);

    for (auto i = 1; i < 10; ++i)    { cout << igen() << endl; }

    genT<float, uniform_real_distribution<> > fgen(-100.0, 30.0);

    for (auto i = 1; i < 10; ++i)    { cout << fgen() << endl; }
}

void test_kmeans(size_t num_pts = 10, size_t num_clus = 2)  {

    vector<fpoint_t> points(num_pts);
    gen_fpoint_t gen_fpt;

    for (auto& p : points)  {
        p = gen_fpt();
    }

    //for (auto& p : points)  {
    //    cout << p << endl;
    //}
    
    auto clusters = kmeans<fpoint_t, 3>(points);
    assert(clusters.size() == 3);

    for (auto& c : clusters)    {
        cout << "\nCluster: ";
        cout << c << endl;
    }
}

int main()  {

    //test_genT();
    
    test_kmeans();

    return 0;
}
