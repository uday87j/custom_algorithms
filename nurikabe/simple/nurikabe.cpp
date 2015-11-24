#include "nurikabe.h"

#include <limits>

using namespace std;

// Unfortunate macros
#define CUR_BOARD   m_boards[m_cur_board]
#define CUR_USER_BOARD   game_->m_boards[game_->m_cur_board]
#define SWEEP_BOARD for (auto itr = CUR_BOARD.begin(); itr != CUR_BOARD.end(); ++itr)
#define SWEEP_USER_BOARD for (auto itr = CUR_USER_BOARD.begin(); itr != CUR_USER_BOARD.end(); ++itr)

namespace ne {

    wall_info_t::wall_info_t(uint32_t r, uint32_t c, char dir, bool built)
        :row(r),
        col(c),
        direction(dir),
        build_status(built) {}

    wall_builder_t::wall_builder_t(nurikabe* user_, uint32_t highest_wall_id)
        : game_(user_),
        highest_wall_id_(highest_wall_id)   {}

    rcell_t* wall_builder_t::build_single_wall(const uint32_t wall_id, const char direction)   {

        auto f  = [this](rcell_t* rc, const char c)    {
            if (c == 'U')   {
                return up(rc, CUR_USER_BOARD);
            }
            else if (c == 'D')  {
                return down(rc, CUR_USER_BOARD);
            }
            else if (c == 'R')  {
                return right(rc, CUR_USER_BOARD);
            }
            else if (c == 'L')  {
                return left(rc, CUR_USER_BOARD);
            }
            else    {
                //assert(0);
                return rc;
            }
        };

        SWEEP_USER_BOARD {
            assert(*itr != nullptr);
            if (((*itr)->id() == wall_id) && ((*itr)->region()->region() == region_t::INCOMPLETE_WALL_REGION))  {
                auto u  = f(*itr, direction);
                //if ((u != nullptr) && (u->colour() == 'G')) {
                if ((u != nullptr)) {
                    
                    // Clone our current board
                    game_->m_boards[game_->m_cur_board + 1]   = game_->m_boards[game_->m_cur_board];
                    ++game_->m_cur_board;
                    
                    auto r  = u->row();
                    auto c  = u->col();

                    auto cell   = CUR_USER_BOARD.cell(r, c);

                    auto dir    = direction;
                    while(dir != ' ')   {
                        
                        if (cell->colour() == 'W') {
                            cout << "\nThis cell is already a wall: (" << r << ", " << c << ")\n";
                        }
                        else    {
                            break;
                        }

                        dir = next_direction(dir);
                        cout << "\nNew dir: " << dir << endl;
                        u  = f(*itr, dir);
                        if (u != nullptr)   {
                            r  = u->row();
                            c  = u->col();
                            cell   = CUR_USER_BOARD.cell(r, c);
                        }
                    }

                    cout << "\nWill assume & set this cell to W: (" << r << ", " << c << ")\n";

                    cell->colour('W'); // Assume this cell to be our Wall
                    CUR_USER_BOARD.update_region(cell);

                    return u;
                }
            }
        }
        return nullptr;

    }

    void wall_builder_t::build_next_wall(bool new_wall)   {
        auto direction   = ' ';
        if (walls_built_.size() == 0) {
            direction = ' ';
        }
        else {
            auto latest_wall = walls_built_.back();
            //if (latest_wall.build_status == true)   {
            if (new_wall)   {
                // Latest wall is strong, go build next one
                direction   = ' ';
            }
            else    {
                // Rebuild latest wall in a different direction
                assert(latest_wall.direction != ' ');
                direction   = next_direction(latest_wall.direction);
            }
        }

        auto new_dir     = next_direction(direction);
        rcell_t* wall_built = nullptr;

        while (new_dir != ' ')  {   // ' ' means exhausted all directions

            cerr << "\nWall id: " << game_->smallest_incomplete_wall_id_ << endl;
            cerr << "\nTrying new direction: " << new_dir << endl;
            wall_built = build_single_wall(game_->smallest_incomplete_wall_id_, new_dir);

            auto result_state   = nurikabe::UNKNOWN;
            if (wall_built != nullptr) {
                result_state   = game_->check_for_validity();
                if (nurikabe::NO_ERROR_YET != result_state)  {

                    cout << "Undoing the move becuase :";
                    game_->print_state(result_state);
                    
                    wall_built->colour('G');
                    //CUR_BOARD.reset();
                    --game_->m_cur_board;  //Backtrack
                    new_dir     = next_direction(new_dir);
                }
                else {
                    cout << "Valid move becuase :";
                    game_->print_state(result_state);
                    //built_new_wall  = true;
                    //draw_board();
                    walls_built_.emplace_back(wall_info_t(
                                wall_built->row(), wall_built->col(), new_dir, true));
                    break;
                }
            }
            else    {
                // no more walls can be built around smallest_incomplete_wall_id_
                // Let's go to higher number
                break;
            }
        }
        if ((wall_built == nullptr) || (new_dir == ' '))    {
            // no more walls can be built around smallest_incomplete_wall_id_
            // Let's go to higher number
            ++game_->smallest_incomplete_wall_id_;
            if (game_->smallest_incomplete_wall_id_ > highest_wall_id_)    {
                return;
            }
            build_next_wall();
        }
    }

    void wall_builder_t::unbuild_latest_wall()  {
        assert(walls_built_.size() > 0);

        auto latest_wall            = walls_built_.back();
        auto latest_wall_direction  = latest_wall.direction;
        auto latest_wall_id         = CUR_USER_BOARD.cell(latest_wall.row, latest_wall.col)->region()->wall_id();
        std::cout << "\nUnbuilding wall at: " << latest_wall.row << ", " << latest_wall.col << " whose direction was: " << latest_wall_direction << endl;

        if (latest_wall_id != game_->smallest_incomplete_wall_id_)  {
            assert(latest_wall_id < game_->smallest_incomplete_wall_id_);
            --game_->smallest_incomplete_wall_id_;
        }

        assert(game_->m_cur_board != 0);
        --game_->m_cur_board;

        latest_wall.build_status = false;
        //walls_built_.erase(walls_built_.end() - 1);   //TODO: I need to refer later!
    }

    char wall_builder_t::next_direction(const char wall_direction)   {
        if (wall_direction == ' ')   {
            return 'U';   //1st attempt
        }
        else if (wall_direction == 'U') {
            return 'D';
        }
        else if (wall_direction == 'D') {
            return 'R';
        }
        else if (wall_direction == 'R')    {
            return 'L';
        }
        else    {
            assert(wall_direction == 'L');   
            //return 'U';
            return ' ';
        }
    }

    nurikabe::nurikabe(const std::uint32_t rows, const std::uint32_t cols):
        m_rows(rows),
        m_cols(cols),
        //m_board(rows, cols),
        m_boards(10),   // Decide a good number
        m_cur_board(0),
        smallest_incomplete_wall_id_(1),
        wb_(this)   {
            CUR_BOARD.init(rows, cols);
            //cout << "\nInitial";
            //draw_board();
        }

    void nurikabe::set_wall(uint32_t r, uint32_t c, uint32_t v) {
        CUR_BOARD.cell(r, c, v);
    }

    nurikabe::game_state_t nurikabe::run_fill_rules()  {
        // 1. Mark all neighbous of '1' as Black
        {
            if (smallest_incomplete_wall_id_ == 1)  {
                SWEEP_BOARD {
                    if (itr->id() == 1)   mark_1s_neigh((*itr));
                }
                ++smallest_incomplete_wall_id_;
            }
        }
        auto state = check_for_validity();
        if (state != NO_ERROR_YET)  {
            return state;                
        }

        // 2. Mark a cell in-between 2 walls as Black
        {
            SWEEP_BOARD {
                mark_mid_cell((*itr));
            }
        }
        state = check_for_validity();
        if (state != NO_ERROR_YET)  {
            return state;                
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
        state = check_for_validity();
        if (state != NO_ERROR_YET)  {
            return state;                
        }

        // 4. Fill Black holes
        {
            SWEEP_BOARD {
                fill_black_hole(*itr);
            }
        }
        state = check_for_validity();
        if (state != NO_ERROR_YET)  {
            return state;                
        }

        // 5. Fill White holes
        {
            SWEEP_BOARD {
                fill_white_hole(*itr);
            }
        }
        state = check_for_validity();

        return state;   // Can be error state too
    }

    nurikabe::game_state_t nurikabe::solve()  {

        auto state  = run_fill_rules();
        assert(state == NO_ERROR_YET);
        CUR_BOARD.update_regions();

        bool request_build_new_wall = true;

        while (smallest_incomplete_wall_id_ != 5)   {
            cerr << "\nTry & build a new wall\n";
            wb_.build_next_wall(request_build_new_wall);
            draw_board();

            cerr << "\nRunning fill rules:\n";
            state   = run_fill_rules();
            print_state(state);

            CUR_BOARD.update_regions();
            state   = check_for_validity();
            draw_board();   cout << endl;
            // //Print regions
            // SWEEP_BOARD     {
            //     auto r  = (*itr)->region();
            //     cout << r->region() << "\t" << r->size() << endl;
            // }

            if (state == NO_ERROR_YET)  {
                cerr << "\nWall successfully built:\n";
                draw_board();
                request_build_new_wall  = true;
                //solve();
            }
            else    {
                cerr << "\nDemolishing previous wall\n";
                wb_.unbuild_latest_wall();
                request_build_new_wall  = false;
            }
        }
        return state;
    }

    nurikabe::game_state_t nurikabe::solve_old()  {

        auto state  = run_fill_rules();

        //cout << "\nGame state after apply fill rules: ";   print_state(state);
        assert(state == NO_ERROR_YET);

        CUR_BOARD.update_regions();

        //Print regions
        // SWEEP_BOARD     {
        //     auto r  = (*itr)->region();
        //     cout << r->region() << "\t" << r->size() << endl;
        // }

        char wall_dir = ' '; //Feedback from assume_and_build_wall()
        if (!game_completed())  {
            auto new_wall = assume_and_build_wall(wall_dir);
            while (new_wall != true || wall_dir != ' ')   {   // New wall not yet built
                new_wall = assume_and_build_wall(wall_dir);
            }
            if (wall_dir == ' ')    {
                // All options for current smallest_incomplete_wall_id_ are tried & failed
                // Let us roll back on our previously built wall for smallest_incomplete_wall_id_ 
                --smallest_incomplete_wall_id_;
                //auto new_wall = assume_and_build_wall(walls_built_.back())
                    // Build another wall for previos smallest_incomplete_wall_id_


            }
            cout << "\nAfter the assumption: ";
            draw_board();

            auto assumption_valid   = run_fill_rules();

            while (assumption_valid != NO_ERROR_YET)  { //Backtrack as the recently built new wall is in wrong place
                CUR_BOARD.reset();                        
                assert(m_cur_board != 0);
                --m_cur_board;
                assert(!game_completed());
                new_wall = assume_and_build_wall(wall_dir);
                cout << "\nAfter the assumption: ";
                draw_board();
                assumption_valid    = run_fill_rules();
            }

            if (smallest_incomplete_wall_id_ == 5)   {
                ;   //Stop solving
            }
            else    {
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
            //auto n  = cell->id();
            auto n  = cell->region()->wall_id();
            assert(n != numeric_limits<uint32_t>::max());
            reach_neigh(cell, n-1);
        }

        // Blacken cells that are adjacent to complete walls
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

    bool nurikabe::build_a_wall(uint32_t id, const char direction)    {
        
        assert((direction == 'U') || (direction == 'D') ||
                (direction == 'L') || (direction == 'R'));

        auto f  = [this](rcell_t* rc, const char c)    {
            if (c == 'U')   {
                return up(rc, CUR_BOARD);
            }
            else if (c == 'D')  {
                return down(rc, CUR_BOARD);
            }
            else if (c == 'R')  {
                return right(rc, CUR_BOARD);
            }
            else if (c == 'L')  {
                return left(rc, CUR_BOARD);
            }
            else    {
                assert(0);
                return rc;
            }
        };

        bool built_new_wall = false;
        
        SWEEP_BOARD {
            assert(*itr != nullptr);
            //TODO: Add assumptions for all numbers 3, 4, 5 etc
            if (((*itr)->id() == id) && ((*itr)->region()->region() == region_t::INCOMPLETE_WALL_REGION))  {
                //auto u  = up(*itr, CUR_BOARD);
                auto u  = f(*itr, direction);
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
                    //assert(new_cell->region()->region() == region_t::COMPLETE_WALL_REGION);
                    //assert(down(new_cell, CUR_BOARD)->region()->region() == region_t::COMPLETE_WALL_REGION);
                    //assert(new_cell->region() == down(new_cell, CUR_BOARD)->region());
                    //TODO::Continue here: Re enable above comment
                    
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
                        walls_built_.push_back(make_pair(make_pair(r, c), direction));
                        break;
                    }
                }
            }
        }
        return built_new_wall;
    }

    bool nurikabe::assume_and_build_wall(char& wall_direction)  {
        if (wall_direction == ' ')   {
            wall_direction = 'U';   //1st attempt
        }
        else if (wall_direction == 'U') {
            wall_direction = 'D';
        }
        else if (wall_direction == 'D') {
            wall_direction = 'R';
        }
        else if (wall_direction == 'R')    {
            wall_direction = 'L';
        }
        else    {
            assert(wall_direction == 'L');   
            wall_direction = 'U';
        }

        bool built_new_wall     = build_a_wall(smallest_incomplete_wall_id_, wall_direction);
        
        if (!built_new_wall && (wall_direction == 'R'))    {
            //Consider net higher numbered wall for building as current number is exhausted
            ++smallest_incomplete_wall_id_;
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
                        cerr << "\nPool at: " << c->row() << ", " << c->col() << endl;
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
