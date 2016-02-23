#include "mode.h"

using namespace std;

namespace ca {

    void test_mode()    {

        int_gen_t igen(-2, 2);
        std::vector<int> ivec(10, 0);
        std::for_each(begin(ivec), end(ivec),
                [&](int& i)  {
                    i = igen();
                });
        print_container<decltype(begin(ivec)), int>(begin(ivec), end(ivec));

        auto mode = find_mode<decltype(begin(ivec)), int>(begin(ivec), end(ivec));
        cout << "\nMode of this array: " << mode << endl;
    }

}
