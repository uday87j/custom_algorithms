#include "nurikabe.h"

using namespace ne;

void set_up_walls(nurikabe& nk) {
    nk.set_wall(1,1,4);
    nk.set_wall(2,4,2);
    nk.set_wall(4,1,2);
    nk.set_wall(4,4,1);
}

int main()  {

    nurikabe solver(5, 5);

    set_up_walls(solver);

    solver.solve();

    return 0;
}

