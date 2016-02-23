#ifndef MY_PARTITION_H_
#define MY_PARTITION_H_

#include "ca_util.h"

#include <algorithm>
#include <iostream>
#include <iterator>

using std::cout;
using std::endl;

// custom algorithms (ca)
namespace ca    {

    extern void test_std_partition();
    extern void test_range_partition();

    enum partition_algo_t   {
        STD_PART    = 0,
        PIVOT_PART,
        PRED_PART
    };

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

            std::swap(*first, *n);

            auto b  = std::next(first, 0);
            auto e  = std::next(last, -1);
            auto p  = *first;

            static auto swap_count  = 0;

            while(b < e)    {
                while(*b <= p)  std::advance(b, 1);
                while(*e >= p)  std::advance(e, -1);

                if(b < e)   {
                    std::iter_swap(b, e);  ++swap_count;
                }
            }

            if(e >= first)   {
                std::iter_swap(first, e);
                return e;
            }

            return first;
        }

    // Input:
    // first: Iterator to beginning of a range
    // last : Iterator to end of the range
    // p    : The predicate that decides which element belongs to group 1
    // Output:
    // The range is paritioned by the predicate so that all the input is partitioned into 2 groups
    // Group 1 evaluates to true for predicate & Group 2 to false
    // Return:
    // Iterator to first memeber of Group 2 or
    // last if all members of input evaluate to Group 1
    // Complexity: O(n)
    template<typename RANDOM_ITR, typename UnaryPred>
        RANDOM_ITR pred_partition(RANDOM_ITR first, RANDOM_ITR last, UnaryPred p)   {
            if (first == last)  return first;

            auto insert_itr = first;

            while((insert_itr != last) && p(*insert_itr))  {
                ++insert_itr;
            }

            auto itr    = insert_itr;
            while(itr != last)  {

                while((false == p(*itr)) && (itr != last)) {
                    ++itr;
                }

                if(itr == last) {
                    return insert_itr;
                }
                else    {
                    std::iter_swap(insert_itr, itr);
                    ++insert_itr;
                    ++itr;
                }
            }
            return insert_itr;
        }
}
#endif
