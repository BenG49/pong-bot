#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <thread>
#include <chrono>
#include "pong.hpp"

PlayerMoveState stupid(float self, float other, float xDist, float ballY, Vector2u size)
{
    if (self < ballY)
        return DOWN;
    if (self > ballY)
        return UP;

    return NONE;
}

int main()
{
    srand(time(NULL));
    RenderWindow window(VideoMode(440, 280), "pong", Style::Titlebar);
    Vector2u size = window.getSize();
    View view(FloatRect(-(size.x / 2.f), -(size.y / 2.f), size.x, size.y));

    Font font;
    if (!font.loadFromFile("/usr/share/fonts/CascadiaMono.ttf"))
        throw;

    window.setView(view);

    Game *game = new Game();
    setGame(game, size, 0, 0);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
            if (event.type == Event::Closed)
                window.close();

        updateGame(game, &stupid, &stupid);

        window.clear();
        if (game->state == PLAY)
        {
            drawGame(&window, game, font);
            window.display();
        } else if (game->state == L_WIN || game->state == R_WIN)
        {
            Text text;
            text.setFont(font);
            if (game->state == L_WIN)
            {
                text.setString("Left Win!");
                setGame(game, game->size, game->lScore+1, game->rScore);
            } else if (game->state == R_WIN)
            {
                text.setString("Right Win!");
                setGame(game, game->size, game->lScore, game->rScore+1);
            }
            text.setCharacterSize(24);

            window.draw(text);
            window.display();

            this_thread::sleep_for (chrono::seconds(1));

        }

    }
}