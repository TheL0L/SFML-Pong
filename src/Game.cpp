#include "Game.h"
#include <cmath>

int getRandomDirection(std::mt19937& randomEngine) {
    std::uniform_int_distribution<int> dist(0, 1);
    return dist(randomEngine) == 0 ? -1 : 1;
}

int getRandomStrength(std::mt19937& randomEngine, float max) {
    std::uniform_real_distribution<float> dist(0, max);
    return dist(randomEngine);
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
    else if (magnitude < ballSpeed * 0.9f)
    {
        float scale = ballSpeed * 0.9f / magnitude;

        ballDirection.x *= scale;
        ballDirection.y *= scale;
    }
}


Game::Game(int windowWidth = 800, int windowHeight = 600, int frameRate = 60) :
    window(sf::VideoMode(windowWidth, windowHeight), "Pong"),
    frameRate(frameRate), isPlaying(true), roundStarting(true),
    randomEngine(std::random_device{}()), difficultyIntervalDist(5, 15)
{
    initWindow();
    loadSounds();
    initGameObjects();
    initScores();
    initCountdown();

    nextDifficultyIncrease = difficultyIntervalDist(randomEngine);
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

    if (!font.loadFromFile("assets/Font/RobotoMono-Bold.ttf"))
    {
        backgroundColor = sf::Color(80, 0, 0, 255);
    }
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
    ballDirection = sf::Vector2f(getRandomDirection(randomEngine) * ballSpeed, getRandomDirection(randomEngine) * ballSpeed);

    clearBallTrail();
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
    if (roundStarting)
    {
        int elapsedSeconds = static_cast<int>(roundStartClock.getElapsedTime().asSeconds());

        if (elapsedSeconds < 3)
            countdownText.setString(std::to_string(3 - elapsedSeconds));
        else roundStarting = false;
    }
    else
    {
        updateBall();
        checkCollisions();
        updateDifficulty();
    }
}

void Game::render()
{
    window.clear(backgroundColor);

    renderBallTrail();

    window.draw(leftPaddle);
    window.draw(rightPaddle);
    window.draw(ball);

    window.draw(leftScoreText);
    window.draw(rightScoreText);

    if (roundStarting)
        window.draw(countdownText);

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
    updateBallTrail();
    ball.move(ballDirection);

    if (ball.getPosition().y < 0 || ball.getPosition().y > window.getSize().y - 2 * ball.getRadius())
        ballDirection.y *= -1;

    if (ball.getPosition().x < 0)
    {
        rightScore += 1;
        rightScoreText.setString(std::to_string(rightScore));
        initGameObjects();
        roundStarting = true;
        roundStartClock.restart();
        ballSpeed = 3.0f;
        goalSound.play();
    }
    else if (ball.getPosition().x > window.getSize().x - 2 * ball.getRadius())
    {
        leftScore += 1;
        leftScoreText.setString(std::to_string(leftScore));
        initGameObjects();
        roundStarting = true;
        roundStartClock.restart();
        ballSpeed = 3.0f;
        goalSound.play();
    }
}

void Game::checkCollisions()
{
    sf::FloatRect ballBounds = ball.getGlobalBounds();
    sf::FloatRect leftPaddleBounds = leftPaddle.getGlobalBounds();
    sf::FloatRect rightPaddleBounds = rightPaddle.getGlobalBounds();

    if (ballBounds.intersects(leftPaddleBounds)) {
        ballDirection.x = std::abs(ballDirection.x);
        ballDirection.y += getRandomDirection(randomEngine) * getRandomStrength(randomEngine, 2.0f);
        enforceBallSpeedLimit(ballDirection, ballSpeed);
        paddleHitSound.play();
    }

    if (ballBounds.intersects(rightPaddleBounds)) {
        ballDirection.x = -std::abs(ballDirection.x);
        ballDirection.y += getRandomDirection(randomEngine) * getRandomStrength(randomEngine, 2.0f);
        enforceBallSpeedLimit(ballDirection, ballSpeed);
        paddleHitSound.play();
    }
}

void Game::initScores()
{
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

void Game::initCountdown()
{
    countdownText.setFont(font);
    countdownText.setCharacterSize(40);
    countdownText.setFillColor(sf::Color::White);
    countdownText.setPosition(window.getSize().x / 2, 60);
    countdownText.setString("3");
}

void Game::updateBallTrail()
{
    sf::CircleShape trailPoint(ball.getRadius());
    trailPoint.setPosition(ball.getPosition());
    sf::Color trailColor = sf::Color(200, 0, 0, 180);
    trailPoint.setFillColor(trailColor);
    ballTrail.push_back(trailPoint);

    for (auto& point : ballTrail) {
        // reduce size
        point.setRadius(point.getRadius() * trailFactor);

        // fade color
        sf::Color color = point.getFillColor();
        if (color.a > 0) {
            color.a = static_cast<sf::Uint8>(color.a * trailFactor);
            point.setFillColor(color);
        }
    }

    // remove points that have become too small or invisible
    ballTrail.erase(
        std::remove_if(ballTrail.begin(), ballTrail.end(),
            [](const sf::CircleShape& point) {
                return point.getRadius() < 1.0f || point.getFillColor().a == 0;
            }
        ), ballTrail.end()
    );
}

void Game::renderBallTrail()
{
    for (const auto& point : ballTrail) window.draw(point);
}

void Game::clearBallTrail()
{
    ballTrail.clear();
}

void Game::updateDifficulty()
{
    if (difficultyClock.getElapsedTime().asSeconds() >= nextDifficultyIncrease) {
        ballSpeed *= 1.25f;
        enforceBallSpeedLimit(ballDirection, ballSpeed);

        difficultyClock.restart();
        nextDifficultyIncrease = difficultyIntervalDist(randomEngine);
    }
}

void Game::loadSounds()
{
    if (!paddleHitBuffer.loadFromFile("assets/Sounds/bounce.wav"))
        backgroundColor = sf::Color(80, 0, 0, 255);
    if (!goalBuffer.loadFromFile("assets/Sounds/goal.wav"))
        backgroundColor = sf::Color(80, 0, 0, 255);

    paddleHitSound.setBuffer(paddleHitBuffer);
    goalSound.setBuffer(goalBuffer);
}
