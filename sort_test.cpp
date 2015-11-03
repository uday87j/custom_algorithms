#include "sort.h"

#include <iostream>
#include <array>
#include <algorithm>
#include <iterator>
#include <random>
#include <chrono>
#include <cassert>

using namespace std;
using namespace ca;
using namespace std::chrono;

template<partition_algo_t alg>
void time_qsort()   {
    array<int, 2000000> iarr = {};
    std::random_device rd;

    std::iota(begin(iarr), end(iarr), 5);
    std::reverse(begin(iarr), end(iarr));
    std::shuffle(begin(iarr), end(iarr), rd);
    //cout << "\nOriginal array:\n";
    //print_container(begin(iarr), end(iarr));  cout << endl;

    auto start  = system_clock::now();
    ca::qsort<decltype(begin(iarr)), alg>(begin(iarr), end(iarr));
    auto finish = system_clock::now();
    cout << "\nTime taken to sort using " << alg << " partition: " << 
        duration_cast<milliseconds>(finish - start).count() << " milli-seconds\n";
    //print_container(begin(iarr), end(iarr));  cout << endl;

    // Check if sorted properly
    std::transform(begin(iarr), std::next(end(iarr), -1), 
            std::next(begin(iarr), 1), begin(iarr),
            [](const int a, const int b)    {
            assert(a <= b);
            return a;
            });
}

namespace ca  {

    void test_qsort()  {
        time_qsort<STD_PART>();
        time_qsort<PIVOT_PART>();
        time_qsort<PRED_PART>();
    }
}
