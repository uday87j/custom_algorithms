#include "partition.h"

#include <iostream>
#include <array>
#include <algorithm>
#include <iterator>
#include <random>
#include <chrono>
#include <cassert>

using namespace std;
using namespace std::chrono;

namespace ca  {

    void test_range_partition()  {

        array<int, 2000000> iarr = {};
        std::random_device rd;
        auto barr   = begin(iarr);
        auto earr   = end(iarr);

        std::iota(barr, earr, 5);
        std::reverse(barr, earr);

        //std::shuffle(barr, earr, rd);
        //cout << "\nOriginal array:\n";
        //print_container(barr, earr);  cout << endl;

        auto n  = std::next(barr, std::distance(barr, earr)/2);
        auto p  = *n;
        cout << "\nPivot partitioning the array: " << *n << "\n";

        auto pred   = [p](const int i) {
                        return i <= p;
                    };

        auto start  = system_clock::now();
        auto split  = std::partition(barr, earr, pred);
        auto finish = system_clock::now();
        cout << "\nTime taken to std::partition: " << 
            duration_cast<nanoseconds>(finish - start).count() << " nano-seconds\n";

        std::sort(begin(iarr), end(iarr));
        //std::shuffle(begin(iarr), end(iarr), rd);
        std::reverse(begin(iarr), end(iarr));
        start  = system_clock::now();
        split  = pivot_partition(barr, earr, n);
        finish = system_clock::now();
        cout << "\nTime taken to pivot_partition: " << 
            duration_cast<nanoseconds>(finish - start).count() << " nano-seconds\n";

        std::sort(begin(iarr), end(iarr));
        //std::shuffle(begin(iarr), end(iarr), rd);
        std::reverse(begin(iarr), end(iarr));
        start  = system_clock::now();
        split  = pred_partition(barr, earr, pred);
        finish = system_clock::now();
        cout << "\nTime taken to pred_partition: " << 
            duration_cast<nanoseconds>(finish - start).count() << " nano-seconds\n";

        //print_container(begin(iarr), std::end(iarr));  cout << endl;
        cout << "\nSplit occurs at: " << *split << endl;
    }

    void test_std_partition()   {
        array<int, 10> iarr = {};
        std::random_device rd;
        auto barr   = begin(iarr);
        auto earr   = end(iarr);

        std::iota(barr, earr, 5);
        std::shuffle(barr, earr, rd);
        cout << "\nOriginal array:\n";
        print_container(barr, earr);  cout << endl;
        
        auto n  = std::next(barr, std::distance(barr, earr)/2);
        auto p  = *n;
        cout << "\nPivot partitioning the array: " << *n << "\n";

        cout << "\nTesting std::partition:\n";
        auto start  = system_clock::now();
        auto split  = std::partition(barr, earr, [p](const int i) {
                //return i <= p;
                return i <= 200;
                });
        auto end    = system_clock::now();
        cout << "\nSplit occurs at: " << *split << endl;
        if(split == earr)   cout << "\nlast: " << earr << ", but std::partition() returns 0 as *split !" << endl;
        cout << "\nTime taken to std::partition: " << duration_cast<nanoseconds>(end - start).count() << " nano-seconds\n";
        print_container(begin(iarr), std::end(iarr));  cout << endl;
    }
}
