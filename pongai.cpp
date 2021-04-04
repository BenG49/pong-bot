#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include "pong.hpp"

PlayerMoveState stupidLeft(float l, float r, float ballX, float ballY, sf::Vector2u size)
{
    if (l < ballY)
        return DOWN;
    if (l > ballY)
        return UP;

    return NONE;
}
PlayerMoveState stupidRight(float l, float r, float ballX, float ballY, sf::Vector2u size)
{
    if (r < ballY)
        return DOWN;
    if (r > ballY)
        return UP;

    return NONE;
}

int main()
{
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(440, 280), "pong", sf::Style::Titlebar);
    sf::Vector2u size = window.getSize();
    sf::View view(sf::FloatRect(-(size.x / 2.f), -(size.y / 2.f), size.x, size.y));

    window.setView(view);

    Game *game = new Game();
    setGame(game, size, 0, 0);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        updateGame(game, &stupidLeft, &stupidRight);

        window.clear();

        if (game->state == PLAY)
            drawGame(&window, game);
        else if (game->state == L_WIN)
        {
        }
        else if (game->state == R_WIN)
        {
        }

        window.display();
    }
}