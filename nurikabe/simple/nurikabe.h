#ifndef NURIKABE_H_
#define NURIKABE_H_

#include "board.h"

#include <cstddef>

using std::uint32_t;

namespace ne    {

    class nurikabe  {

        public:
            enum game_state_t   {
                SOLVED  = 0,
                POOL_EXISTS,
                INCOMPLETE,
                INCOMPLETE_WALLS,
                UNREACHABLE_WATER,
                OVERLAPPING_ISLANDS,
                NO_ERROR_YET,
                CANT_PROCEED,
                UNKNOWN
            };

            nurikabe(const std::uint32_t rows = 0, const std::uint32_t cols = 0);

            void set_wall(uint32_t r, uint32_t c, uint32_t v);
            game_state_t solve();

            void init(std::uint32_t rows = 0, const std::uint32_t cols = 0);
            void reset();

        private:

            void draw_board();

            board_t& cur_board();

            // Step-wise solvers
            void mark_1s_neigh(rcell_t*);
            void mark_mid_cell(rcell_t*);
            void mark_unreachables(rcell_t*);
            void fill_black_hole(rcell_t*);// A non-wall is surrounded by 'B'
            void fill_white_hole(rcell_t*);// An incomplete wall will expand if there is only one possible way
            bool assume_and_build_wall();
            

            // Helper functions for accessiblity
            void reach_neigh(rcell_t* cell, size_t depth = 1);
            void reach_2s_neigh(rcell_t* cell);

            game_state_t check_for_validity();
            game_state_t game_solved();

            // Helper checkers
            bool game_completed();
            bool does_pool_exist();
            bool any_overlapping_walls();
            bool any_unreachable_water();

            void print_state(game_state_t);

            uint32_t m_rows;
            uint32_t m_cols;
            std::vector<board_t> m_boards;
            uint32_t m_cur_board;
    };

}
#endif
