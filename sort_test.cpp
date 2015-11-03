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

template<partition_algo_t alg = PRED_PART>
void time_qsort(bool use_qsort = false, size_t samples = 100)   {
    array<int, 200000> iarr = {};
    std::random_device rd;
    typedef typename std::chrono::time_point<std::chrono::system_clock>::rep rep;
    std::vector<rep> timings(samples, rep());

    for(auto i = 0; i < samples; ++i)   {
        std::iota(begin(iarr), end(iarr), 5);
        std::reverse(begin(iarr), end(iarr));
        std::shuffle(begin(iarr), end(iarr), rd);
        //cout << "\nOriginal array:\n";
        //print_container(begin(iarr), end(iarr));  cout << endl;

        auto start  = system_clock::now();

        if(use_qsort)   std::sort(begin(iarr), end(iarr));
        else            ca::qsort<decltype(begin(iarr)), alg>(begin(iarr), end(iarr));
        
        auto finish = system_clock::now();
        timings.push_back(duration_cast<milliseconds>(finish - start).count());
        //cout << "\nTime taken to sort using " << alg << " partition: " << 
        //    duration_cast<milliseconds>(finish - start).count() << " milli-seconds\n";
        //print_container(begin(iarr), end(iarr));  cout << endl;

        // Check if sorted properly
        std::transform(begin(iarr), std::next(end(iarr), -1), 
                std::next(begin(iarr), 1), begin(iarr),
                [](const int a, const int b)    {
                assert(a <= b);
                return a;
                });
    }
    auto sum_samples    = std::accumulate(begin(timings), end(timings), 0);
    cout << sum_samples << endl;
    cout << "\n Avg. time to sort on " << samples << " samples using parition algo " << alg <<
        " is; " << static_cast<double>(sum_samples*1.0/samples) << endl;
}

namespace ca  {

    void test_qsort()  {
        time_qsort<STD_PART>();
        time_qsort<PIVOT_PART>();
        time_qsort<PRED_PART>();
        time_qsort(true);
    }
}
