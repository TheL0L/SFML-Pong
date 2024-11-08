#include "Game.h"
#include <random>
#include <cmath>

int getRandomDirection() {
    static std::random_device rd;  // Seed for randomness
    static std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<int> dist(0, 1);

    return dist(gen) == 0 ? -1 : 1;
}

int getRandomStrength(float max) {
    static std::random_device rd;  // Seed for randomness
    static std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<float> dist(0, max);

    return dist(gen);
}

void enforceBallSpeedLimit(sf::Vector2f& ballDirection, float ballSpeed)
{
    float magnitude = std::sqrt(ballDirection.x * ballDirection.x + ballDirection.y * ballDirection.y);

    if (magnitude > ballSpeed)
    {
        float scale = ballSpeed / magnitude;

        ballDirection.x *= scale;
        ballDirection.y *= scale;
    }
}


Game::Game() : window(sf::VideoMode(800, 600), "Pong"),
    frameRate(60), isPlaying(true)
{
    initWindow();
    initGameObjects();
    initScores();
}

Game::Game(int windowWidth, int windowHeight, int frameRate) :
    window(sf::VideoMode(windowWidth, windowHeight), "Pong"),
    frameRate(frameRate), isPlaying(true)
{
    initWindow();
    initGameObjects();
    initScores();
}

Game::~Game() {}

void Game::run()
{
    while (window.isOpen())
    {
        processEvents();
        if (isPlaying) update();
        render();
    }
}

void Game::initWindow()
{
    window.setFramerateLimit(this->frameRate);
    backgroundColor = sf::Color::Black;
}

void Game::initGameObjects()
{
    leftPaddle.setSize(sf::Vector2f(paddleWidth, paddleHeight));
    leftPaddle.setPosition(50, window.getSize().y / 2 - leftPaddle.getSize().y / 2);
    leftPaddle.setFillColor(sf::Color::Green);

    rightPaddle.setSize(sf::Vector2f(paddleWidth, paddleHeight));
    rightPaddle.setPosition(window.getSize().x - 50, window.getSize().y / 2 - rightPaddle.getSize().y / 2);
    rightPaddle.setFillColor(sf::Color::Green);

    ball.setRadius(ballRadius);
    ball.setPosition(window.getSize().x / 2 - ball.getRadius(), window.getSize().y / 2 - ball.getRadius());
    ball.setFillColor(sf::Color::Red);
    ballDirection = sf::Vector2f(getRandomDirection() * ballSpeed, getRandomDirection() * ballSpeed);
}

void Game::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed) window.close();

        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
        {
            this->isPlaying = !this->isPlaying;
            backgroundColor = this->isPlaying ? sf::Color::Black : sf::Color(0, 0, 80, 255);
        }
    }
}

void Game::update()
{
    updatePaddles();
    updateBall();
    checkCollisions();
}

void Game::render()
{
    window.clear(backgroundColor);

    window.draw(leftPaddle);
    window.draw(rightPaddle);
    window.draw(ball);

    window.draw(leftScoreText);
    window.draw(rightScoreText);

    window.display();
}

void Game::updatePaddles()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        leftPaddle.move(0, -paddleSpeed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        leftPaddle.move(0, paddleSpeed);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        rightPaddle.move(0, -paddleSpeed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        rightPaddle.move(0, paddleSpeed);

    
    if (leftPaddle.getPosition().y < 0)
        leftPaddle.setPosition(leftPaddle.getPosition().x, 0);
    if (leftPaddle.getPosition().y > window.getSize().y - leftPaddle.getSize().y)
        leftPaddle.setPosition(leftPaddle.getPosition().x, window.getSize().y - leftPaddle.getSize().y);
    
    if (rightPaddle.getPosition().y < 0)
        rightPaddle.setPosition(rightPaddle.getPosition().x, 0);
    if (rightPaddle.getPosition().y > window.getSize().y - rightPaddle.getSize().y)
        rightPaddle.setPosition(rightPaddle.getPosition().x, window.getSize().y - rightPaddle.getSize().y);
}

void Game::updateBall()
{
    ball.move(ballDirection);

    if (ball.getPosition().y < 0 || ball.getPosition().y > window.getSize().y - 2 * ball.getRadius())
        ballDirection.y *= -1;

    if (ball.getPosition().x < 0)
    {
        rightScore += 1;
        rightScoreText.setString(std::to_string(rightScore));
        initGameObjects();
    }
    else if (ball.getPosition().x > window.getSize().x - 2 * ball.getRadius())
    {
        leftScore += 1;
        leftScoreText.setString(std::to_string(leftScore));
        initGameObjects();
    }
}

void Game::checkCollisions()
{
    sf::FloatRect ballBounds = ball.getGlobalBounds();
    sf::FloatRect leftPaddleBounds = leftPaddle.getGlobalBounds();
    sf::FloatRect rightPaddleBounds = rightPaddle.getGlobalBounds();

    if (ballBounds.intersects(leftPaddleBounds)) {
        ballDirection.x = std::abs(ballDirection.x);
        ballDirection.y += getRandomDirection() * getRandomStrength(2.0f);
        enforceBallSpeedLimit(ballDirection, ballSpeed);
    }

    if (ballBounds.intersects(rightPaddleBounds)) {
        ballDirection.x = -std::abs(ballDirection.x);
        ballDirection.y += getRandomDirection() * getRandomStrength(2.0f);
        enforceBallSpeedLimit(ballDirection, ballSpeed);
    }
}

void Game::initScores()
{
    if (!font.loadFromFile("assets/Font/RobotoMono-Bold.ttf"))
    {
        backgroundColor = sf::Color(80, 0, 0, 255);
        return;
    }

    const float windowHalfWidth = window.getSize().x / 2;

    leftScoreText.setFont(font);
    leftScoreText.setCharacterSize(24);
    leftScoreText.setFillColor(sf::Color::White);
    leftScoreText.setPosition(windowHalfWidth - windowHalfWidth / 2, 20);

    rightScoreText.setFont(font);
    rightScoreText.setCharacterSize(24);
    rightScoreText.setFillColor(sf::Color::White);
    rightScoreText.setPosition(windowHalfWidth + windowHalfWidth / 2, 20);

    leftScoreText.setString(std::to_string(leftScore));
    rightScoreText.setString(std::to_string(rightScore));
}
