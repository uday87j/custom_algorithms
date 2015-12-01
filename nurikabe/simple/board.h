#ifndef BOARD_H_
#define BOARD_H_

#include <iostream>
#include <cassert>
#include <vector>
#include <memory>
#include <iterator>

namespace ne {

    template<typename T>
        struct cell_t   {
            cell_t()
                : colour('G'),
                id(T()) {}

            bool operator == (const cell_t& other) const  {
                return (id == other.id &&
                        colour == other.colour);
            }

            char colour;
            T id;
        };

    template <typename T>
        struct point_cell_t {
            point_cell_t()  {}

            bool operator == (const point_cell_t& other) const    {
                return (cell_ == other.cell() &&
                        row_ == other.row() &&
                        col_ == other.col());
            }

            T id() const  { return cell_.id; }
            char colour() const   { return cell_.colour; }
            uint32_t row() const  { return row_; }
            uint32_t col() const  { return col_; }
            cell_t<T> cell() const    { return cell_; }
            void id(const T id)    { cell_.id = id; }
            void row(uint32_t r)    { row_ = r; }
            void col(uint32_t c)    { col_ = c; }
            void colour(char c) { cell_.colour = c; }

            private:
                cell_t<T> cell_;
                uint32_t row_;
                uint32_t col_;
        };

    template <typename T>
    struct region_cell_t;

    class board_t;

    class region_t  {
        public:

            typedef std::vector<region_cell_t<std::uint32_t>*> rcells_t;

            enum region_type    {
                INCOMPLETE_WALL_REGION  = 0,
                COMPLETE_WALL_REGION,
                WATER_REGION,
                UNKNOWN_REGION,
                INVALID_REGION
            };

            region_t()  = delete;
            region_t(board_t*);
            region_t(const region_t&);
            region_t& operator = (const region_t&);

            rcells_t cells() const;

            size_t size() const     { return cells_.size(); }

            void set_board(board_t* b);
            void set_cell_ptr(region_cell_t<std::uint32_t>* rc, uint32_t index);

            void add_cell(region_cell_t<std::uint32_t>*);
            //void remove_cell(rcell_t*);

            region_type region();
            void set_region(region_type);
            void update_region(region_cell_t<std::uint32_t>*);

            void merge_with_region(region_t*);

            uint32_t wall_id();
            void set_wall_id(uint32_t w_id);

            bool is_wall()  { return (region_ == COMPLETE_WALL_REGION) ||
                (region_ == INCOMPLETE_WALL_REGION);    }


        private:

            region_type region_;
            rcells_t cells_;
            uint32_t wall_;
            board_t* board_;
    };



    template <typename T>
        struct region_cell_t    {
            region_cell_t()
                : region_cell_t(nullptr)    {}

            region_cell_t(region_t* r)
                : region_(r)    {
                    region_->add_cell(this);
                }

            region_cell_t(const region_cell_t& rc)  {
                pc_     = rc.pc_;
                assert(rc.region_ != nullptr);
                region_ = rc.region_;
            }

            region_cell_t& operator = (const region_cell_t& rc)  {
                pc_     = rc.pc_;
                assert(rc.region_ != nullptr);
                region_ = rc.region_;
                return *this;
            }

            bool operator == (const region_cell_t& other)   {
                return point_cell_t<T>::operator =(other) &&
                    region_ == other.region_;                    
            }

            T id() const  { return pc_.id(); }
            char colour() const   { return pc_.colour(); }
            uint32_t row() const  { return pc_.row(); }
            uint32_t col() const  { return pc_.col(); }
            cell_t<T> cell() const    { return pc_.cell(); }
            void id(const T id)    { 
                pc_.id(id);
                assert(region_ != nullptr);
                region_->update_region(this);
                //this->colour('W');    //Unless you leak max() logic here :-(
            }
            void row(uint32_t r)    { pc_.row(r); }
            void col(uint32_t c)    { pc_.col(c); }
            void colour(char c) {
                if (c != pc_.colour())  {
                    pc_.colour(c);
                    assert(region_ != nullptr);
                    region_->update_region(this);
                }
            }

            region_t* region() const  { return region_; }
            void set_region(region_t* r)    {
                if (r != nullptr)   region_ = r;
            }
            void join_region(region_t* r)    {
                region_->set_region(region_t::INVALID_REGION);
                region_ = r;
                region_->add_cell(this);
            }

            private:
                point_cell_t<T> pc_;
                region_t* region_;
        };

    template <typename T>
    bool operator != (const point_cell_t<T>& a, const point_cell_t<T>& b)  { return !(a == b); }

    template <typename T>
    bool operator != (const region_cell_t<T>& a, const region_cell_t<T>& b)  { return !(a == b); }

    typedef point_cell_t<std::uint32_t> icell_t;
    typedef region_cell_t<std::uint32_t> rcell_t;


    extern void set_cell_attr(rcell_t& cell, uint32_t id, char colour, uint32_t row, uint32_t col);

    class board_t   {
        public:
            typedef std::vector<std::unique_ptr<rcell_t> > rcells_t;
            typedef std::vector<std::unique_ptr<region_t> > regions_t;
            typedef std::vector<region_t*> regions_ptr_t;

            board_t(const int rows = 0, const int cols = 0);
            board_t(const board_t&);
            board_t& operator = (const board_t&);

            void init(const int rows = 0, const int cols = 0);
            void reset();
            
            void cell(uint32_t r, uint32_t c, uint32_t v);
            rcell_t* cell(uint32_t r, uint32_t c) const;

            bool is_wall(rcell_t*) const;

            
            void draw(std::ostream& = std::cout, const std::string delim = " ") const;
            
            uint32_t rows() const { return rows_; }
            uint32_t cols() const { return cols_; }

            void update_regions();
            void update_region(rcell_t*);

            regions_ptr_t   regions() const;


            class iterator  {
                public:
                    typedef iterator        this_type;
                    typedef rcell_t*        value_type;
                    typedef std::ptrdiff_t  difference_type;
                    typedef rcell_t**       pointer;
                    typedef rcell_t&        reference;
                    typedef struct random_access_iterator_tag iterator_category;

                    iterator(board_t* board = nullptr)
                    : board_(board),
                    cur_itr_()    {
                        if (board)  {
                            cur_itr_    = (board->cells_).begin();
                        }
                    }

                    iterator(board_t* board, rcells_t::iterator itr)
                        : board_(board),
                        cur_itr_(itr)   {}

                    rcell_t* operator * ()  {
                        return cur_itr_->get();
                    }

                    rcell_t* operator -> () {
                        return cur_itr_->get();
                    }

                    this_type operator ++ ()    {
                        this_type t = *this;
                        ++cur_itr_;
                        return t;
                    }

                    this_type operator ++ (int)    {
                        ++cur_itr_;
                        return *this;
                    }

                    bool operator == (this_type other)  {
                        return cur_itr_ == other.cur_itr_;
                    }

                    bool operator != (this_type other)  {
                        return cur_itr_ != other.cur_itr_;
                    }

                private:
                    board_t* board_;
                    rcells_t::iterator cur_itr_;

            };

            iterator begin();
            iterator end();

        private:

            uint32_t rows_;
            uint32_t cols_;
            rcells_t cells_;
            regions_t regions_;
    };

    template<typename T>
        region_cell_t<T>* up(region_cell_t<T>* cell, const board_t& board)    {
            if (cell != nullptr)    {
                auto r  = cell->row() - 1;
                auto c  = cell->col();
                if(is_inside_board(board, r, c)) return board.cell(r, c);
                else return nullptr;
            }
            else return nullptr;
        }

    template<typename T>
        region_cell_t<T>* down(region_cell_t<T>* cell, const board_t& board)    {
            if (cell != nullptr)    {
                auto r  = cell->row() + 1;
                auto c  = cell->col();
                if(is_inside_board(board, r, c)) return board.cell(r, c);
                else return nullptr;
            }
            else return nullptr;
        }

    template<typename T>
        region_cell_t<T>* left(region_cell_t<T>* cell, const board_t& board)    {
            if (cell != nullptr)    {
                auto r  = cell->row();
                auto c  = cell->col() - 1;
                if(is_inside_board(board, r, c)) return board.cell(r, c);
                else return nullptr;
            }
            else return nullptr;
        }

    template<typename T>
        region_cell_t<T>* right(region_cell_t<T>* cell, const board_t& board)    {
            if (cell != nullptr)    {
                auto r  = cell->row();
                auto c  = cell->col() + 1;
                if(is_inside_board(board, r, c)) return board.cell(r, c);
                else return nullptr;
            }
            else return nullptr;
        }

    extern bool is_inside_board(const board_t& board, const uint32_t r, const uint32_t c);

    extern void relate_2_rcells(rcell_t* a, rcell_t* b);
}
#endif
