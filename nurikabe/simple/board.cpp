#include "board.h"

#include <limits>

using namespace std;

namespace ne    {

    board_t::board_t(const int rows, const int cols)
        : rows_(rows),
        cols_(cols),
        cells_()    {
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
            cells_.emplace_back(unique_ptr<icell_t>(new icell_t));
            set_cell_attr(*cells_[i], numeric_limits<uint32_t>::max(), 'G');
            cells_[i]->row(r);
            cells_[i]->col(c);
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

    icell_t* board_t::cell(uint32_t r, uint32_t c) const  {
        assert(r < rows_);
        assert(c < cols_);
        return cells_[r*cols_ + c].get();
    }

    bool board_t::is_wall(icell_t* c)    {
        return c->id() != numeric_limits<uint32_t>::max();
    }

    void board_t::draw(ostream& ost, const std::string delim)    {
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

    void set_cell_attr(icell_t& cell, uint32_t id, char colour)    {
        assert(colour == 'B' ||
                colour == 'W' ||
                colour == 'G');
        cell.id(id);
        cell.colour(colour);
    }

    bool is_inside_board(const board_t& board, const uint32_t r, const uint32_t c)    {
        auto max_r  = board.rows();
        auto max_c  = board.cols();
        return r < max_r &&
            c < max_c;
    }
}
