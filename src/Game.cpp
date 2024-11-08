#include "Game.h"

Game::Game() : window(sf::VideoMode(800, 600), "Pong"),
    frameRate(60), isPlaying(true), ballDirection(0.5f, 0.5f)
{
    initWindow();
    initGameObjects();
}

Game::Game(int windowWidth, int windowHeight, int frameRate) :
    window(sf::VideoMode(windowWidth, windowHeight), "Pong"),
    frameRate(frameRate), isPlaying(true), ballDirection(0.5f, 0.5f)
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
    leftPaddle.setSize(sf::Vector2f(10, 100));
    leftPaddle.setPosition(50, window.getSize().y / 2 - leftPaddle.getSize().y / 2);

    rightPaddle.setSize(sf::Vector2f(10, 100));
    rightPaddle.setPosition(window.getSize().x - 50, window.getSize().y / 2 - rightPaddle.getSize().y / 2);

    ball.setRadius(10);
    ball.setPosition(window.getSize().x / 2 - ball.getRadius(), window.getSize().y / 2 - ball.getRadius());
}

void Game::processEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed) window.close();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        leftPaddle.move(0, -paddleSpeed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        leftPaddle.move(0, paddleSpeed);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        rightPaddle.move(0, -paddleSpeed);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        rightPaddle.move(0, paddleSpeed);
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

}

void Game::updateBall()
{

}

void Game::checkCollisions()
{

}

