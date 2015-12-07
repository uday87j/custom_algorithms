// This hack doesn't work
// #include <initializer_list>
// #undef _GLIBXX_HAVE_GETS

#include "nurikabe.h"
#include <chrono>

using std::chrono::system_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;

using namespace ne;
using namespace std;

void puzzle_1(nurikabe& nk) {
    nk.set_wall(1,1,4);
    nk.set_wall(2,4,2);
    nk.set_wall(4,1,2);
    nk.set_wall(4,4,1);
}

void puzzle_3(nurikabe& nk) {
    nk.set_wall(0,1,3);
    nk.set_wall(0,3,1);
    nk.set_wall(3,0,2);
    nk.set_wall(4,2,4);
    nk.set_wall(4,4,1);
}

void puzzle_2(nurikabe& nk) {
    nk.set_wall(1,1,4);
    nk.set_wall(1,3,4);
    nk.set_wall(3,1,4);
    nk.set_wall(3,3,4);
}

void puzzle_4(nurikabe& nk) {
    nk.set_wall(0,2,2);
    nk.set_wall(2,0,1);
    nk.set_wall(2,4,3);
    nk.set_wall(4,2,4);
}

void puzzle_5(nurikabe& nk) {
    nk.set_wall(0,3,3);
    nk.set_wall(1,0,2);
    nk.set_wall(3,4,4);
    nk.set_wall(4,1,1);
}

void puzzle_6(nurikabe& nk) {
    nk.set_wall(0,0,1);
    nk.set_wall(1,4,1);
    nk.set_wall(2,1,5);
    nk.set_wall(4,4,3);
}

void puzzle_7(nurikabe& nk) {
    nk.set_wall(0,1,2);
    nk.set_wall(0,5,2);
    nk.set_wall(2,3,2);
    nk.set_wall(3,0,2);
    nk.set_wall(4,5,2);
    nk.set_wall(5,0,2);
    nk.set_wall(5,3,2);
}

void puzzle_8(nurikabe& nk) {
    nk.set_wall(0,0,2);
    nk.set_wall(2,0,1);
    nk.set_wall(3,2,6);
}

void puzzle_9(nurikabe& nk) {
    nk.set_wall(0,0,3);
    nk.set_wall(0,2,1);
    nk.set_wall(4,2,4);
    nk.set_wall(4,4,5);
}

void puzzle_10(nurikabe& nk) {
    nk.set_wall(0,3,3);
    nk.set_wall(2,1,1);
    nk.set_wall(2,3,6);
    nk.set_wall(4,1,3);
}

void puzzle_11(nurikabe& nk) {
    nk.set_wall(0,2,2);
    nk.set_wall(1,4,1);
    nk.set_wall(2,0,4);
    nk.set_wall(3,5,3);
    nk.set_wall(4,1,2);
    nk.set_wall(5,3,1);
}

void puzzle_12(nurikabe& nk) {
    nk.set_wall(0,0,2);
    nk.set_wall(0,9,2);
    nk.set_wall(1,6,2);
    nk.set_wall(2,1,2);
    nk.set_wall(2,4,7);
    nk.set_wall(4,6,3);
    nk.set_wall(4,8,3);
    nk.set_wall(5,2,2);
    nk.set_wall(5,7,3);
    nk.set_wall(6,0,2);
    nk.set_wall(6,3,4);
    nk.set_wall(8,1,1);
    nk.set_wall(8,6,2);
    nk.set_wall(8,8,4);
    
}


int main()  {

#ifdef DONT_RUN_ME
#endif
    {
        auto start  = system_clock::now();
        cout << "\nPuzzle 1\n";
       
        nurikabe solver(5, 5);

        puzzle_1(solver);

        solver.solve();
        auto finish = system_clock::now();
        cout << "\nTime taken to solve: " << duration_cast<milliseconds>(finish - start).count() << " ms\n";

        solver.reset();
    }

    {
        auto start  = system_clock::now();
        cout << "\nPuzzle 2\n";

        nurikabe solver(5, 5);

        puzzle_2(solver);
        
        solver.solve();
        auto finish = system_clock::now();
        cout << "\nTime taken to solve: " << duration_cast<milliseconds>(finish - start).count() << " ms\n";
        
        solver.reset();
    }
    
    {
        auto start  = system_clock::now();
        cout << "\nPuzzle 3\n";

        nurikabe solver(5, 5);

        puzzle_3(solver);
        
        solver.solve();
        auto finish = system_clock::now();
        cout << "\nTime taken to solve: " << duration_cast<milliseconds>(finish - start).count() << " ms\n";
        
        solver.reset();
    }

    {
        auto start  = system_clock::now();
        cout << "\nPuzzle 4\n";

        nurikabe solver(5, 5);

        puzzle_4(solver);
        
        solver.solve();
        auto finish = system_clock::now();
        cout << "\nTime taken to solve: " << duration_cast<milliseconds>(finish - start).count() << " ms\n";
        
        solver.reset();
    }

    {
        auto start  = system_clock::now();
        cout << "\nPuzzle 5\n";

        nurikabe solver(5, 5);

        puzzle_5(solver);
        
        solver.solve();
        auto finish = system_clock::now();
        cout << "\nTime taken to solve: " << duration_cast<milliseconds>(finish - start).count() << " ms\n";
        
        solver.reset();
    }

    {
        auto start  = system_clock::now();
        cout << "\nPuzzle 6\n";

        nurikabe solver(5, 5);

        puzzle_6(solver);
        
        solver.solve();
        auto finish = system_clock::now();
        cout << "\nTime taken to solve: " << duration_cast<milliseconds>(finish - start).count() << " ms\n";
        
        solver.reset();
    }

    {
        auto start  = system_clock::now();
        cout << "\nPuzzle 8\n";

        nurikabe solver(5, 5);

        puzzle_8(solver);
        
        solver.solve();
        auto finish = system_clock::now();
        cout << "\nTime taken to solve: " << duration_cast<milliseconds>(finish - start).count() << " ms\n";
        
        solver.reset();
    }

    {
        auto start  = system_clock::now();
        cout << "\nPuzzle 9\n";

        nurikabe solver(5, 5);

        puzzle_9(solver);
        
        solver.solve();
        auto finish = system_clock::now();
        cout << "\nTime taken to solve: " << duration_cast<milliseconds>(finish - start).count() << " ms\n";
        
        solver.reset();
    }

    {
        auto start  = system_clock::now();
        cout << "\nPuzzle 10\n";

        nurikabe solver(5, 5);

        puzzle_10(solver);
        
        solver.solve();
        auto finish = system_clock::now();
        cout << "\nTime taken to solve: " << duration_cast<milliseconds>(finish - start).count() << " ms\n";
        
        solver.reset();
    }

    {
        auto start  = system_clock::now();
        cout << "\nPuzzle 11\n";

        nurikabe solver(6, 6);

        puzzle_11(solver);
        
        solver.solve();
        auto finish = system_clock::now();
        cout << "\nTime taken to solve: " << duration_cast<milliseconds>(finish - start).count() << " ms\n";
        
        solver.reset();
    }

#ifdef DONT_RUN_ME
#endif

    {
        auto start  = system_clock::now();
        cout << "\nPuzzle 7\n";

        nurikabe solver(6, 6);

        puzzle_7(solver);
        
        solver.solve();
        auto finish = system_clock::now();
        cout << "\nTime taken to solve: " << duration_cast<milliseconds>(finish - start).count() << " ms\n";
        
        solver.reset();
    }
    
    {
        auto start  = system_clock::now();
        cout << "\nPuzzle 12\n";

        nurikabe solver(9, 10);

        puzzle_12(solver);
        
        solver.solve();
        auto finish = system_clock::now();
        cout << "\nTime taken to solve: " << duration_cast<milliseconds>(finish - start).count() << " ms\n";
        
        solver.reset();
    }

    return 0;
}
//TODO: Step down wall_id multiple times when there was no wall built for that id
