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

    void test_pivot_partition() {
        array<int, 1000000> iarr = {};
        std::random_device rd;
        auto barr   = begin(iarr);
        auto earr   = end(iarr);

        std::iota(barr, earr, 0);
        std::reverse(barr, earr);

        std::shuffle(barr, earr, rd);
        //cout << "\nOriginal array:\n";
        //print_container(barr, earr);  cout << endl;

        auto iarr2  = iarr;
        auto barr2  = begin(iarr2);
        auto earr2  = std::end(iarr2);
        auto n2     = std::next(barr2, std::distance(barr2, earr2)/2);

        auto n  = std::next(barr, std::distance(barr, earr)/2);
        auto p  = *n;
        cout << "\nPivot partitioning the array: " << *n << "\n";

        cout << "\nTesting pivot_partition:\n";
        auto start  = system_clock::now();
        auto split  = std::partition(barr, earr, [p](const int i) {
                return i <= p;
                });
        auto end    = system_clock::now();
        cout << "\nSplit occurs at: " << *split << endl;
        //duration<double> sec    = (end - start);
        cout << "\nTime taken to std::partition: " << duration_cast<nanoseconds>(end - start).count() << " nano-seconds\n";
        //print_container(begin(iarr), std::end(iarr));  cout << endl;

        
        //print_container(barr2, earr2);  cout << endl;

        cout << "Pivot n2: " << *n2 << endl;
        p   = *n2;

        start  = system_clock::now();
        //pivot_partition(barr2, earr2, n2);
        split   = pred_partition(barr2, earr2, [p](const int i) {
                return i <= p;
                });
        end    = system_clock::now();
        cout << "\nSplit occurs at: " << *split << endl;
        //sec    = (end - start);
        cout << "\nTime taken to partition: " << duration_cast<nanoseconds>(end - start).count() << " nano-seconds\n";
        //print_container(barr2, earr2);  cout << endl;
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
