#include <SFML/Graphics.hpp>

#include <vector>
#include <string>
#include <iostream>
#include <boost/lexical_cast.hpp>

using namespace std;

int main()
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");


    // define Nurikabe board
    const size_t CELLS  = 25;
    std::vector<sf::RectangleShape> board(CELLS, sf::RectangleShape(sf::Vector2f(100, 100)));  // 5 * 5
    auto row  = 0;
    auto col  = -1;
    for(auto i = 0; i < CELLS; ++i)    {
        
        if(i%5 == 0)    {
            col += 1;
            row = 0;
        }

        sf::RectangleShape r(sf::Vector2f(100, 100));
        r.setPosition(10 + 100*row, 10 + 100*col);
        r.setOutlineColor(sf::Color(0, 0, 0));
        r.setOutlineThickness(2);   // pixels
        board[i]    = r;

        ++row;
    }
    cout << board.size() << endl;

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with black color
        window.clear(sf::Color::Blue);

        // draw everything here...
        for(auto& cell : board)    {
            window.draw(cell);
        }

        // end the current frame
        window.display();
    }

    return 0;
}
