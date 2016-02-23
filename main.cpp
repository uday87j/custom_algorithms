#include "partition.h"
#include "sort.h"
#include "rank.h"
#include "erase.h"
//#include "closest_pair.h"
#include "kmeans.h"
#include "mode.h"
#include "struct_size.h"

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
    test_mode();
#endif
    test_struct_size();

    return 0;
}
