#include "partition.h"
#include "sort.h"
#include "rank.h"

using namespace std;
using namespace ca;

int main()  {

#ifdef DONT_TEST_ME
    test_std_partition();
    test_range_partition();
    test_qsort();
#endif
    test_rank();

    return 0;
}
