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

            uint32_t m_rows;
            uint32_t m_cols;
            board_t m_board;
    };

}
#endif
