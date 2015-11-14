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

            void init(std::uint32_t rows = 0, const std::uint32_t cols = 0);
            void reset();

        private:
            void draw_board();

            // Step-wise solvers
            void mark_1s_neigh(icell_t*);
            void mark_mid_cell(icell_t*);
            void mark_unreachables(icell_t*);
            void fill_black_hole(icell_t*);// A non-wall is surrounded by 'B'
            void fill_white_hole(icell_t*);// An incomplete wall will expand if there is only one possible way
            
            void reach_neigh(icell_t* cell, size_t depth = 1);
            void reach_2s_neigh(icell_t* cell);

            uint32_t m_rows;
            uint32_t m_cols;
            board_t m_board;
    };

}
#endif
