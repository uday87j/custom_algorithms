#ifndef BOARD_H_
#define BOARD_H_

#include <iostream>
#include <cassert>
#include <vector>
#include <memory>

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
    bool operator != (const point_cell_t<T>& a, const point_cell_t<T>& b)  { return !(a == b); }

    typedef point_cell_t<std::uint32_t> icell_t;


    extern void set_cell_attr(icell_t& cell, uint32_t id, char colour);

    class board_t   {
        public:
            typedef std::vector<std::unique_ptr<icell_t> > icells_t;

            board_t(const int rows = 0, const int cols = 0);
            
            void cell(uint32_t r, uint32_t c, uint32_t v);
            icell_t* cell(uint32_t r, uint32_t c) const;
            bool is_wall(icell_t*);
            
            void draw(std::ostream& = std::cout, const std::string delim = " ");
            
            uint32_t rows() const { return rows_; }
            uint32_t cols() const { return cols_; }

            icell_t* begin();
            icell_t* next();
            icell_t* end();
        private:
            uint32_t rows_;
            uint32_t cols_;
            icells_t cells_;
            icells_t::iterator cur_itr_;
    };

    template<typename T>
        point_cell_t<T>* up(point_cell_t<T>* cell, const board_t& board)    {
            auto r  = cell->row() - 1;
            auto c  = cell->col();
            if(is_inside_board(board, r, c)) return board.cell(r, c);
            else return cell;
        }

    template<typename T>
        point_cell_t<T>* down(point_cell_t<T>* cell, const board_t& board)    {
            auto r  = cell->row() + 1;
            auto c  = cell->col();
            if(is_inside_board(board, r, c)) return board.cell(r, c);
            else return cell;
        }

    template<typename T>
        point_cell_t<T>* left(point_cell_t<T>* cell, const board_t& board)    {
            auto r  = cell->row();
            auto c  = cell->col() - 1;
            if(is_inside_board(board, r, c)) return board.cell(r, c);
            else return cell;
        }

    template<typename T>
        point_cell_t<T>* right(point_cell_t<T>* cell, const board_t& board)    {
            auto r  = cell->row();
            auto c  = cell->col() + 1;
            if(is_inside_board(board, r, c)) return board.cell(r, c);
            else return cell;
        }

    extern bool is_inside_board(const board_t& board, const uint32_t r, const uint32_t c);
}
#endif
