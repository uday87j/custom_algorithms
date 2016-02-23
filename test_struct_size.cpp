#include "struct_size.h"
#include <iostream>

using namespace std;

namespace ca  {

    std::ostream& operator << (std::ostream& os, const ca_int_t& ci)    {
        return ci.print(os);            
    }

    void test_struct_size() {

        int local_int   = 32;
        cout << "\nSize of local_int: " << sizeof(local_int) << endl;
        cout << "\nlcoal_int: " << local_int << endl;

        ca::ca_int_t ca_int;
        ca_int.ivar = 32;
        cout << "\nSize of ca_int: " << sizeof(ca_int) << endl;
        cout << "\nca_int: " << ca_int << endl;
    }
}
