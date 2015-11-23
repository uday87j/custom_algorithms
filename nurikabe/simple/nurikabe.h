#ifndef NURIKABE_H_
#define NURIKABE_H_

#include "board.h"

#include <cstddef>

using std::uint32_t;

namespace ne    {

    class nurikabe;

    struct wall_info_t  {
        wall_info_t(uint32_t r = -1, uint32_t c = -1, char dir = ' ', bool built = false);
        uint32_t row;
        uint32_t col;
        char direction;
        bool build_status;
    };

    class wall_builder_t    {
        public:
            wall_builder_t(nurikabe* user = nullptr, uint32_t highest_wall_id = 0);
            wall_builder_t(const wall_builder_t&)   = delete;
            wall_builder_t& operator = (const wall_builder_t&)  = delete;

            void build_next_wall(bool new_wall = true); //If new_wall == false, this wil rebuild previosuly built wall
            void unbuild_latest_wall();
        private:
            rcell_t* build_single_wall(const uint32_t wall_id, const char direction);
            char next_direction(const char);
            nurikabe* game_;
            std::vector<wall_info_t> walls_built_;
            uint32_t highest_wall_id_;
    };

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
            game_state_t run_fill_rules();

            void init(std::uint32_t rows = 0, const std::uint32_t cols = 0);
            void reset();

        private:
            friend class wall_builder_t;

            void draw_board();

            board_t& cur_board();

            // House keeper
            wall_builder_t wb_;

            // Step-wise solvers
            void mark_1s_neigh(rcell_t*);
            void mark_mid_cell(rcell_t*);
            void mark_unreachables(rcell_t*);
            void fill_black_hole(rcell_t*);// A non-wall is surrounded by 'B'
            void fill_white_hole(rcell_t*);// An incomplete wall will expand if there is only one possible way
            bool build_a_wall(uint32_t id, const char direction);
            bool assume_and_build_wall(char& wall_direction);
            

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
            game_state_t solve_old();   //Remove this later

            uint32_t m_rows;
            uint32_t m_cols;
            std::vector<board_t> m_boards;
            uint32_t m_cur_board;

            // Tracking to solve steps
            uint32_t smallest_incomplete_wall_id_;
            std::vector<std::pair<std::pair<uint32_t, uint32_t>, char> > walls_built_;
    };

}
#endif
