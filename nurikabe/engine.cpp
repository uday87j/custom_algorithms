#include "engine.h"

#include <SFML/Graphics.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>

using namespace std;
using namespace sf;

namespace ne    {

    cell_t::cell_t(std::uint8_t n)
        :num_(n) {
            if(n != numeric_limits<uint8_t>::max()) {
                Font font;
                if (!font.loadFromFile("FreeMono.ttf")) std::cerr << "\nFont is invalid\n";

                text_.setFont(font);
                text_.setString("Hello");
                //text_.setString(boost::lexical_cast<string>(n));
                text_.setCharacterSize(12);
                text_.setColor(Color::White);
            }
        }

    //void cell_t::set_position(sf::Vector2f pos) {
    //    text_.setPosition(pos);
    //}

    board_t::board_t(std::uint8_t nrows, std::uint8_t ncols)
    : num_cells_(nrows*ncols),
    board_(num_cells_, sf::RectangleShape(sf::Vector2f(100, 100))),
    cells_(num_cells_, 7)    {
        auto row  = 0;
        auto col  = -1;
        for(auto i = 0; i < num_cells_; ++i)    {
            
            if(i%5 == 0)    {
                col += 1;
                row = 0;
            }

            sf::RectangleShape r(sf::Vector2f(100, 100));
            r.setPosition(10 + 100*row, 10 + 100*col);
            r.setOutlineColor(sf::Color(0, 0, 0));
            r.setOutlineThickness(2);   // pixels
            board_[i]    = r;

            cells_[i].set_position(sf::Vector2f(10 + 100*row, 10 + 100*col));

            ++row;
        }

       
    }

    void board_t::draw(sf::RenderWindow& w)   {
        for(auto i = 0; i < num_cells_; ++i)    {
            auto text   = cells_[i].get_numer_text();
            //text.setPosition(board_[i].getPosition());
            w.draw(board_[i]);
            w.draw(text);   // FIXME: Causes segfault
        }
    }

    void board_t::handle_events(sf::Event& e, sf::RenderWindow& w)  {
        sf::Vector2i localPosition = sf::Mouse::getPosition(w);
        localPosition   -= sf::Vector2i(10, 10);
    }

    class engine_t::engine_impl_t {
        typedef engine_impl_t this_type;
    };

    engine_t::engine_t(sf::RenderWindow* w)
    : window_(w),
    board_(ROWS, COLS) {
    }

    engine_t::~engine_t()   {;}

    void engine_t::draw(sf::RenderWindow& w)   {
        board_.draw(w);
    }

    void engine_t::handle_events(sf::Event& e)  {
        //if (e.type == sf::Event::Closed)  // FIXME: Causes segfault
        //    window_->close(); 
        if (e.type == sf::Event::MouseButtonPressed)    {
            board_.handle_events(e, *window_);
        }
    }
}
