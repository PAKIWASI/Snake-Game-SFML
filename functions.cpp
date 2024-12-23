#include "functions.h"
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>

bool gameOver;
const int block = 20;
const int height = 30;    // dimensions of game area
const int width = 40;

int x, y, ntail, score;  // pos of snake, tail pos, no of tails and score
int Xtail[100], Ytail[100] = { 0 };  // arrays for storing x, y coords of tails (at index 0 of both-> x,y of first tail)
int fruitX, fruitY;

enum eDirection { STOP, UP, DOWN, RIGHT, LEFT, };  // all possible states and directions of snake
eDirection dir;

void Setup()   // set initial conditions of the game
{
    gameOver = false;
    x = width / 2;
    y = height / 2;
    dir = STOP;
    score = 0;
    srand(static_cast<unsigned int>(time(0)));
    randFruit();
    Xtail[0] = x - 1;
    Ytail[0] = y;
    ntail = 1;
}

void randFruit()
{
    fruitX = rand() % width;
    fruitY = rand() % height;
    for (int i = 0; i < ntail; i++) {
        if (Xtail[i] == fruitX && Ytail[i] == fruitY) {
            fruitX = rand() % width;
            fruitY = rand() % height;
        }
    }
}

void Draw(sf::RenderWindow& window)
{
    window.clear(sf::Color::Green);

    // Draw the snake's head
    sf::RectangleShape head(sf::Vector2f(block, block));
    head.setFillColor(sf::Color(139, 69, 19)); // DarkBrown
    head.setPosition(x * block, y * block);
    window.draw(head);

    // Draw the snake's tail
    for (int i = 0; i < ntail; i++) {
        sf::RectangleShape tail(sf::Vector2f(block, block));
        tail.setFillColor(sf::Color(139, 69, 19)); // Brown
        tail.setPosition(Xtail[i] * block, Ytail[i] * block);
        window.draw(tail);
    }

    // Draw the fruit
    sf::RectangleShape fruit(sf::Vector2f(block, block));
    fruit.setFillColor(sf::Color::Red);
    fruit.setPosition(fruitX * block, fruitY * block);
    window.draw(fruit);

    // Draw the score
    sf::Font font;
    font.loadFromFile("arial.ttf");
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Score: " + std::to_string(score));
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);
    window.draw(scoreText);

    if (gameOver) {
        sf::Text gameOverText;
        gameOverText.setFont(font);
        gameOverText.setString("GAME OVER!");
        gameOverText.setCharacterSize(40);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setPosition(window.getSize().x / 2 - gameOverText.getLocalBounds().width / 2, window.getSize().y / 2 - 20);
        window.draw(gameOverText);

        sf::Text restartText;
        restartText.setFont(font);
        restartText.setString("Press R to Restart");
        restartText.setCharacterSize(20);
        restartText.setFillColor(sf::Color::Black);
        restartText.setPosition(window.getSize().x / 2 - restartText.getLocalBounds().width / 2, window.getSize().y / 2 + 40);
        window.draw(restartText);
    }

    window.display();
}

void Input(sf::RenderWindow& window)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && dir != DOWN) dir = UP;    // Move up
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && dir != UP) dir = DOWN;    // Move down
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && dir != LEFT) dir = RIGHT; // Move right
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && dir != RIGHT) dir = LEFT; // Move left
}

void Logic()
{
    updateTailPos();
    updateHeadPos();
    isFruitEaten();
    isTailHit();
    boundry();
}

void updateTailPos()
{
    if (dir != STOP) {              // tail[0] -> first tail after head
        int prevX = Xtail[0];  // prevXY is temp var to transfer coords
        int prevY = Ytail[0];  // prevXY stores the 1st tail coords
        Xtail[0] = x;           // coords of head are given to 1st tail (indicating forward movement)
        Ytail[0] = y;
        int prev2X, prev2Y;     // temp var to store ith tail coords
        for (int i = 1; i < ntail; i++) {   // go through all the tails in array
            prev2X = Xtail[i];
            prev2Y = Ytail[i];       // ith tail coords stored in prev2XY 
            Xtail[i] = prevX;        // ith tail coords updated to the coords of the tail in front of it
            Ytail[i] = prevY;
            prevX = prev2X;          // prevX is given ith coord for next iteration
            prevY = prev2Y;
        }
    }
}

void updateHeadPos()
{
    switch (dir) {              // increase head-cord according to direction
    case UP: y--;  break;
    case DOWN: y++; break;
    case RIGHT: x++; break;
    case LEFT: x--; break;
    case STOP: break;
    }
}

void isFruitEaten()
{
    if (x == fruitX && y == fruitY) {   // eat fruit logic
        score += 10;
        ntail++;
        randFruit();
    }
}

void isTailHit()
{
    for (int i = 0; i < ntail; i++) {            // hit tail logic
        if (x == Xtail[i] && y == Ytail[i])
            gameOver = true;
    }
}

void boundry()
{
    if (x < 0 || x >= width || y < 0 || y >= height) {
        gameOver = true;
    }
}

int menu(sf::RenderWindow& window)
{
    const char* options[] = { "Play", "Highscore", "Exit" };
    int selectedOption = 0; // Tracks the current selected option
    int numOptions = 3;     // Total number of menu options

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);

        sf::Font font;
        font.loadFromFile("arial.ttf");
        sf::Text titleText;
        titleText.setFont(font);
        titleText.setString("Snake Game");
        titleText.setCharacterSize(40);
        titleText.setFillColor(sf::Color::Green);
        titleText.setPosition(window.getSize().x / 2 - titleText.getLocalBounds().width / 2, 50);
        window.draw(titleText);

        for (int i = 0; i < numOptions; i++) {
            sf::Text optionText;
            optionText.setFont(font);
            optionText.setString(options[i]);
            optionText.setCharacterSize(20);
            optionText.setFillColor(i == selectedOption ? sf::Color::Green : sf::Color::Black);
            optionText.setPosition(window.getSize().x / 2 - optionText.getLocalBounds().width / 2, 150 + i * 50);
            window.draw(optionText);
        }

        window.display();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) selectedOption = (selectedOption + 1) % numOptions;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) selectedOption = (selectedOption - 1 + numOptions) % numOptions;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) return selectedOption;
    }

    return -1; // Return -1 if the window is closed
}
