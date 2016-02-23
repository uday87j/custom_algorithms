#include "kmeans.h"
#include <iostream>

using namespace std;

namespace ca    {

    void test_kmeans()  {

        gen_fpoint_t fgen(-100.0, 100.0);
        std::vector<fpoint_t> fpts(50);

        for (auto& p : fpts)    {
            p   = fgen();
            cout << p << "\n";
        }

        auto clusters = kmeans<fpoint_t, 5>(fpts);

        cout << endl << "Clusters:\n";

        for (auto& c : clusters)    {
            cout << c << "\t";
        }
        cout << endl;
    }
}


