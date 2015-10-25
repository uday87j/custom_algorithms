#ifndef MY_PARTITION_H_
#define MY_PARTITION_H_

#include <algorithm>
#include <iostream>
#include <iterator>

using std::cout;
using std::endl;

// custom algorithms (ca)
namespace ca    {

    extern void test_std_partition();
    extern void test_range_partition();

    template<typename T>
    void print_container(const T& b, const T& e, std::ostream& out = cout)  {
        std::copy(b, e, std::ostream_iterator<int>(out, " "));
    }

    // Input:
    // first: Iterator to beginning of a range
    // last : Iterator to end of the range
    // n    : The iterator whose pointee will be used as pivot
    // Output:
    // The range is paritioned by the pivot so that all the elements to the left of pivot are less than or equal to it
    // and those to the right of it are greater than it
    // Complexity: O(n)
    template<typename RANDOM_ITR>
        RANDOM_ITR pivot_partition(RANDOM_ITR first, RANDOM_ITR last, RANDOM_ITR n)    {

            //cout << "\n Swapping " << *first << " and " << *n << endl;
            std::swap(*first, *n);
            //print_container(first, last);   cout << endl;

            auto b  = std::next(first, 0);
            auto e  = std::next(last, -1);
            auto p  = *first;

            static auto swap_count  = 0;

            while(b < e)    {
                while(*b <= p)  std::advance(b, 1);
                while(*e >= p)  std::advance(e, -1);

                if(b < e)   {
                    //cout << "\nSwapping " << *b << " and " << *e << endl;
                    std::iter_swap(b, e);  ++swap_count;
                }
                
                //print_container(first, last);
                //cout << "\nf:" << *b << " p:" << p << " e: " << *e << endl;
            }

            if(e >= first)   {
                //cout << "\n Swapping " << *first << " and " << *e << endl;
                std::iter_swap(first, e);
                return e;
            }

            //print_container(first, last); cout << endl;
            //cout << "\nf:" << *b << " n:" << *n << " last: " << *e << endl;
            //cout << "\nNumber of swaps: " << swap_count << endl;
            return first;
        }

    template<typename RANDOM_ITR, typename UnaryPred>
        RANDOM_ITR pred_partition(RANDOM_ITR first, RANDOM_ITR last, UnaryPred p)   {
            auto insert_itr = first;

            while((insert_itr != last) && p(*insert_itr))  {
                //std::advance(insert_itr, 1);
                ++insert_itr;
            }

            auto itr    = insert_itr;
            while(itr != last)  {

                while((false == p(*itr)) && (itr != last)) {
                    //cout << "\n*itr: " << *itr << endl;
                    //std::advance(itr, 1);
                    ++itr;
                }

                if(itr == last) {
                    return insert_itr;
                }
                else    {
                    //cout << "\nSwapping " << *insert_itr << " and " << *itr << endl;
                    //std::swap(*insert_itr, *itr);
                    std::iter_swap(insert_itr, itr);
                    //print_container(first, last);
                    //std::advance(insert_itr, 1);
                    ++insert_itr;
                    //itr = std::next(insert_itr, 1);
                    //std::advance(itr, 1);
                    ++itr;
                }
            }
            return insert_itr;
        }
}
#endif
