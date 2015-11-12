#include "nurikabe.h"

using namespace std;

namespace ne {

    nurikabe::nurikabe(const std::uint32_t rows, const std::uint32_t cols):
        m_rows(rows),
        m_cols(cols),
        m_board(rows, cols) {
            draw_board();
        }

    void nurikabe::set_wall(uint32_t r, uint32_t c, uint32_t v) {
        m_board.cell(r, c, v);
    }

    void nurikabe::solve()  {

        auto i  = 0;
        // 1. Mark all neighbous of '1' as Black
        for (auto itr = m_board.begin(); itr != m_board.end(); itr = m_board.next(), ++i)    {
            if (itr->id() == 1)   mark_1s_neigh(itr);
        }

        i  = 0;
        // 2. Mark a cell in-between 2 walls as Black
        for (auto itr = m_board.begin(); itr != m_board.end(); itr = m_board.next(), ++i)    {
            mark_mid_cell(itr);
        }

        i  = 0;
        //// 3. Mark unreachable cells as Black
        for (auto itr = m_board.begin(); itr != m_board.end(); itr = m_board.next(), ++i)    {
            mark_unreachables(itr);
        }
        cout << "\nLoop count: " << i << endl;
        
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
    }

    void nurikabe::reach_3s_neigh(icell_t* cell)    {
        reach_2s_neigh(left(cell, m_board));
        reach_2s_neigh(up(cell, m_board));
        reach_2s_neigh(down(cell, m_board));
        reach_2s_neigh(right(cell, m_board));
    }

    void nurikabe::reach_4s_neigh(icell_t* cell)    {
        reach_3s_neigh(left(cell, m_board));
        reach_3s_neigh(up(cell, m_board));
        reach_3s_neigh(down(cell, m_board));
        reach_3s_neigh(right(cell, m_board));
    }

    void nurikabe::mark_unreachables(icell_t* cell) {
        if (m_board.is_wall(cell))  {
            auto n  = cell->id();
            cout << n << endl;

            switch(n)   {
                case 2:
                    reach_2s_neigh(cell);
                    break;
                case 3:
                    reach_3s_neigh(cell);
                    break;
                case 4:
                    reach_4s_neigh(cell);
                    break;
                case 5:
                    break;
                default:
                    break
                        ;
            }
            draw_board();

            for (auto itr = m_board.begin(); itr != m_board.end(); itr = m_board.next())    {
                if (itr->colour() == 'G')   itr->colour('B');
                if (itr->colour() == 'R')   itr->colour('G');
            }

        }
    }
}
