#ifndef MY_PARTITION_H_
#define MY_PARTITION_H_

#include <algorithm>
#include <iostream>
#include <iterator>

using std::cout;
using std::endl;

// custom algorithms (ca)
namespace ca    {

    template<typename T>
    void print_container(const T& b, const T& e, std::ostream& out = cout)  {
        std::copy(b, e, std::ostream_iterator<int>(out, ""));
    }

    template<typename RANDOM_ITR>
        void partition(RANDOM_ITR first, RANDOM_ITR n, RANDOM_ITR last)    {

            auto b  = first;
            auto e  = std::next(last, -1);

            cout << "\nf:" << *b << " n:" << *n << " last: " << *e << endl;
            
            while(b < e)    {
                
                while((*b <= *n) && (b < n)) std::advance(b, 1);
                while((*e >= *n) && (e > n)) std::advance(e, -1);
                cout << "\n Swapping " << *b << " and " << *e << endl;
                std::swap(*b, *e);

                print_container(first, last); cout << endl;
                cout << "\nf:" << *b << " n:" << *n << " last: " << *e << endl;
            }
            //print_container(first, last); cout << endl;
        }

}
#endif
