#include "partition.h"
#include "sort.h"
#include "rank.h"
#include "erase.h"
#include "closest_pair.h"

using namespace std;
using namespace ca;

int main()  {

#ifdef DONT_TEST_ME
    test_std_partition();
    test_range_partition();
    test_qsort();
    test_rank();
    test_erase();
#endif
    test_closest_pair();

    return 0;
}
