#include "partition.h"

#include <iostream>
#include <array>
#include <algorithm>
#include <iterator>
#include <random>

using namespace std;
using ca::print_container;

int main()  {

    array<int, 10> iarr = { {1, 5, 8, 2, 3, 7, 0, 9, 4, 6 } };
    std::random_device rd;
    auto barr   = begin(iarr);
    auto earr   = end(iarr);
    print_container(barr, earr);  cout << endl;

    //std::iota(barr, earr, 0);
    //print_container(barr, earr);  cout << endl;

    //std::shuffle(barr, earr, rd);
    //print_container(barr, earr);  cout << endl;

    auto n  = std::next(barr, std::distance(barr, earr)/2);
    ca::partition(barr, n, earr);

    print_container(barr, n);  cout << " ";
    cout << *n << " ";
    print_container(std::next(n, 1), earr); cout << endl;


    return 0;
}
