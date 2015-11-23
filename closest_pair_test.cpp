#include "closest_pair.h"
#include "print.h"

#include <array>
#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <random>
#include <algorithm>

#define IP_BEG  begin(ipoints)
#define IP_END  end(ipoints)
#define FP_BEG  begin(fpoints)
#define FP_END  end(fpoints)

using namespace std;

namespace ca    {    

    void test_closest_pair()    {}

    void test_cp_methods()    {

        point_t<int32_t> p1{3, 4};
        cout << p1 << endl;

        fpoint_t f1{3.14, 5.87};
        cout << f1 << endl;

        std::vector<ipoint_t> ipoints(10, {0, 0});
        gen_ipoint_t igen(0, 10);
        std::generate(IP_BEG, IP_END, igen);
        cout << "\nint points:\n";  print_sequence_container(ipoints);

        cout << endl;

        std::sort(IP_BEG, IP_END, &x_less_than<int32_t>);
        cout << "\nSort on p.x:\n";  print_sequence_container(ipoints);

        cout << endl;

        std::sort(IP_BEG, IP_END, &y_less_than<int32_t>);
        cout << "\nSort on p.y:\n";  print_sequence_container(ipoints);

        cout << endl;

        std::vector<fpoint_t> fpoints(10, {0, 0});
        gen_fpoint_t fgen(0, 10.0);
        std::generate(FP_BEG, FP_END, fgen);
        cout << "\nfloat points:\n";  print_sequence_container(fpoints);

        cout << endl;

        std::sort(FP_BEG, FP_END, &x_less_than<float>);
        cout << "\nSort on p.x:\n";  print_sequence_container(fpoints);

        cout << endl;

        std::sort(FP_BEG, FP_END, &y_less_than<float>);
        cout << "\nSort on p.y:\n";  print_sequence_container(fpoints);

        cout << endl;

        ofstream ofile("cp.dat");
        print_sequence_container(ipoints, "\n", ofile);
    }
}
