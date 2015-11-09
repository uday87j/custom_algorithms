#ifndef NURIKABE_ENGINE_H_
#define NURIKABE_ENGINE_H_

#include <SFML/Graphics.hpp>

#include <memory>
#include <cstddef>


namespace ne    {

    class cell_t    {
        public:
            cell_t(std::uint8_t n = std::numeric_limits<std::uint8_t>::max());

            bool is_numbered()  { return num_ != std::numeric_limits<std::uint8_t>::max(); }

            void set_position(sf::Vector2f pos) { text_.setPosition(pos); }

            sf::Text get_numer_text()   { return text_; }
        private:
            std::uint8_t num_;
            sf::Text text_;
    };


    class board_t   {

        public:
            board_t(std::uint8_t nrows, std::uint8_t ncols);

            void draw(sf::RenderWindow& w);

            void handle_events(sf::Event&, sf::RenderWindow&);

        private:
            std::uint32_t num_cells_;
            std::vector<sf::RectangleShape> board_;
            std::vector<cell_t> cells_;

    };


    class engine_t  {
        
        public:
            engine_t(sf::RenderWindow* w = nullptr);
            ~engine_t();
            
            void draw(sf::RenderWindow&);

            void handle_events(sf::Event&);

            const size_t ROWS  = 5;
            const size_t COLS  = 5;
            const size_t CELLS = ROWS*COLS;
        
        private:
            class engine_impl_t;

            sf::RenderWindow* window_;

            board_t board_;

            std::unique_ptr<engine_impl_t> impl_;
    };
}
#endif
