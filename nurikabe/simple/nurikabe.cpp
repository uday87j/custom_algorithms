#include "nurikabe.h"

using namespace std;

// Unfortunate macros
#define CUR_BOARD   m_boards[m_cur_board]
#define SWEEP_BOARD for (auto itr = CUR_BOARD.begin(); itr != CUR_BOARD.end(); ++itr)

namespace ne {

    nurikabe::nurikabe(const std::uint32_t rows, const std::uint32_t cols):
        m_rows(rows),
        m_cols(cols),
        //m_board(rows, cols),
        m_boards(10),   // Decide a good number
        m_cur_board(0)  {
            CUR_BOARD.init(rows, cols);
            //cout << "\nInitial";
            //draw_board();
        }

    void nurikabe::set_wall(uint32_t r, uint32_t c, uint32_t v) {
        CUR_BOARD.cell(r, c, v);
    }

    nurikabe::game_state_t nurikabe::solve()  {

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

        // 4. Fill White holes
        {
            SWEEP_BOARD {
                fill_white_hole(*itr);
            }
        }

        // Update regions
        CUR_BOARD.update_regions();

        //Print regions
        // SWEEP_BOARD     {
        //     auto r  = (*itr)->region();
        //     cout << r->region() << "\t" << r->size() << endl;
        // }

        auto state = check_for_validity();
        cout << "\nGame state after apply fill rules: ";   print_state(state);
        assert(state == NO_ERROR_YET);

        if (!game_completed())  {
            auto new_wall = assume_and_build_wall();
            cout << "\nAfter the assumption: ";
            
            cout << "\nEnd of solve() iteration:";
            draw_board();

            if (new_wall)   {
                cout << "\nCalling solve() recursively: ";
                solve();
            }
            //draw_board();
        }

        //cout << "\nTest cloning board():";
        //++m_cur_board;
        //CUR_BOARD = m_boards[m_cur_board - 1];
        //assert(CUR_BOARD == m_boards[m_cur_board - 1]);   //TODO: Define board_t::==
        //draw_board();

        return state;
    }

    void nurikabe::init(uint32_t rows, const uint32_t cols)  {
        m_rows = rows;
        m_cols = cols;
        for (auto& b : m_boards)    {
            b.init();
        }
    }
    void nurikabe::reset()    {
        m_rows = 0;
        m_cols = 0;
        for (auto& b : m_boards)    {
            b.reset();
        }
    }

    board_t& nurikabe::cur_board()    {
        return m_boards[m_cur_board];
    }
    void nurikabe::draw_board() {
        CUR_BOARD.draw();
    }

    void nurikabe::print_state(game_state_t s)  {
        if (s == SOLVED)    cout << "SOLVED";
        if (s == POOL_EXISTS)    cout << "POOL_EXISTS";
        if (s == INCOMPLETE)    cout << "INCOMPLETE";
        if (s == INCOMPLETE_WALLS)    cout << "INCOMPLETE_WALLS";
        if (s == UNREACHABLE_WATER)    cout << "UNREACHABLE_WATER";
        if (s == OVERLAPPING_ISLANDS)    cout << "OVERLAPPING_ISLANDS";
        if (s == NO_ERROR_YET)    cout << "NO_ERROR_YET";
        if (s == UNKNOWN)    cout << "UNKNOWN";
        cout << endl;
    }

    void nurikabe::mark_1s_neigh(rcell_t* cell)   {
        auto c  = up(cell, CUR_BOARD);
        if (c != nullptr && c != cell) c->colour('B');

        c  = down(cell, CUR_BOARD);
        if (c != nullptr && c != cell) c->colour('B');

        c  = left(cell, CUR_BOARD);
        if (c != nullptr && c != cell) c->colour('B');

        c  = right(cell, CUR_BOARD);
        if (c != nullptr && c != cell) c->colour('B');
    }

    void nurikabe::mark_mid_cell(rcell_t* cell)   {
        if(CUR_BOARD.is_wall(cell))   {
            auto* n  = up(cell, CUR_BOARD);
            if (n != nullptr)   {
                auto* c  = up(n, CUR_BOARD);
                if(c != nullptr && CUR_BOARD.is_wall(c))  n->colour('B');
            }

            n  = down(cell, CUR_BOARD);
            if (n != nullptr)   {
                auto* c  = down(n, CUR_BOARD);
                if(c != nullptr && CUR_BOARD.is_wall(c))  n->colour('B');
            }

            n  = left(cell, CUR_BOARD);
            if (n != nullptr)   {
                auto* c  = left(n, CUR_BOARD);
                if(c != nullptr && CUR_BOARD.is_wall(c))  n->colour('B');
            }

            n  = right(cell, CUR_BOARD);
            if (n != nullptr)   {
                auto* c  = right(n, CUR_BOARD);
                if(c != nullptr && CUR_BOARD.is_wall(c))  n->colour('B');

            }
        }
    }

    void nurikabe::reach_neigh(rcell_t* cell, size_t depth)   {
        if ((cell != nullptr) && (cell->region()->region() != region_t::COMPLETE_WALL_REGION))    {
            if (depth == 1) reach_2s_neigh(cell);
            else    {
                reach_neigh(cell, depth-1);
                reach_neigh(up(cell, CUR_BOARD), depth-1);
                reach_neigh(down(cell, CUR_BOARD), depth-1);
                reach_neigh(left(cell, CUR_BOARD), depth-1);
                reach_neigh(right(cell, CUR_BOARD), depth-1);
            }
        }
    }

    void nurikabe::reach_2s_neigh(rcell_t* cell)    {
        if ((cell != nullptr) && (cell->region()->region() != region_t::COMPLETE_WALL_REGION))    {
            auto* c = up(cell, CUR_BOARD);
            if (c != nullptr && c->colour() == 'G') c->colour('R');
            c = down(cell, CUR_BOARD);
            if (c != nullptr && c->colour() == 'G') c->colour('R');
            c = left(cell, CUR_BOARD);
            if (c != nullptr && c->colour() == 'G') c->colour('R');
            c = right(cell, CUR_BOARD);
            if (c != nullptr && c->colour() == 'G') c->colour('R');
            //draw_board();
            //cout << "\n2s done";
        }
    }

    void nurikabe::mark_unreachables(rcell_t* cell) {
        // Mark cells reachable by numbered walls
        if ((cell != nullptr) && CUR_BOARD.is_wall(cell) &&
                (cell->region()->region() != region_t::COMPLETE_WALL_REGION))  {
            auto n  = cell->id();
            reach_neigh(cell, n-1);
        }

        // Remove cells that care adjacent to complete walls
        if ((cell != nullptr) && CUR_BOARD.is_wall(cell) &&
            (cell->region()->region() == region_t::COMPLETE_WALL_REGION))   {
                auto* u = up(cell, CUR_BOARD);    
                auto* d = down(cell, CUR_BOARD);
                auto* l = left(cell, CUR_BOARD);
                auto* r = right(cell, CUR_BOARD);

                if ((u != nullptr) && (u->colour() == 'R')) {
                    u->colour('G'); // Revert to unreachable
                }
                if ((d != nullptr) && (d->colour() == 'R')) {
                    d->colour('G'); // Revert to unreachable
                }
                if ((l != nullptr) && (l->colour() == 'R')) {
                    l->colour('G'); // Revert to unreachable
                }
                if ((r != nullptr) && (r->colour() == 'R')) {
                    r->colour('G'); // Revert to unreachable
                }
            }
    }

    void nurikabe::fill_black_hole(rcell_t* cell)   {
        if (cell != nullptr)    {
            if (!CUR_BOARD.is_wall(cell)) {
                auto* u = up(cell, CUR_BOARD);    
                auto* d = down(cell, CUR_BOARD);
                auto* l = left(cell, CUR_BOARD);
                auto* r = right(cell, CUR_BOARD);

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

    void nurikabe::fill_white_hole(rcell_t* cell)   {
        if (cell != nullptr)    {
            if (cell->region()->region() == region_t::INCOMPLETE_WALL_REGION)   {
                auto greys  = 0;
                rcell_t* gc = nullptr;

                auto u = up(cell, CUR_BOARD);
                if ((u != nullptr) && (u->colour() == 'G'))   {
                    ++greys  ;
                    gc = u;
                };

                auto d = down(cell, CUR_BOARD);
                if ((d != nullptr) && (d->colour() == 'G'))   {
                    ++greys  ;
                    gc = d;
                };

                auto r = right(cell, CUR_BOARD);
                if ((r != nullptr) && (r->colour() == 'G'))   {
                    ++greys  ;
                    gc = r;
                };

                auto l = left(cell, CUR_BOARD);
                if ((l != nullptr) && (l->colour() == 'G'))   {
                    ++greys  ;
                    gc = l;
                };

                if (1 == greys) {
                    gc->colour('W');                        
                }
            }
        }
    }

    bool nurikabe::assume_and_build_wall()  {
        auto i = 0;
        bool built_new_wall = false;
        // Let us strat with '2'
        SWEEP_BOARD {
            assert(*itr != nullptr);
            //TODO: Add assumptions for all numbers 3, 4, 5 etc
            if (((*itr)->id() == 2) && ((*itr)->region()->region() == region_t::INCOMPLETE_WALL_REGION))  {
                auto u  = up(*itr, CUR_BOARD);
                //TODO: Add down, L & R
                if ((u != nullptr) && (u->colour() == 'G')) {
                    
                    // Clone our current board
                    m_boards[m_cur_board + 1]   = m_boards[m_cur_board];
                    ++m_cur_board;
                    
                    auto r  = u->row();
                    auto c  = u->col();

                    cout << "\nWill assume & set this cell to W: (" << r << ", " << c << ")\n";
                    CUR_BOARD.cell(r, c)->colour('W'); // Assume this cell to be our Wall
                    CUR_BOARD.update_region(CUR_BOARD.cell(r, c));
                    auto new_cell   = CUR_BOARD.cell(r, c);
                    assert(new_cell->region()->region() == region_t::COMPLETE_WALL_REGION);
                    assert(down(new_cell, CUR_BOARD)->region()->region() == region_t::COMPLETE_WALL_REGION);
                    assert(new_cell->region() == down(new_cell, CUR_BOARD)->region());
                    
                    auto result_state   = check_for_validity();
                    if (NO_ERROR_YET != result_state)  {
                        cout << "Undoing the move becuase :";
                        print_state(result_state);
                        u->colour('G');
                        //CUR_BOARD.reset();
                        --m_cur_board;  //Backtrack
                    }
                    else {
                        cout << "Valid move becuase :";
                        print_state(result_state);
                        built_new_wall  = true;
                        //draw_board();
                        break;
                    }
                }
            }
            ++i;
        }
        return built_new_wall;
    }

    nurikabe::game_state_t nurikabe::game_solved() {

        if (!game_completed())  {
            return INCOMPLETE;                
        }

        return check_for_validity();
    }
    nurikabe::game_state_t nurikabe::check_for_validity() {
        if (does_pool_exist())  {
            return POOL_EXISTS;
        }

        if (any_overlapping_walls())    {
            return OVERLAPPING_ISLANDS;
        }

        if (any_unreachable_water())    {
            return UNREACHABLE_WATER;
        }

        return NO_ERROR_YET;
    }

    bool nurikabe::does_pool_exist() {
        auto regs   = CUR_BOARD.regions();
        for (auto reg : regs) {
            if ((reg->region() == region_t::WATER_REGION) && (reg->size() >= 4))    {
                auto cells = reg->cells();
                //for (auto c : cells)    {
                //    cout << c->colour() << "\t" << endl;
                //}
                for (auto c : cells)    {
                    assert(c->colour() == 'B');

                    auto r  = right(c, CUR_BOARD);
                    if ((r == nullptr) || (reg != r->region()))   break;

                    assert(r->colour() == 'B');
                    auto d  = down(c, CUR_BOARD);

                    if ((d == nullptr) || (reg != d->region()))   break;
                    assert(d->colour() == 'B');
                    
                    auto rd = down(r, CUR_BOARD);
                    if ((rd != nullptr) && (reg == rd->region())) {
                        assert(rd->colour() == 'B');
                        return true;
                    }
                }
            }
        }
        return false;
    }

    bool nurikabe::any_overlapping_walls()  {
        SWEEP_BOARD     {
            if (itr->colour() == 'W')   {
                auto r  = right(*itr, CUR_BOARD);
                //if (r != nullptr && r->colour() == 'W') return true;
                if (r != nullptr && CUR_BOARD.is_wall(r) && (r->region() != r->region()))   {
                    return true;    // 2 different regions are wallls & adjacent                    
                }

                auto d  = down(*itr, CUR_BOARD);
                //if (d != nullptr && d->colour() == 'W') return true;
                if (d != nullptr && CUR_BOARD.is_wall(d) && (itr->region() != d->region()))   {
                    return true;    // 2 different regions are wallls & adjacent                    
                }
            }
        }
        return false;
    }

    bool nurikabe::game_completed() {
        SWEEP_BOARD     {
            if (itr->colour() == 'G')   {
                return false;
            }
        }
        return true;
    }

    bool nurikabe::any_unreachable_water()  {
        SWEEP_BOARD     {
            auto u  = up(*itr, CUR_BOARD);
            auto d  = down(*itr, CUR_BOARD);
            auto r  = right(*itr, CUR_BOARD);
            auto l  = left(*itr, CUR_BOARD);

            if (((u != nullptr) && (u->colour() == 'W')) &&
                        ((d != nullptr) && (d->colour() == 'W')) &&
                        ((l != nullptr) && (l->colour() == 'W')) &&
                        ((r != nullptr) && (r->colour() == 'W')))   {
                return true;
            }
        }
        return false;

    }
}
