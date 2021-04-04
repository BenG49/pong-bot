#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <cmath>

#include <iostream>

#define PLAY 0
#define L_WIN 2
#define R_WIN 3

#define LEFT 0
#define RIGHT 1

struct Game
{
    sf::Vector2u size;
    float lPos;
    float rPos;
    float ballX;
    float ballY;
    float ballSpeed;
    int ballAngle;
    int state;
    int lScore;
    int rScore;
};
enum PlayerMoveState
{
    UP,
    DOWN,
    NONE
};

const sf::Vector2f PADDLE_SIZE(10, 70);
const int PADDLE_WALL_GAP = 10;
const int MAX_ANGLE = 45;
const float PADDLE_BOUNCE = 1.1;
const float PADDLE_SPEED = 0.1;
const float START_SPEED = 0.1;
const float BALL_SIZE = 7.5;

double degToRad(int deg)
{
    return deg * (M_PI / 180);
}

void setGame(struct Game *game, sf::Vector2u size, int lScore, int rScore)
{
    game->size = size;
    game->lPos = 0;
    game->rPos = 0;
    game->ballX = 0;
    game->ballY = 0;
    game->ballSpeed = START_SPEED;
    int angle;
    while ((angle > MAX_ANGLE && angle < 180 - MAX_ANGLE) || (angle > 180 + MAX_ANGLE && angle < 360 - MAX_ANGLE))
        angle = rand() % 360;
    game->ballAngle = angle;
    game->state = PLAY;
    game->lScore = lScore;
    game->rScore = rScore;
}

bool withinPaddle(struct Game *game, int player, float ballY)
{
    float playerPos;
    if (player == LEFT)
        playerPos = game->lPos;
    else if (player == RIGHT)
        playerPos = game->rPos;

    float min = playerPos - PADDLE_SIZE.y / 2.f;
    float max = playerPos + PADDLE_SIZE.y / 2.f;

    return ballY >= min && ballY <= max;
}

void updateGame(struct Game *game,
                PlayerMoveState (*l)(float l, float r, float ballX, float ballY, sf::Vector2u size),
                PlayerMoveState (*r)(float l, float r, float ballX, float ballY, sf::Vector2u size))
{
    float newX = game->ballX + game->ballSpeed * std::cos(degToRad(game->ballAngle));
    float newY = game->ballY + game->ballSpeed * std::sin(degToRad(game->ballAngle));

    float rightWall = game->size.x / 2.f;

    // game end
    if (newX < -rightWall)
    {
        game->state = R_WIN;
        return;
    }
    if (newX > rightWall)
    {
        game->state = L_WIN;
        return;
    }

    float bottomWall = game->size.y / 2.f;

    // vertical borders
    if (newY < -bottomWall || newY + BALL_SIZE > bottomWall)
        // angle of 45 goes to 360 - 45
        game->ballAngle = 360 - game->ballAngle;

    // left paddle
    if (newX < -rightWall + PADDLE_WALL_GAP + PADDLE_SIZE.x && withinPaddle(game, LEFT, newY))
    {
        game->ballSpeed *= PADDLE_BOUNCE;
        game->ballAngle = (rand() % (180 - MAX_ANGLE * 2) + 270 + MAX_ANGLE) % 360;
    }
    // right paddle
    if (newX > rightWall - PADDLE_WALL_GAP - PADDLE_SIZE.x && withinPaddle(game, RIGHT, newY))
    {
        game->ballSpeed *= PADDLE_BOUNCE;
        game->ballAngle = rand() % (180 - MAX_ANGLE * 2) + 90 + MAX_ANGLE;
    }

    // update paddles
    PlayerMoveState left = l(game->lPos, game->rPos, game->ballX, game->ballY, game->size);
    PlayerMoveState right = r(game->lPos, game->rPos, game->ballX, game->ballY, game->size);

    if (left == UP && game->lPos - PADDLE_SIZE.y / 2.f > -bottomWall)
        game->lPos -= PADDLE_SPEED;
    else if (left == DOWN && game->lPos + PADDLE_SIZE.y / 2.f < bottomWall)
        game->lPos += PADDLE_SPEED;

    if (right == UP && game->rPos - PADDLE_SIZE.y / 2.f > -bottomWall)
        game->rPos -= PADDLE_SPEED;
    else if (right == DOWN && game->rPos + PADDLE_SIZE.y / 2.f < bottomWall)
        game->rPos += PADDLE_SPEED;

    game->ballX = newX;
    game->ballY = newY;
}

void drawGame(sf::RenderWindow *window, struct Game *game)
{
    sf::Vector2u size = window->getSize();

    // ball
    sf::CircleShape ball(BALL_SIZE);
    ball.setPosition(sf::Vector2f(game->ballX - ball.getRadius() / 2.f, game->ballY - ball.getRadius() / 2.f));
    window->draw(ball);

    // paddles
    sf::RectangleShape l(PADDLE_SIZE);
    sf::RectangleShape r(PADDLE_SIZE);

    l.setPosition(sf::Vector2f(
        -(size.x / 2.f) + PADDLE_WALL_GAP,
        game->lPos - PADDLE_SIZE.y / 2));
    r.setPosition(sf::Vector2f(
        size.x / 2.f - PADDLE_WALL_GAP - PADDLE_SIZE.x,
        game->rPos - PADDLE_SIZE.y / 2));

    window->draw(l);
    window->draw(r);

    // center line
    sf::RectangleShape line(sf::Vector2f(2, size.y));
    line.setFillColor(sf::Color(255, 255, 255, 100));
    line.setPosition(sf::Vector2f(line.getSize().x / 2.f, -(size.y / 2.f)));
    window->draw(line);
}