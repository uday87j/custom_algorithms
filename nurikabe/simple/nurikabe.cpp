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

        // 1. Mark all neighbous of '1' as Black
        for (auto itr = m_board.begin(); itr != m_board.end(); itr = m_board.next())    {
            //cout << itr->id() << endl;
            if ((*itr).id() == 1)   mark_1s_neigh(itr);
        }
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
}
