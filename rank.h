#ifndef RANK_H_
#define RANK_H_

#include "partition.h"

#include <iterator>

namespace ca    {
    extern void test_rank();

    template<typename RANDOM_ITR>
        RANDOM_ITR rank(RANDOM_ITR first, RANDOM_ITR pos, RANDOM_ITR last) {
            if (first == last ||
                    pos == last)  return first;
            
            auto pivot  = std::min(*first, *pos);   std::cout << "\nPivot: " << pivot << std::endl;
            
            auto pred   = [pivot](const int a)  {   return a <= pivot;   };

            auto p  = pred_partition(first, last, pred);
            //auto p  = std::partition(first, last, pred);
            std::cout << "\nArray: \n";   print_container(first, last);
            
            if (p != last)  std::cout << "\nPartitioned at " << *p << std::endl;
            
            if (p == pos)   {
                std::cout << "\nFound it\n";
                for(auto itr = p; itr != last; ++itr)    {
                    if(*itr < *p)   p = itr;
                }
                return p;
            }
            if (p < pos)    {
                std::cout << "\np < pos\n";
                return rank(std::next(p, 1), pos, last);
            }
            else    {
                std::cout << "\np > pos\n";
                return rank(first, pos, p);
            }
        }
}

#endif
