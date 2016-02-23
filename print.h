#ifndef CONTAINER_PRINT_H_
#define CONTAINER_PRINT_H_

#include <iostream>
#include <algorithm>
#include <vector>
#include <map>

namespace ca    {

    template<typename Container>
        void print_sequence_container(const Container c, const std::string delim = " ", std::ostream& ost = std::cout)   {
            for(auto& v : c)    ost << v << delim;
        }

    template<typename Container>
        void print_assoc_container(const Container c, const std::string delim = " ", std::ostream& ost = std::cout)   {
            for(auto& v : c)    ost << "(" << v.first << "," << v.second << ")" << delim;
        }

    template<typename T, typename A>
        void print_container(const std::vector<T, A>& v)    {
            print_sequence_container(v);
        }

    template<typename Key, typename T, typename Compare, typename A>
        void print_container(const std::map<Key, T, Compare, A> m)    {
            print_assoc_container(m);                
        }
}

#endif
