#ifndef CONTAINER_ERASE_H_
#define CONTAINER_ERASE_H_

#include <iostream>
#include <algorithm>
#include <vector>
#include <map>

namespace ca    {

    extern void test_erase();

    struct vector_tag   {};
    struct list_tag   {};
    struct map_tag   {};

    template<typename Container>
        struct container_t  {};

    //template<>
    //    struct container_t<std::vector>  {
    //        typedef vector_tag this_type;
    //    };


    //template<typename Container>
    //    void print_container(Container c, const std::string delim = " ", std::ostream& ost = std::cout)   {
    //        for(auto& v : c)    ost << v << delim;
    //    }

    template<typename Container, typename Value>
        void erase_sequence_container(Container& c, Value v)  {
            c.erase(std::remove(begin(c), std::end(c), v), std::end(c));
        }

    template<typename Container, typename Key>
        void erase_assoc_container(Container& c, const Key k)   {
            c.erase(k);
        }

    template<typename T, typename A>
        void erase(std::vector<T, A>& c, const T v)    {
            erase_sequence_container(c, v);
        }

    template<typename Key, typename T, typename Compare, typename A>
        void erase(std::map<Key, T, Compare, A>& m, const Key k)    {
            erase_assoc_container(m, k);
        }
}
#endif
