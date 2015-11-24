#include "closest_pair.h"
#include "print.h"

#include <array>
#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <random>
#include <algorithm>
#include <cassert>

#define IP_BEG  begin(ipoints)
#define IP_END  end(ipoints)
#define FP_BEG  begin(fpoints)
#define FP_END  end(fpoints)

using namespace std;

namespace ca    {    

    void test_closest_pair()    {
        // Create a vector of co-ordinates
        std::vector<ipoint_t> ipoints(50, {0, 0});
        gen_ipoint_t igen(0, 200, 5);
        std::generate(IP_BEG, IP_END, igen);
        cout << "\nint points:\n";  print_sequence_container(ipoints);
        ofstream of1("ci.dat");
        print_sequence_container(ipoints, "\n", of1);

        // Invoke closest_pair() to return co-ordinates of 2 closest points
        auto cp     = closest_pair(ipoints);
        cout << "\nThe closest pair: " << cp << endl;
        cout << "\nDistance between them: " << distance(cp.p1, cp.p2) << endl;
        ofstream of2("cp_ci.dat");
        of2 << cp;
    }

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

        ofstream of1("ci.dat");
        print_sequence_container(ipoints, "\n", of1);

        ofstream of2("cf.dat");
        print_sequence_container(fpoints, "\n", of2);

        // Test distance
        assert(ipoints.size() > 1);
        auto dist   = distance(ipoint_t(8, 1), ipoint_t(9, 9));
        cout << "\nDistance between (8, 1) & (9, 9): " << dist << endl;
        //assert(dist == 8.06226);
    }
}
