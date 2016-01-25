#include "nurikabe.h"

#include <limits>
#include <algorithm>

using namespace std;

// Unfortunate macros
#define CUR_BOARD   m_boards[m_cur_board]
#define CUR_USER_BOARD   game_->m_boards[game_->m_cur_board]
#define SWEEP_BOARD for (auto itr = CUR_BOARD.begin(); itr != CUR_BOARD.end(); ++itr)
#define SWEEP_USER_BOARD for (auto itr = CUR_USER_BOARD.begin(); itr != CUR_USER_BOARD.end(); ++itr)
#define UNBUILDABLE_WALL unbuildable_cells_[game_->m_cur_board]
#define DEBUG(...) 
//#define DEBUG(...) cout << __VA_ARGS__;
#define DRAW(...)
//#define DRAW(...) __VA_ARGS__;

namespace ne {

    wall_info_t::wall_info_t(uint32_t r, uint32_t c, char dir, bool built)
        :row(r),
        col(c),
        direction(dir),
        build_status(built) {}

    wall_builder_t::wall_builder_t(nurikabe* user_, uint32_t highest_wall_id)
        : game_(user_),
        highest_wall_id_(highest_wall_id),
        unbuildable_cells_(100)  {}

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
            if (((*itr)->region()->wall_id() == wall_id) && ((*itr)->region()->region() == region_t::INCOMPLETE_WALL_REGION))  {
                auto u  = f(*itr, direction);

                if ((u != nullptr) && (u->colour() != 'W')) {
                //if ((u != nullptr) && (u->colour() == 'G')) {
                // if (u != nullptr) {
                    assert(u->colour() != 'W');

                    auto r  = u->row();
                    auto c  = u->col();

                    auto cell   = CUR_USER_BOARD.cell(r, c);

                    if (cell->colour() == 'W') {
                        DEBUG("\nThis cell is already a wall: (" << r << ", " << c << ")\n");
                    }
                    else if (find(begin(UNBUILDABLE_WALL), end(UNBUILDABLE_WALL), 
                                make_pair(cell->row(), cell->col())) != end(UNBUILDABLE_WALL)) {
                        DEBUG("\nA wall can't be built here: (" << r << ", " << c << ")\n");
                    }
                    else    {
                        
                        // A wall can be built here
                        // Clone our current board
                        game_->m_boards[game_->m_cur_board + 1]   = game_->m_boards[game_->m_cur_board];
                        ++game_->m_cur_board;
                        cell   = CUR_USER_BOARD.cell(r, c);
                        DEBUG("\nIncrement board: " << game_->m_cur_board << endl);

                        DEBUG("\nWill assume & set this cell to W: (" << r << ", " << c << ")\n");

                        cell->colour('W'); // Assume this cell to be our Wall

                        DEBUG("\nSetting cell to W: (" << r << "," << c << ")\n");
                        DRAW(game_->draw_board());
                        
                        cell->region()->set_wall_id(itr->region()->wall_id());

                        auto w_leader = itr->region()->get_wall_leader();
                        cell->region()->set_wall_leader(w_leader.first, w_leader.second);

                        CUR_USER_BOARD.update_region(cell);

                        return u;
                    }
                }
            }
        }
        return nullptr;

    }

    void wall_builder_t::build_next_wall(bool new_wall)   {
        auto direction   = ' ';

        auto new_dir     = next_direction(direction);
        rcell_t* wall_built = nullptr;

        while (new_dir != ' ')  {   // ' ' means exhausted all directions for current cell

            DEBUG("\nWall id: " << game_->smallest_incomplete_wall_id_ << endl);
            DEBUG("\nTrying new direction: " << new_dir << endl);
            wall_built = build_single_wall(game_->smallest_incomplete_wall_id_, new_dir);

            auto result_state   = nurikabe::UNKNOWN;
            if (wall_built != nullptr) {
                result_state   = game_->check_for_validity();
                if (nurikabe::NO_ERROR_YET != result_state)  {

                    DEBUG("Undoing the move becuase :");
                    game_->print_state(result_state);
                    
                    UNBUILDABLE_WALL.push_back(make_pair(wall_built->row(), wall_built->col()));

                    --game_->m_cur_board;  //Backtrack
                    DEBUG("\nDecrement board: " << game_->m_cur_board << endl);
                    
                    new_dir     = next_direction(new_dir);  //Try in a different direction
                }
                else {
                    DEBUG("Valid move becuase :");
                    game_->print_state(result_state);
                    
                    DRAW(game_->draw_board());
                    
                    walls_built_.emplace_back(wall_info_t(
                                wall_built->row(), wall_built->col(), new_dir, true));
                    break;
                }
            }
            else    {
                // no more walls can be built around in current direction
                // Try a different direction
                new_dir     = next_direction(new_dir);
            }
        }

        if (new_dir == ' ')   {
            // Exhausted searching around all directions

            if (game_->any_incomplete_walls_of_id(game_->smallest_incomplete_wall_id_)) {
                unbuild_latest_wall();  // As the latest built wall is incorrect & we can't proceed with current state
                build_next_wall(true);
            }
            else    {
                assert(!game_->any_incomplete_walls_of_id(game_->smallest_incomplete_wall_id_));

                // no more walls can be built around smallest_incomplete_wall_id_
                // Let's go to higher number
                ++game_->smallest_incomplete_wall_id_;
            }

            // TODO: Get rid of this check here
            if (game_->smallest_incomplete_wall_id_ > highest_wall_id_)    {
                DEBUG("\nExit build_next_wall() as smallest_incomplete_wall_id_ > highest_wall_id_\n");
                DEBUG(endl << game_->smallest_incomplete_wall_id_ << " > " << highest_wall_id_ << endl);
                return;
            }

            build_next_wall(true);
        }
        else    {
            ;
            // A proper wall was built            
        }
    }

    void wall_builder_t::unbuild_latest_wall()  {
        assert(walls_built_.size() > 0);

        for (auto ritr = walls_built_.rbegin(); ritr != walls_built_.rend(); ++ritr)    {
            auto latest_wall            = *ritr;

            assert(CUR_USER_BOARD.is_wall(CUR_USER_BOARD.cell(latest_wall.row, latest_wall.col)));
            
            if (CUR_USER_BOARD.is_wall(CUR_USER_BOARD.cell(latest_wall.row, latest_wall.col)))   {

                auto latest_wall_direction  = latest_wall.direction;
                auto latest_wall_id         = CUR_USER_BOARD.cell(latest_wall.row, latest_wall.col)->region()->wall_id();
                DEBUG("\nUnbuilding wall at: " << latest_wall.row << ", " << latest_wall.col << " whose direction was: " << latest_wall_direction << endl);

                // Step down in wall_id
                // TODO: Step down correctly
                if (latest_wall_id != game_->smallest_incomplete_wall_id_)  {
                    assert(latest_wall_id < game_->smallest_incomplete_wall_id_);
                    --game_->smallest_incomplete_wall_id_;
                    while (game_->walls_for_id_[game_->smallest_incomplete_wall_id_] == false) {
                        DEBUG("\nNo wall built of id: " << game_->smallest_incomplete_wall_id_);
                        --game_->smallest_incomplete_wall_id_;
                    }
                }

                assert(game_->m_cur_board != 0);
                UNBUILDABLE_WALL.clear();
                --game_->m_cur_board;
                DEBUG("\nDecrement board: " << game_->m_cur_board << endl);

                // Mark this cell as "non-wall"
                UNBUILDABLE_WALL.push_back(make_pair(latest_wall.row, latest_wall.col));

                // Done unbuilding latest wall
                DRAW(game_->draw_board());
                break;

            }
        }
        walls_built_.erase(walls_built_.end() - 1);
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
        HIGHEST_WALL_ID(8),
        wb_(this, HIGHEST_WALL_ID - 1),
        m_rows(rows),
        m_cols(cols),
        //m_board(rows, cols),
        m_boards(100),   // Decide a good number
        m_cur_board(0),
        smallest_incomplete_wall_id_(1),
        walls_for_id_(HIGHEST_WALL_ID, false)   {
            CUR_BOARD.init(rows, cols);
            //DEBUG("\nInitial");
            DRAW(draw_board());
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
            //DEBUG("\nAfter transformation:\n");
            //DRAW(draw_board());
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
        // TODO: This functionlity is not completely correct
        // {
        //     SWEEP_BOARD {
        //         fill_white_hole(*itr);
        //     }
        // }
        // state = check_for_validity();

        return state;   // Can be error state too
    }

    nurikabe::game_state_t nurikabe::solve()  {

        auto state  = run_fill_rules();
        assert(state == NO_ERROR_YET);
        CUR_BOARD.update_regions();

        bool request_build_new_wall = true;

        while (smallest_incomplete_wall_id_ != HIGHEST_WALL_ID)   {
            DEBUG("\nTry & build a new wall\n");
            wb_.build_next_wall(request_build_new_wall);
            DRAW(draw_board());

            DEBUG("\nRunning fill rules:\n");
            state   = run_fill_rules();
            print_state(state);

            CUR_BOARD.update_regions();
            state   = check_for_validity();
            print_state(state);
            DRAW(draw_board();   DEBUG(endl));
             //Print regions
             //SWEEP_BOARD     {
             //    auto r  = (*itr)->region();
             //    DEBUG(r->region() << "\t" << r->size() << endl);
             //}

            if (state == NO_ERROR_YET)  {
                DEBUG("\nWall successfully built:\n");
                DRAW(draw_board());
                request_build_new_wall  = true;
                walls_for_id_[smallest_incomplete_wall_id_] = true; //We built a wall for this id
            }
            else    {
                DEBUG("\nDemolishing previous wall\n");
                wb_.unbuild_latest_wall();
                request_build_new_wall  = false;
            }
        }

        cout << "\nFinal solution:\n";
        draw_board();

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
        if (s == SOLVED)    DEBUG("SOLVED");
        if (s == POOL_EXISTS)    DEBUG("POOL_EXISTS");
        if (s == INCOMPLETE)    DEBUG("INCOMPLETE");
        if (s == INCOMPLETE_WALLS)    DEBUG("INCOMPLETE_WALLS");
        if (s == UNREACHABLE_WATER)    DEBUG("UNREACHABLE_WATER");
        if (s == DISCONNECTED_WATER)    DEBUG("DISCONNECTED_WATER");
        if (s == OVERLAPPING_ISLANDS)    DEBUG("OVERLAPPING_ISLANDS");
        if (s == NO_ERROR_YET)    DEBUG("NO_ERROR_YET");
        if (s == UNKNOWN)    DEBUG("UNKNOWN");
        DEBUG(endl);
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
                if(c != nullptr && CUR_BOARD.is_wall(c) && !CUR_BOARD.is_wall(n))  n->colour('B');
            }

            n  = down(cell, CUR_BOARD);
            if (n != nullptr)   {
                auto* c  = down(n, CUR_BOARD);
                if(c != nullptr && CUR_BOARD.is_wall(c) && !CUR_BOARD.is_wall(n))  n->colour('B');
            }

            n  = left(cell, CUR_BOARD);
            if (n != nullptr)   {
                auto* c  = left(n, CUR_BOARD);
                if(c != nullptr && CUR_BOARD.is_wall(c) && !CUR_BOARD.is_wall(n))  n->colour('B');
            }

            n  = right(cell, CUR_BOARD);
            if (n != nullptr)   {
                auto* c  = right(n, CUR_BOARD);
                if(c != nullptr && CUR_BOARD.is_wall(c) && !CUR_BOARD.is_wall(n))  n->colour('B');

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
            //DRAW(draw_board());
            //DEBUG("\n2s done");
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
                    //DEBUG("\nHit a case: " << cell->colour() << endl);
                    if (cell->colour() == 'G')  cell->colour('B');
                    //DEBUG("\nNew: " << cell->colour() << endl);
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
                    DEBUG("\nfill_white_hole(): Setting cell to W: (" << gc->row() << "," << gc->col() << ")");
                    DEBUG("\nRelated to cell : (" << cell->row() << ", "<< cell->col() << ")\n");
                    gc->colour('W');                        
                }
            }
        }
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

        if (any_disconnected_water())    {
            return DISCONNECTED_WATER;
        }

        return NO_ERROR_YET;
    }

    bool nurikabe::does_pool_exist() {
        auto regs   = CUR_BOARD.regions();
        for (auto reg : regs) {
            if ((reg->region() == region_t::WATER_REGION) && (reg->size() >= 4))    {
                auto cells = reg->cells();
                //for (auto c : cells)    {
                //    DEBUG(c->colour() << "\t" << endl);
                //}
                for (auto* c : cells)    {
                    assert(c->colour() == 'B');

                    auto r  = right(c, CUR_BOARD);
                    if ((r == nullptr) || (reg != r->region()))   continue;

                    assert(r->colour() == 'B');
                    auto d  = down(c, CUR_BOARD);

                    if ((d == nullptr) || (reg != d->region()))   continue;
                    assert(d->colour() == 'B');
                    
                    auto rd = down(r, CUR_BOARD);
                    if ((rd != nullptr) && (reg == rd->region())) {
                        assert(rd->colour() == 'B');
                        DEBUG("\nPool at: " << c->row() << ", " << c->col() << endl);
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
                if (r != nullptr && CUR_BOARD.is_wall(r) && (itr->region() != r->region()))   {
                    DEBUG("\nOverlapping islands Right at (" << itr->row() << ", " << itr->col() << ")\n");
                    return true;    // 2 different regions are wallls & adjacent                    
                }

                auto d  = down(*itr, CUR_BOARD);
                if (d != nullptr && CUR_BOARD.is_wall(d) && (itr->region() != d->region()))   {
                    DEBUG("\nOverlapping islands Down at (" << itr->row() << ", " << itr->col() << ")\n");
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

    bool nurikabe::any_incomplete_walls() {
        SWEEP_BOARD     {
            if (itr->region()->region() == region_t::INCOMPLETE_WALL_REGION)   {
                return true;
            }
        }
        return false;
    }

    bool nurikabe::any_incomplete_walls_of_id(uint32_t w_id) {
        SWEEP_BOARD     {
            if (itr->region()->wall_id() == w_id &&
                    itr->region()->region() == region_t::INCOMPLETE_WALL_REGION)   {
                return true;
            }
        }
        return false;
    }

    bool nurikabe::any_disconnected_water()  {
        SWEEP_BOARD     {
            if (itr->colour() == 'G')   {
                return false;   // The game is incomplete, can't determine disconnected water status
            }
        }

        region_t* reg   = nullptr;
        auto regs   = CUR_BOARD.regions();
        for (auto r : regs) {
            if (r->region() == region_t::WATER_REGION)  {
                if (reg == nullptr) {
                    reg     = r;
                }
                else    {
                    if (reg != r)   {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    bool nurikabe::any_unreachable_water()  {
        SWEEP_BOARD     {
            auto u  = up(*itr, CUR_BOARD);
            auto d  = down(*itr, CUR_BOARD);
            auto r  = right(*itr, CUR_BOARD);
            auto l  = left(*itr, CUR_BOARD);

            if (itr->colour() == 'B')   {
                if (((u == nullptr) || (u->colour() == 'W')) &&
                            ((d == nullptr) || (d->colour() == 'W')) &&
                            ((l == nullptr) || (l->colour() == 'W')) &&
                            ((r == nullptr) || (r->colour() == 'W')))   {
                    DEBUG("\nUnreachable water at (" << itr->row() << ", " << itr->col() << ")\n");
                    return true;
                }
            }
        }

        //TODO: Make this less costly, NOW!
        return false;

    }
}
