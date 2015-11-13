#ifndef NURIKABE_H_
#define NURIKABE_H_

#include "board.h"

#include <cstddef>

using std::uint32_t;

namespace ne    {

    class nurikabe  {

        public:
            nurikabe(const std::uint32_t rows = 0, const std::uint32_t cols = 0);

            void set_wall(uint32_t r, uint32_t c, uint32_t v);
            void solve();

        private:
            void draw_board();

            void mark_1s_neigh(icell_t* cell);
            void mark_mid_cell(icell_t* cell);
            void mark_unreachables(icell_t* cell);
            void reach_2s_neigh(icell_t* cell);
            void reach_3s_neigh(icell_t* cell);
            void reach_4s_neigh(icell_t* cell);

            template<const size_t n>
                void reach_neigh(icell_t* cell) {
                    reach_neigh<n - 1>(left(cell, m_board));
                    reach_neigh<n - 1>(up(cell, m_board));
                    reach_neigh<n - 1>(down(cell, m_board));
                    reach_neigh<n - 1>(right(cell, m_board));
                }

            template<>
                void reach_neigh<0>(icell_t* cell)  {
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

            uint32_t m_rows;
            uint32_t m_cols;
            board_t m_board;
    };

}
#endif
