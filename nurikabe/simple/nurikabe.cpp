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
        m_board.update_regions();

        //Print regions
        // SWEEP_BOARD     {
        //     auto r  = (*itr)->region();
        //     cout << r->region() << "\t" << r->size() << endl;
        // }

        auto state = check_for_validity();
        cout << "\nGame state: ";   print_state(state);
        
        cout << "\nEnd of solve():";
        draw_board();

        return state;
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
        if ((cell != nullptr) && (cell->region()->region() != region_t::COMPLETE_WALL_REGION))    {
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
        if ((cell != nullptr) && (cell->region()->region() != region_t::COMPLETE_WALL_REGION))    {
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
        if ((cell != nullptr) && m_board.is_wall(cell) &&
                (cell->region()->region() != region_t::COMPLETE_WALL_REGION))  {
            auto n  = cell->id();
            reach_neigh(cell, n-1);
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

    void nurikabe::fill_white_hole(rcell_t* cell)   {
        if (cell != nullptr)    {
            if (cell->region()->region() == region_t::INCOMPLETE_WALL_REGION)   {
                auto greys  = 0;
                rcell_t* gc = nullptr;

                auto u = up(cell, m_board);
                if ((u != nullptr) && (u->colour() == 'G'))   {
                    ++greys  ;
                    gc = u;
                };

                auto d = down(cell, m_board);
                if ((d != nullptr) && (d->colour() == 'G'))   {
                    ++greys  ;
                    gc = d;
                };

                auto r = right(cell, m_board);
                if ((r != nullptr) && (r->colour() == 'G'))   {
                    ++greys  ;
                    gc = r;
                };

                auto l = left(cell, m_board);
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

    void nurikabe::assume_and_build_wall()  {
        // Let us strat with '2'
        SWEEP_BOARD {
            if (((*itr)->id() == 2) && ((*itr)->region()->region() == region_t::INCOMPLETE_WALLS))  {
                auto u  = up(*itr, m_board);
                if ((u != nullptr) && (u->colour() == 'G')) {
                    u->colour('W'); // Assume this cell to be our Wall
                    if (ANY_ERROR == check_for_validity())  {
                        u->colour('G');
                        //TODO: Backtrack!
                    }
                }
            }
        }
    }

    nurikabe::game_state_t nurikabe::check_for_validity() {
        if (does_pool_exist())  {
            return POOL_EXISTS;
        }

        if (any_overlapping_walls())    {
            return OVERLAPPING_ISLANDS;
        }

        if (!game_completed())  {
            return INCOMPLETE;                
        }

        return NO_ERROR_YET;
    }

    bool nurikabe::does_pool_exist() {
        auto regs   = m_board.regions();
        for (auto reg : regs) {
            if ((reg->region() == region_t::WATER_REGION) && (reg->size() >= 4))    {
                auto cells = reg->cells();
                //for (auto c : cells)    {
                //    cout << c->colour() << "\t" << endl;
                //}
                for (auto c : cells)    {
                    assert(c->colour() == 'B');

                    auto r  = right(c, m_board);
                    if ((r == nullptr) || (reg != r->region()))   break;

                    assert(r->colour() == 'B');
                    auto d  = down(c, m_board);

                    if ((d == nullptr) || (reg != d->region()))   break;
                    assert(d->colour() == 'B');
                    
                    auto rd = down(r, m_board);
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
                auto r  = right(*itr, m_board);
                if (r->colour() == 'W') return true;

                auto d  = down(*itr, m_board);
                if (d->colour() == 'W') return true;
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
}
