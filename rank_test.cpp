#include "rank.h"

#include <iostream>
#include <array>
#include <random>

using namespace ca;
using namespace std;

namespace ca    {
    void test_rank()    {
        std::array<int, 7> iarr = {};
        std::random_device rd;

        std::iota(begin(iarr), end(iarr), 1);
        std::shuffle(begin(iarr), end(iarr), rd);

        cout <<"\nInput array:\n";  print_container(begin(iarr), end(iarr));

        auto barr   = begin(iarr);
        auto earr   = end(iarr);
        
        auto n      = 1;
        auto piv_itr  = std::next(barr, std::distance(barr, earr) / 2);
        n   = iarr.size() / 2;
        piv_itr     = std::next(barr, n);   //2nd largest element
        
        //auto pivot  = *piv_itr;
        //cout << "\nPivot: " << pivot << endl;
        
        //auto pred   = [pivot](const int a)    {
        //            return std::less_equal<int>()(a, pivot);
        //        };
        
        auto median_itr = rank(barr, piv_itr, earr);
        //auto median_itr = std::partition(barr, earr, pred);
        //cout <<"\nPartitioned array:\n";  print_container(begin(iarr), end(iarr));
        cout << "\nThe " << n + 1 << "th largest of this array is: " << *median_itr << endl;
#ifdef RUN_ME
        std::array<int, 3> arr  = {{3, 1, 2}};
        auto barr   = begin(arr);
        auto earr   = end(arr);
        auto itr    = std::partition(begin(arr), end(arr), [](const int a){ return a <= 1;   });
        cout << *itr << endl;
        print_container(barr, earr);
        print_container(begin(arr), end(arr));
#endif
    }
}
