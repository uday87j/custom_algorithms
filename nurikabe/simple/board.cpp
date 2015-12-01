#include "board.h"

#include <limits>
#include <algorithm>

using namespace std;

namespace ne    {

    region_t::region_t(board_t* bptr)
        : region_(UNKNOWN_REGION),
        wall_(0),
        board_(bptr)    {}

    region_t::region_t(const region_t& r)  {
        region_ = r. region_;
        cells_.reserve(r.cells_.size());
        std::copy(begin(r.cells_), end(r.cells_), begin(cells_)); //Only pointer (shallow) copy
        wall_   = r.wall_;
        board_  = r.board_;
    }
    
    region_t& region_t::operator = (const region_t& r) {
        region_ = r. region_;
        //cells_.reserve(max(r.cells_.size(), cells_.size()));
        cells_.resize(max(r.cells_.size(), cells_.size()));
        std::copy(begin(r.cells_), end(r.cells_), begin(cells_)); //Only pointer (shallow) copy
        wall_   = r.wall_;
        board_  = r.board_;
        return *this;
    }

    void region_t::set_board(board_t* b)  {
        if (b != nullptr)   board_ = b;
    }

    void region_t::set_cell_ptr(region_cell_t<std::uint32_t>* rc, uint32_t index) {
        if ((index < cells_.size()) && (rc != nullptr))  {
            cells_[index]   = rc;
        }
    }

    region_t::rcells_t region_t::cells() const { return cells_; }

    void region_t::add_cell(rcell_t* c) {
        if (c != nullptr)   {
            if (region_ == INCOMPLETE_WALL_REGION ||
                    region_ == COMPLETE_WALL_REGION)    {

                assert(board_->is_wall(c));
                wall_   = min(wall_, c->id());
                wall_   = min(wall_, c->region()->wall_id());
                assert(wall_ != numeric_limits<uint32_t>::max());
                //if (c->id() != numeric_limits<uint32_t>::max())   {
                //    if (wall_ != numeric_limits<uint32_t>::max()) {
                //        wall_   = c->id();
                //    }
                //}
                cells_.push_back(c);
                //assert(cells_.size() >= wall_);   //TODO: Required?
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
                assert((board_->is_wall(c)) ||
                        (c->colour() == 'G')); //Happens in backtracking
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
    void region_t::set_wall_id(uint32_t w_id)   { wall_ = w_id; }

    void region_t::merge_with_region(region_t* r)   {
        assert(r != nullptr);
        if (r->region() == COMPLETE_WALL_REGION ||
                this->region() == COMPLETE_WALL_REGION)    {
            return; //Can't merge into complete wallls            
        }
        if (this->is_wall() &&
                r->is_wall())  {
            if (this->wall_ != r->wall_)    {
                return; //Don't merge 2 independent walls
            }
        }
        //TODO: Add if() for not merging independent wall w/ same wall_
        for(auto& c : r->cells())    {
            c->join_region(this) ;
            assert(this == c->region());
        }
    }

    board_t::board_t(const int rows, const int cols)
        : rows_(rows),
        cols_(cols),
        cells_(),
        regions_()  {
            init(rows, cols);
        }

    board_t::board_t(const board_t& b)
        : rows_(b.rows_),
        cols_(b.cols_),
        cells_(),
        regions_()  {
            (*this) = b;
        }

    board_t& board_t::operator = (const board_t& b) {
        
        // Resize our board
        rows_ = b.rows_;
        cols_ = b.cols_;
        init(rows_, cols_);

        for(auto i = 0; i < rows_*cols_; ++i)   {
            *(cells_[i])  = *(b.cells_[i]);   //Deep copy
            *(regions_[i])= *(b.regions_[i]);
        }
        // Our board's cells & regions now point to "b"'s cells & regions
        
        // Let's change those pointers to point into our cells & regions
        //cout << "\nNum regions: " << b.regions().size() << endl;
        //TODO: Revisit: something wrong here!
        auto ref_regions    = b.regions();
        assert(ref_regions.size() == rows_*cols_);
        for(auto i = 0; i < rows_*cols_; ++i)   {
            regions_[i]->set_board(this);

            if (ref_regions[i]->region() != region_t::INVALID_REGION)   {
                auto ref_cells  = ref_regions[i]->cells();
                assert(regions_[i]->size() == ref_cells.size());
                for (auto j = 0; j < ref_regions[i]->size(); ++j)    {
                    //cout << "\nNum cells in this region: " << regions_[j]->size() << endl;
                    //Point my regions to cells in my board
                    //Read the indeces of those cells from "b"
                    auto r  = ref_cells[j]->row();
                    auto c  = ref_cells[j]->col();
                    auto cell  = this->cell(r, c);
                    //assert(cell->region() == ref_regions[i]);
                    regions_[i]->set_cell_ptr(cell, j); 
                    //Point the cells to their newly assigned region
                    cell->set_region(regions_[i].get());
                }
            }
            else    {
                assert(ref_regions[i]->region() == region_t::INVALID_REGION);
            }
        }

        return *this;
    }

    void board_t::init(const int rows, const int cols)    {
        rows_   = rows;
        cols_   = cols;

        auto r  = -1;
        auto c  = 0;

        cells_.clear();
        regions_.clear();
        
        for(auto i = 0; i < rows*cols; ++i, ++c)   {
            if (i % cols == 0)  {
                ++r;
                c   = 0;
            }
            
            regions_.emplace_back(unique_ptr<region_t>(new region_t(this)));
            
            cells_.emplace_back(unique_ptr<rcell_t>(new rcell_t(regions_[i].get())));
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
        if (v != numeric_limits<uint32_t>::max())   {
            cells_[r*cols_ + c]->id(v);
            cells_[r*cols_ + c]->colour('W');
        }
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
        if (c->region()->region() != region_t::INVALID_REGION)  {
            auto u  = up(c, *this);
            relate_2_rcells(c, u);

            u  = down(c, *this);
            relate_2_rcells(c, u);

            u  = left(c, *this);
            relate_2_rcells(c, u);

            u  = right(c, *this);
            relate_2_rcells(c, u);
        }
    }

    void board_t::update_regions()  {
        for(auto i = 0; i < cells_.size(); ++i) {
            auto c  = cells_[i].get();
            update_region(c);
        }
    }

    board_t::regions_ptr_t board_t::regions() const  {
        regions_ptr_t ret(rows_*cols_);
        for(auto i = 0; i < cells_.size(); ++i) {
            ret[i]  = regions_[i].get();
        }
        return ret;
    }

    void board_t::draw(ostream& ost, const std::string delim) const    {
        ost << endl;

        for(auto i = 0; i < cells_.size(); ++i) {

            if(i % rows_ == 0)   ost << endl;
            
            auto num    = cells_[i]->id();
            if (num == numeric_limits<uint32_t>::max()) {
                ost << cells_[i]->colour() << delim << delim;
            }
            else    {
                //ost << cells_[i]->colour() << "(";
                //ost << cells_[i]->id() << ")" << delim << delim;
                ost << cells_[i]->id() << delim << delim;
            }
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
