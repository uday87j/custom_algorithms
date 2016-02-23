#ifndef CA_UTIL_H_
#define CA_UTIL_H_

#include <random>
#include <limits>
#include <iostream>
#include <iterator>

namespace ca    {
    
    template<typename T, typename V>
        void print_container(const T& b, const T& e, std::ostream& out = std::cout)  {
            std::copy(b, e, std::ostream_iterator<V>(out, " "));
        }

    template<bool unique = false, typename T = int32_t, typename DISTRIBUTION = std::uniform_int_distribution<> >
        struct genT {

            genT(const T minT =std::numeric_limits<T>::min(), const T maxT = std::numeric_limits<T>::max())
                : dis_(minT, maxT)  {
                    
                    if (minT > maxT)    {
                        typename DISTRIBUTION::param_type p(maxT, minT);
                        dis_.param(p);
                    }

                    std::random_device rd;
                    gen_.seed(rd());
                }

            T operator ()() {
                auto p = dis_(gen_);
                if (unique) {
                    for (auto& a : vec_)    {
                        if (a == p) {
                            return this->operator()();
                        }
                    }
                }

                vec_.push_back(p);
                
                return vec_.back();
            }

            DISTRIBUTION dis_;
            std::mt19937 gen_;
            std::vector<T> vec_;
        };

    typedef genT<> int_gen_t;
}

#endif
