#ifndef RANK_H_
#define RANK_H_

#include "partition.h"

#include <iterator>

namespace ca    {
    extern void test_rank();

    template<typename RANDOM_ITR, typename UnaryPred>
        RANDOM_ITR rank(RANDOM_ITR first, RANDOM_ITR pos, RANDOM_ITR last, UnaryPred pred) {
            if (first == last)  return first;
            auto p  = pred_partition(first, last, pred);
            std::cout << "\nArray: \n";   print_container(first, last);
            if (p != last)   std::cout << "\nPartitioned at " << *p << std::endl;
            if (p == pos)   return p;
            if (p < pos) return rank(std::next(p, 1), pos, last, pred);
            if (p > pos) return rank(first, pos, p, pred);
            return first;   //Dummy
        }
}

#endif
