#include "nurikabe.h"

#include <iostream>

using namespace ne;
using namespace std;

void puzzle_1(nurikabe& nk) {
    nk.set_wall(1,1,4);
    nk.set_wall(2,4,2);
    nk.set_wall(4,1,2);
    nk.set_wall(4,4,1);
}

void puzzle_2(nurikabe& nk) {
    nk.set_wall(1,1,4);
    nk.set_wall(1,3,4);
    nk.set_wall(3,1,4);
    nk.set_wall(3,3,4);
}

int main()  {

    {
        cout << "\nPuzzle 1\n";
       
        nurikabe solver(5, 5);

        puzzle_1(solver);

        solver.solve();

        solver.reset();
    }

    {
        cout << "\nPuzzle 2\n";

        nurikabe solver(5, 5);
        //solver.init(5, 5);

        puzzle_2(solver);
        
        solver.solve();
        
        solver.reset();
    }

    return 0;
}

