#ifndef CA_STRUCT_SIZE_H_
#define CA_STRUCT_SIZE_H_

#include <iostream>
#include <map>

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


    //----------------Register & Deduction of types----------------------------
    
#define INT 0
    
    template <int id> struct id_to_type_t;
    template <>   struct id_to_type_t<INT> { typedef int this_type; };

    template <typename T> struct type_to_id_t;
    template <> struct type_to_id_t<int>    { static const int id = INT; };

    struct type_holder_t    {

    };

    // Illegal code:
    // template <typename T>
    //     struct atype_t  {
    //         typedef T this_type;

    //         template <typename R>
    //             void change_type(const R& r)    {
    //                 typedef R atype_t<T>::this_type;
    //             }
    //     };


    template <typename T>
        struct any_type_t   {
            typedef T this_type;
        };

    struct type_db_t    {
            type_db_t(const type_db_t&)  = delete;
            type_db_t& operator = (const type_db_t&)  = delete;

            static type_db_t& get_instance()   {
                static type_db_t db;
                return db;
            }

            template <typename T>
                int register_type() {
                    static std::map<int, any_type_t<T> > type_map_;
                    any_type_t<T> atype;
                    type_map_[type_count] = atype;
                    return type_count++;
                }

            
        private:
            type_db_t() {}
            static int type_count;

    };
    //----------------End Register & Deduction of types----------------------------
}

#endif
