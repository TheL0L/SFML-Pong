#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

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

    void initScores();
    void initCountdown();

    void updateBallTrail();
    void renderBallTrail();
    void clearBallTrail();

    sf::RenderWindow window;
    int frameRate;
    sf::Color backgroundColor;

    sf::RectangleShape leftPaddle;
    sf::RectangleShape rightPaddle;
    sf::CircleShape ball;

    bool isPlaying;
    bool roundStarting;

    const float paddleWidth = 15.0f;
    const float paddleHeight = 100.0f;
    const float ballRadius = 10.0f;

    const float paddleSpeed = 5.0f;
    const float ballSpeed = 3.0f;
    sf::Vector2f ballDirection;

    int leftScore = 0;
    int rightScore = 0;

    sf::Font font;
    sf::Text leftScoreText;
    sf::Text rightScoreText;

    sf::Clock roundStartClock;
    sf::Text countdownText;

    std::vector<sf::CircleShape> ballTrail;
    const float trailFactor = 0.98f;
};
