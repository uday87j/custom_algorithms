#include "struct_size.h"
#include <iostream>
#include <typeinfo>

using namespace std;

namespace ca  {

    std::ostream& operator << (std::ostream& os, const ca_int_t& ci)    {
        return ci.print(os);            
    }

    int type_db_t::type_count = 0;

    void test_struct_size() {

        // Test int v/s int inside struct
        int local_int   = 32;
        cout << "\nSize of local_int: " << sizeof(local_int) << endl;
        cout << "\nlcoal_int: " << local_int << endl;

        ca::ca_int_t ca_int;
        ca_int.ivar = 32;
        cout << "\nSize of ca_int: " << sizeof(ca_int) << endl;
        cout << "\nca_int: " << ca_int << endl;
        //---------------------------------
        
        // Test type_db_t
        auto& db = type_db_t::get_instance();
        auto int_id = db.register_type<int>();
        //auto int_type_wrap = db.recover_type(int_id);
        //cout << "\nType in int_type_wrap: " << typeid(int_type_wrap::this_type).name() << endl;
        const auto iid = type_to_id_t<int>::id;
        //auto itype = id_to_type_t<iid>::this_type;
        cout << "\nType in int_type_wrap: " << typeid(id_to_type_t<iid>::this_type).name() << endl;
        // --------------------------------
    }
}
