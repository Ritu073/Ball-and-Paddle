#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>

using namespace sf;
using namespace std;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const float PADDLE_WIDTH = 150;
const float PADDLE_HEIGHT = 120;
const float BALL_RADIUS = 50;

int main() {
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Catch the Falling Ball");
    window.setFramerateLimit(60);

    // Load background texture
    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("graphics/background.png")) {
        cout << "Failed to load background texture!" << endl;
        return -1;
    }
    Sprite background;
    background.setTexture(backgroundTexture);
    background.setScale(
        (float)WINDOW_WIDTH / backgroundTexture.getSize().x,
        (float)WINDOW_HEIGHT / backgroundTexture.getSize().y
    );

    // Load paddle texture
    Texture paddleTexture;
    if (!paddleTexture.loadFromFile("graphics/paddle.png")) {
        cout << "Failed to load paddle texture!" << endl;
        return -1;
    }

    // Load ball texture
    Texture ballTexture;
    if (!ballTexture.loadFromFile("graphics/ball.png")) {
        cout << "Failed to load ball texture!" << endl;
        return -1;
    }

    // Load font for text display
    Font font;
    if (!font.loadFromFile("fonts/ByteBounce.ttf")) {
        cout << "Failed to load font!" << endl;
        return -1;
    }

    // Score setup
    int score = 0;
    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(50);
    scoreText.setFillColor(Color::Black);
    scoreText.setPosition(10, 10);

    // Game Over text
    Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(60);
    gameOverText.setFillColor(Color::Black);
    gameOverText.setString("Game Over!");
    gameOverText.setPosition(WINDOW_WIDTH / 2 - 150, WINDOW_HEIGHT / 2 - 30);

    // Start game text
    Text startText;
    startText.setFont(font);
    startText.setCharacterSize(40);
    startText.setFillColor(Color::Black);
    startText.setString("Start the Game");
    startText.setPosition(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 30);

    // Time bar setup
    RectangleShape timeBar(Vector2f(WINDOW_WIDTH, 10));
    timeBar.setFillColor(Color::Red);
    timeBar.setPosition(0, WINDOW_HEIGHT - 10);
    float timeRemaining = 15.0f;
    Clock clock;

    // Paddle setup
    Sprite paddle;
    paddle.setTexture(paddleTexture);
    paddle.setScale(PADDLE_WIDTH / paddleTexture.getSize().x, PADDLE_HEIGHT / paddleTexture.getSize().y);
    paddle.setPosition(WINDOW_WIDTH / 2 - PADDLE_WIDTH / 2, WINDOW_HEIGHT - PADDLE_HEIGHT - 10);

    // Ball setup
    Sprite ball;
    ball.setTexture(ballTexture);
    ball.setScale((BALL_RADIUS * 2) / ballTexture.getSize().x, (BALL_RADIUS * 2) / ballTexture.getSize().y);
    ball.setPosition(rand() % (WINDOW_WIDTH - (int)BALL_RADIUS * 5), 0);

    float paddleSpeed = 5.0f;
    float ballSpeed = 3.0f;
    bool gameOver = false;
    bool gameStarted = false;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (!gameStarted && event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                gameStarted = true;
                clock.restart();
            }

            if (gameOver && event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
                // Restart game
                gameOver = false;
                score = 0;
                timeRemaining = 20.0f;
                ball.setPosition(rand() % (WINDOW_WIDTH - (int)BALL_RADIUS * 2), 0);
                paddle.setPosition(WINDOW_WIDTH / 2 - PADDLE_WIDTH / 2, WINDOW_HEIGHT - PADDLE_HEIGHT - 10);
                gameStarted = false;
            }
        }

        if (gameStarted && !gameOver) {
            // Paddle movement
            if (Keyboard::isKeyPressed(Keyboard::Left) && paddle.getPosition().x > 0)
                paddle.move(-paddleSpeed, 0);
            if (Keyboard::isKeyPressed(Keyboard::Right) && paddle.getPosition().x < WINDOW_WIDTH - PADDLE_WIDTH)
                paddle.move(paddleSpeed, 0);

            // Ball movement
            ball.move(0, ballSpeed);
            if (ball.getPosition().y + BALL_RADIUS * 2 > WINDOW_HEIGHT) {
                timeRemaining -= 3.0f;
                ball.setPosition(rand() % (WINDOW_WIDTH - (int)BALL_RADIUS * 2), 0);
            }

            // Collision detection
            if (ball.getGlobalBounds().intersects(paddle.getGlobalBounds())) {
                ball.setPosition(rand() % (WINDOW_WIDTH - (int)BALL_RADIUS * 2), 0);
                score++;
                timeRemaining += 1.0f;
            }

            // Update score text
            stringstream ss;
            ss << "Score: " << score;
            scoreText.setString(ss.str());

            // Update time bar
            float elapsedTime = clock.getElapsedTime().asSeconds();
            timeRemaining -= elapsedTime;
            clock.restart();
            if (timeRemaining <= 0) {
                gameOver = true;
            }
            timeBar.setSize(Vector2f((timeRemaining / 15.0f) * WINDOW_WIDTH, 10));
        }

        // Render
        window.clear();
        window.draw(background);
        if (!gameStarted) {
            window.draw(startText);
        } else if (gameOver) {
            window.draw(gameOverText);
        } else {
            window.draw(paddle);
            window.draw(ball);
            window.draw(scoreText);
            window.draw(timeBar);
        }
        window.display();
    }

    return 0;
}
