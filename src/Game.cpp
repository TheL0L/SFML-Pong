#include "Game.h"

Game::Game() : window(sf::VideoMode(800, 600), "Pong"),
    frameRate(60), isPlaying(true)
{
    initWindow();
    initGameObjects();
}

Game::Game(int windowWidth, int windowHeight, int frameRate) :
    window(sf::VideoMode(windowWidth, windowHeight), "Pong"),
    frameRate(frameRate), isPlaying(true)
{
    initWindow();
    initGameObjects();
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
    ballDirection = sf::Vector2f(ballSpeed, ballSpeed);
}

void Game::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed) window.close();
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
    window.clear();

    window.draw(leftPaddle);
    window.draw(rightPaddle);
    window.draw(ball);

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

}

void Game::checkCollisions()
{

}

