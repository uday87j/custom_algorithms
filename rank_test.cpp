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
        auto piv_itr  = std::next(barr, std::distance(barr, earr) / 2);
        auto pivot  = *piv_itr;
        cout << "\nPivot: " << pivot << endl;
        auto median_itr = rank(barr, earr, piv_itr,
                [pivot](const int a)    {
                    return std::less_equal<int>()(a, pivot);
                });
        cout << "\nThe median of this array is: " << *median_itr << endl;
    }
}
