#pragma once

#include <SFML/Graphics.hpp>


class Game
{
public:
    Game();
    Game(int windowWidth, int windowHeight, int frameRate);
    ~Game();

    void run();

private:
    void initWindow();
    void initGameObjects();

    void processEvents();
    void update();
    void render();

    void updatePaddles();
    void updateBall();
    void checkCollisions();

    sf::RenderWindow window;
    int frameRate;

    sf::RectangleShape leftPaddle;
    sf::RectangleShape rightPaddle;
    sf::CircleShape ball;

    bool isPlaying;

    const float paddleSpeed = 5.0f;
    const float ballSpeed = 4.0f;
    sf::Vector2f ballDirection;
};
