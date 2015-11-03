#ifndef MY_SORT_H_
#define MY_SORT_H_

#include "partition.h"

#include <cassert>
#include <iostream>

namespace ca    {

    extern void test_qsort();

    template<typename RandItr, partition_algo_t alg>
        void qsort(RandItr first, RandItr last) {

            //Base cases
            if(std::distance(first, last) <= 1) return;

            if(std::distance(first, last) == 2) {
                auto second = std::next(first, 1);
                if(*first > *second)   std::iter_swap(first, second);
                return;
            }

            auto pivot  = (std::next(first, std::distance(first, last)/2));
            assert(pivot != last);
            
            auto p  = std::min(*pivot, *first);
            auto pred   = [p](const int i)  {
                return i <= p;
            };
            auto mid    = first;

            if(alg == PIVOT_PART)       mid    = ca::pivot_partition(first, last, pivot);
            
            else if(alg == STD_PART)    mid    = std::partition(first, last, pred);

            else if(alg == PRED_PART)   mid    = ca::pred_partition(first, last, pred);

            assert(mid != last);
            
            qsort<RandItr, alg>(first, mid);
            qsort<RandItr, alg>(mid, last);
        }
}

#endif
