#ifndef CA_STRUCT_SIZE_H_
#define CA_STRUCT_SIZE_H_

#include <iostream>

namespace ca {

    extern void test_struct_size();

    struct ca_int_t {
        int ivar;

        std::ostream& print(std::ostream& os = std::cout) const {
            os << ivar;
            return os;
        }   
    };

    extern std::ostream& operator << (std::ostream& os, const ca_int_t& ci);
}

#endif
