#include "erase.h"

using namespace ca;
using namespace std;

namespace ca    {

    void test_erase()   {

        std::vector<int> ivec   = { {1, 2, 3, 4, 5, 6, 7, 8, 9, 10} };
        print_container(ivec);  cout << endl;
        ca::erase(ivec, 8);
        print_container(ivec);  cout << endl;

        std::map<int, string> ismap;
        ismap[1]    = "one";
        ismap[2]    = "two";
        ismap[3]    = "three";
        print_container(ismap);  cout << endl;
        ca::erase(ismap, 2);
        print_container(ismap);  cout << endl;
    }
}
