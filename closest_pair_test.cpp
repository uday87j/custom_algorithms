#include "closest_pair.h"
#include "print.h"

#include <array>
#include <iostream>
#include <utility>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

namespace ca    {    

    void test_closest_pair()    {

        point_t<int32_t> p1{3, 4};
        cout << p1 << endl;

        fpoint_t f1{3.14, 5.87};
        cout << f1 << endl;

        std::vector<fpoint_t> fpoints(10, {0, 0});
        gen_fpoint_t fgen(0, 10.0);
        std::generate(begin(fpoints), end(fpoints), fgen);
        print_sequence_container(fpoints);
    }
}
