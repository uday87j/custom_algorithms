#include "partition.h"
#include "sort.h"
#include "rank.h"
#include "erase.h"

using namespace std;
using namespace ca;

int main()  {

#ifdef DONT_TEST_ME
    test_std_partition();
    test_range_partition();
    test_qsort();
    test_rank();
#endif
    test_erase();

    return 0;
}
