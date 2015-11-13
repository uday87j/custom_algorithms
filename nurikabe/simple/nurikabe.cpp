#include "nurikabe.h"

using namespace std;

namespace ne {

    nurikabe::nurikabe(const std::uint32_t rows, const std::uint32_t cols):
        m_rows(rows),
        m_cols(cols),
        m_board(rows, cols) {
            cout << "\nInitial";
            draw_board();
        }

    void nurikabe::set_wall(uint32_t r, uint32_t c, uint32_t v) {
        m_board.cell(r, c, v);
    }

    void nurikabe::solve()  {

        // 1. Mark all neighbous of '1' as Black
        for (auto itr = m_board.begin(); itr != m_board.end(); /*itr = m_board.next()*/++itr)    {
            if (itr->id() == 1)   mark_1s_neigh((*itr));
        }

        // 2. Mark a cell in-between 2 walls as Black
        for (auto itr = m_board.begin(); itr != m_board.end(); ++itr)    {
            mark_mid_cell((*itr));
        }

        // 3. Mark unreachable cells as Black
        {
            for (auto itr = m_board.begin(); itr != m_board.end(); ++itr)    {
                mark_unreachables((*itr));
            }
            for (auto itr = m_board.begin(); itr != m_board.end(); ++itr)    {
                if (itr->colour() == 'G')   itr->colour('B');
            }
            for (auto itr = m_board.begin(); itr != m_board.end(); ++itr)    {
                if (itr->colour() == 'R')   itr->colour('G');
            }
            cout << "\nAfter transformation:\n";
            draw_board();
        }
        
        cout << "\nfinal";
        draw_board();
    }

    void nurikabe::draw_board() {
        m_board.draw();
    }

    void nurikabe::mark_1s_neigh(icell_t* cell)   {
        auto c  = up(cell, m_board);
        if (c != cell) c->colour('B');

        c  = down(cell, m_board);
        if (c != cell) c->colour('B');

        c  = left(cell, m_board);
        if (c != cell) c->colour('B');

        c  = right(cell, m_board);
        if (c != cell) c->colour('B');
    }

    void nurikabe::mark_mid_cell(icell_t* cell)   {
        if(m_board.is_wall(cell))   {
            auto* n  = up(cell, m_board);
            auto* c  = up(n, m_board);
            if(m_board.is_wall(c))  n->colour('B');
        }
    }

    void nurikabe::reach_2s_neigh(icell_t* cell)    {
        auto* c = up(cell, m_board);
        if (c->colour() == 'G') c->colour('R');
        c = down(cell, m_board);
        if (c->colour() == 'G') c->colour('R');
        c = left(cell, m_board);
        if (c->colour() == 'G') c->colour('R');
        c = right(cell, m_board);
        if (c->colour() == 'G') c->colour('R');
        draw_board();
        cout << "\n2s done";
    }

    void nurikabe::reach_3s_neigh(icell_t* cell)    {
        reach_2s_neigh(cell);
        reach_2s_neigh(left(cell, m_board));
        reach_2s_neigh(up(cell, m_board));
        reach_2s_neigh(down(cell, m_board));
        reach_2s_neigh(right(cell, m_board));
    }

    void nurikabe::reach_4s_neigh(icell_t* cell)    {
        reach_3s_neigh(cell);
        reach_3s_neigh(left(cell, m_board));
        reach_3s_neigh(up(cell, m_board));
        reach_3s_neigh(down(cell, m_board));
        reach_3s_neigh(right(cell, m_board));
    }

    void nurikabe::mark_unreachables(icell_t* cell) {
        if (m_board.is_wall(cell))  {
            auto n  = cell->id();

            switch(n)   {
                case 2:
                    cout << "\nStart 2";
                    reach_2s_neigh(cell);
                    cout << "\n2s done";
                    //draw_board();
                    break;
                case 3:
                    cout << "\nStart 3";
                    reach_3s_neigh(cell);
                    cout << "\n3s done";
                    break;
                case 4:
                    cout << "\nStart 4";
                    reach_4s_neigh(cell);
                    cout << "\n4s done";
                    break;
                case 5:
                    break;
                default:
                    break
                        ;
            }
        }
    }
}
