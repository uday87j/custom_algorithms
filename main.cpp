#include "partition.h"
#include "sort.h"
#include "rank.h"
#include "erase.h"
//#include "closest_pair.h"
#include "kmeans.h"
#include "mode.h"

using namespace std;
using namespace ca;

int main()  {

#ifdef DONT_TEST_ME
    test_std_partition();
    test_range_partition();
    test_qsort();
    test_rank();
    test_erase();
    test_cp_methods();
    test_closest_pair();
    test_kmeans();
#endif
    test_mode();

    return 0;
}
