#include "board.h"

#include <limits>
#include <algorithm>

using namespace std;

namespace ne    {

    region_t::region_t(board_t* bptr)
        : region_(UNKNOWN_REGION),
        wall_(0),
        board_(bptr)    {}

    region_t::rcells_t region_t::cells() const { return cells_; }

    void region_t::add_cell(rcell_t* c) {
        if (c != nullptr)   {
            if (region_ == INCOMPLETE_WALL_REGION ||
                    region_ == COMPLETE_WALL_REGION)    {

                assert(board_->is_wall(c));
                cells_.push_back(c);
                assert(cells_.size() > wall_);
                if (cells_.size() == wall_)    {
                    region_ = COMPLETE_WALL_REGION;
                }
            }
            else    {
                assert(region_ == WATER_REGION || region_ == UNKNOWN_REGION);
                if(board_->is_wall(c))  {
                    assert(region_ == UNKNOWN_REGION);
                    wall_   = c->id();
                    if (wall_ == 1) {
                        region_ = COMPLETE_WALL_REGION;
                    }
                    else    {
                        region_ = INCOMPLETE_WALL_REGION;
                    }
                }
                else    {
                    if (c->colour() == 'B') {
                        if (region_ == UNKNOWN_REGION)  {
                            region_ = WATER_REGION;
                        }
                    }
                }
                cells_.push_back(c);
                assert(c->region() == this);
            }
        }
    }

    void region_t::update_region(rcell_t* c)    {
        if (c != nullptr)   {
            if (region_ == UNKNOWN_REGION)  {
                if (c->colour() == 'B') {
                    region_ = WATER_REGION;
                }
                else if (board_->is_wall(c))    {
                    wall_   = c->id();
                    if (wall_ == 1) {
                        region_ = COMPLETE_WALL_REGION;
                    }
                    else    {
                        region_ = INCOMPLETE_WALL_REGION;
                    }
                }
            }
            else if (region_ == INCOMPLETE_WALL_REGION)   {
                assert(board_->is_wall(c));
                if (cells_.size() == wall_)    {
                    region_ = COMPLETE_WALL_REGION;
                }
            }
                //TODO:Any more cases? 
        }
    }


    // If there is a use for remove_cell(),
    // we should complete this definition &
    // maintain this class's invariant
    //void region_t::remove_cell(rcell_t* c)  {
    //    if (c != nullptr)   {
    //        cells_.erase(std::remove_if(cells_.begin(), cells_.end(),
    //                    [c](const rcell_t* cell)    {
    //                    return *cell == *c;
    //                    }), cells_.end());
    //    }
    //}

    region_t::region_type region_t::region()  { return region_; }

    void region_t::set_region(region_type r)    { region_ = r; }

    uint32_t region_t::wall_id()    { return wall_; }

    void region_t::merge_with_region(region_t* r)   {
        for(auto c : r->cells())    {
            c->join_region(this) ;
        }
    }

    board_t::board_t(const int rows, const int cols)
        : rows_(rows),
        cols_(cols),
        cells_(),
        regions_()  {
            init(rows, cols);
        }

    void board_t::init(const int rows, const int cols)    {
        rows_   = rows;
        cols_   = cols;

        auto r  = -1;
        auto c  = 0;

        cells_.clear();
        
        for(auto i = 0; i < rows*cols; ++i, ++c)   {
            if (i % cols == 0)  {
                ++r;
                c   = 0;
            }
            
            regions_.emplace_back(unique_ptr<region_t>(new region_t(this)));
            
            cells_.emplace_back(unique_ptr<rcell_t>(new rcell_t(regions_[i].get())));
            //regions_[i].get()->add_cell(cells_[i].get());
            set_cell_attr(*cells_[i], numeric_limits<uint32_t>::max(), 'G', r, c);
        }
    }

    void board_t::reset()   {

        rows_   = 0;
        cols_   = 0;
        cells_.clear();
    }

    void board_t::cell(uint32_t r, uint32_t c, uint32_t v)  {
        assert(r < rows_);
        assert(c < cols_);
        assert(v < cells_.size());
        cells_[r*cols_ + c]->id(v);
    }

    rcell_t* board_t::cell(uint32_t r, uint32_t c) const  {
        assert(r < rows_);
        assert(c < cols_);
        return cells_[r*cols_ + c].get();
    }

    bool board_t::is_wall(rcell_t* c) const    {
        return (c->id() != numeric_limits<uint32_t>::max() &&
            (c->id() != 0)) ||
            (c->colour() == 'W');
    }

    void board_t::update_region(rcell_t* c)   {
        auto u  = up(c, *this);
        relate_2_rcells(c, u);

        u  = down(c, *this);
        relate_2_rcells(c, u);

        u  = left(c, *this);
        relate_2_rcells(c, u);

        u  = right(c, *this);
        relate_2_rcells(c, u);
    }

    void board_t::update_regions()  {
        for(auto i = 0; i < cells_.size(); ++i) {
            auto c  = cells_[i].get();
            update_region(c);
        }
    }

    void board_t::draw(ostream& ost, const std::string delim) const    {
        ost << endl;

        for(auto i = 0; i < cells_.size(); ++i) {

            if(i % rows_ == 0)   ost << endl;
            
            auto num    = cells_[i]->id();
            if (num == numeric_limits<uint32_t>::max())
                ost << cells_[i]->colour() << delim;
            else
                ost << cells_[i]->id() << delim;
        }

        ost << endl;
    }

    board_t::iterator board_t::begin()  {
        return iterator(this);
    }

    board_t::iterator board_t::end() {
        return iterator(this, cells_.end());
    }

    void set_cell_attr(rcell_t& cell, uint32_t id, char colour, uint32_t row, uint32_t col)    {
        assert(colour == 'B' ||
                colour == 'W' ||
                colour == 'G');
        cell.id(id);
        cell.colour(colour);
        cell.row(row);
        cell.col(col);
    }

    bool is_inside_board(const board_t& board, const uint32_t r, const uint32_t c)    {
        auto max_r  = board.rows();
        auto max_c  = board.cols();
        return r < max_r &&
            c < max_c;
    }
    
    void relate_2_rcells(rcell_t* a, rcell_t* b)    {
        if (((a != nullptr) && (b != nullptr)) &&
                ((a->colour() != 'G') && (b->colour() != 'G')))  {
            auto ar = a->region();
            auto br = b->region();
            if ((ar != br) && (a->colour() == b->colour())) {
                if (ar->region() == br->region())   {
                    ar->merge_with_region(br);
                }
            }
        }
    }
}
