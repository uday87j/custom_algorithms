#include "nurikabe.h"

using namespace std;

// Unfortunate macros
#define SWEEP_BOARD for (auto itr = m_board.begin(); itr != m_board.end(); ++itr)

namespace ne {

    nurikabe::nurikabe(const std::uint32_t rows, const std::uint32_t cols):
        m_rows(rows),
        m_cols(cols),
        m_board(rows, cols) {
            //cout << "\nInitial";
            //draw_board();
        }

    void nurikabe::set_wall(uint32_t r, uint32_t c, uint32_t v) {
        m_board.cell(r, c, v);
    }

    void nurikabe::solve()  {

        // 1. Mark all neighbous of '1' as Black
        {
            SWEEP_BOARD {
                if (itr->id() == 1)   mark_1s_neigh((*itr));
            }
        }

        // 2. Mark a cell in-between 2 walls as Black
        {
            SWEEP_BOARD {
                mark_mid_cell((*itr));
            }
        }

        // 3. Mark unreachable cells as Black
        {
            SWEEP_BOARD {
                mark_unreachables((*itr));
            }
            SWEEP_BOARD {
                if (itr->colour() == 'G')   itr->colour('B');
            }
            SWEEP_BOARD {
                if (itr->colour() == 'R')   itr->colour('G');
            }
            //cout << "\nAfter transformation:\n";
            //draw_board();
        }

        // 4. Fill Black holes
        {
            SWEEP_BOARD {
                fill_black_hole(*itr);
            }
        }

        // Update regions
        m_board.update_regions();
        
        cout << "\nfinal";
        draw_board();
        SWEEP_BOARD     {
            auto r  = (*itr)->region();
            cout << r->region() << "\t" << r->size() << endl;
        }
    }

    void nurikabe::init(uint32_t rows, const uint32_t cols)  {
        m_rows = rows;
        m_cols = cols;
        m_board.init(rows, cols);
    }
    void nurikabe::reset()    {
        m_rows = 0;
        m_cols = 0;
        m_board.reset();
    }

    void nurikabe::draw_board() {
        m_board.draw();
    }

    void nurikabe::mark_1s_neigh(rcell_t* cell)   {
        auto c  = up(cell, m_board);
        if (c != nullptr && c != cell) c->colour('B');

        c  = down(cell, m_board);
        if (c != nullptr && c != cell) c->colour('B');

        c  = left(cell, m_board);
        if (c != nullptr && c != cell) c->colour('B');

        c  = right(cell, m_board);
        if (c != nullptr && c != cell) c->colour('B');
    }

    void nurikabe::mark_mid_cell(rcell_t* cell)   {
        if(m_board.is_wall(cell))   {
            auto* n  = up(cell, m_board);
            if (n != nullptr)   {
                auto* c  = up(n, m_board);
                if(c != nullptr && m_board.is_wall(c))  n->colour('B');
            }

            n  = down(cell, m_board);
            if (n != nullptr)   {
                auto* c  = down(n, m_board);
                if(c != nullptr && m_board.is_wall(c))  n->colour('B');
            }

            n  = left(cell, m_board);
            if (n != nullptr)   {
                auto* c  = left(n, m_board);
                if(c != nullptr && m_board.is_wall(c))  n->colour('B');
            }

            n  = right(cell, m_board);
            if (n != nullptr)   {
                auto* c  = right(n, m_board);
                if(c != nullptr && m_board.is_wall(c))  n->colour('B');

            }
        }
    }

    void nurikabe::reach_neigh(rcell_t* cell, size_t depth)   {
        if (cell != nullptr)    {
            if (depth == 1) reach_2s_neigh(cell);
            else    {
                reach_neigh(cell, depth-1);
                reach_neigh(up(cell, m_board), depth-1);
                reach_neigh(down(cell, m_board), depth-1);
                reach_neigh(left(cell, m_board), depth-1);
                reach_neigh(right(cell, m_board), depth-1);
            }
        }
    }

    void nurikabe::reach_2s_neigh(rcell_t* cell)    {
        if (cell != nullptr)    {
            auto* c = up(cell, m_board);
            if (c != nullptr && c->colour() == 'G') c->colour('R');
            c = down(cell, m_board);
            if (c != nullptr && c->colour() == 'G') c->colour('R');
            c = left(cell, m_board);
            if (c != nullptr && c->colour() == 'G') c->colour('R');
            c = right(cell, m_board);
            if (c != nullptr && c->colour() == 'G') c->colour('R');
            //draw_board();
            //cout << "\n2s done";
        }
    }

    void nurikabe::mark_unreachables(rcell_t* cell) {
        if (cell != nullptr && m_board.is_wall(cell))  {
            auto n  = cell->id();

            switch(n)   {
                case 2:
                    //cout << "\nStart 2";
                    reach_neigh(cell, 1);
                    //cout << "\n2s done";
                    //draw_board();
                    break;
                case 3:
                    //cout << "\nStart 3";
                    reach_neigh(cell, 2);
                    //cout << "\n3s done";
                    break;
                case 4:
                    //cout << "\nStart 4";
                    reach_neigh(cell, 3);
                    //cout << "\n4s done";
                    break;
                case 5:
                    break;
                default:
                    break
                        ;
            }
        }
    }

    void nurikabe::fill_black_hole(rcell_t* cell)   {
        if (cell != nullptr)    {
            if (!m_board.is_wall(cell)) {
                auto* u = up(cell, m_board);    
                auto* d = down(cell, m_board);
                auto* l = left(cell, m_board);
                auto* r = right(cell, m_board);

                if ((u == nullptr || u->colour() == 'B') &&
                        (d == nullptr || d->colour() == 'B') &&
                        (l == nullptr || l->colour() == 'B') &&
                        (r == nullptr || r->colour() == 'B')) {
                    assert(cell->colour() != 'W');
                    //cout << "\nHit a case: " << cell->colour() << endl;
                    if (cell->colour() == 'G')  cell->colour('B');
                    //cout << "\nNew: " << cell->colour() << endl;
                }
            }
        }
    }
}
