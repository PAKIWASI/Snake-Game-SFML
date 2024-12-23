#include "functions.h"
#include <fstream>
#include <algorithm>
#include <iostream>

bool gameOver;
const int block = 20;
const int height = 30;
const int width = 40;
int x, y, ntail, score;
int Xtail[100], Ytail[100] = { 0 };
int fruitX, fruitY;

enum eDirection { STOP, UP, DOWN, RIGHT, LEFT };
eDirection dir;

sf::Texture snakeHeadTexture, snakeBodyTexture, fruitTexture, backgroundTexture;

// High score management
std::vector<int> highScores;

void Setup() {
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

    // Load textures
    if (!snakeHeadTexture.loadFromFile("snake-png/snake_head.png")) {
        std::cout << "Failed to load snake head texture!" << std::endl;
    }
    if (!snakeBodyTexture.loadFromFile("snake-png/snake_body.png")) {
        std::cout << "Failed to load snake body texture!" << std::endl;
    }
    if (!fruitTexture.loadFromFile("snake-png/apple.png")) {
        std::cout << "Failed to load fruit texture!" << std::endl;
    }
    if (!backgroundTexture.loadFromFile("snake-png/grass.png")) {
        std::cout << "Failed to load background texture!" << std::endl;
    }

    LoadHighScores();
}

void LoadHighScores() {
    std::ifstream file("highscores.txt");
    if (file.is_open()) {
        int score;
        while (file >> score) {
            highScores.push_back(score);
        }
        file.close();
    }
    std::sort(highScores.rbegin(), highScores.rend());  // Sort in descending order
}

void SaveHighScores() {
    std::ofstream file("highscores.txt");
    if (file.is_open()) {
        for (int score : highScores) {
            file << score << std::endl;
        }
        file.close();
    }
}

void UpdateHighScores(int newScore) {
    // Load existing high scores
    LoadHighScores();

    // Add the new score to the high scores
    highScores.push_back(newScore);

    // Sort the high scores in descending order
    std::sort(highScores.begin(), highScores.end(), std::greater<int>());

    // Keep only the top 5 scores
    if (highScores.size() > 5) {
        highScores.resize(5);
    }

    // Save the updated high scores
    SaveHighScores();
}

void DisplayHighScores(sf::RenderWindow& window) {
    // Load high scores from file
    LoadHighScores();

    // Load font
    sf::Font font;
    if (!font.loadFromFile("snake-png/arial.ttf")) {
        std::cout << "Failed to load font!" << std::endl;
        return;
    }

    // Create the highscore title text
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("High Scores");
    titleText.setCharacterSize(40);
    titleText.setFillColor(sf::Color::Green);
    titleText.setPosition(window.getSize().x / 2 - titleText.getLocalBounds().width / 2, 50);

    // Create the list of high scores text
    std::vector<sf::Text> scoreTexts;
    for (int i = 0; i < 5; i++) {
        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setString(std::to_string(i + 1) + ". " + std::to_string(highScores[i]));
        scoreText.setCharacterSize(30);
        scoreText.setFillColor(sf::Color::Black);
        scoreText.setPosition(window.getSize().x / 2 - scoreText.getLocalBounds().width / 2, 120 + i * 40);
        scoreTexts.push_back(scoreText);
    }

    // Display the highscore screen
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();  // Close the window if the close button is pressed
        }

        window.clear(sf::Color::White); // Clear previous content

        // Draw the title and high scores
        window.draw(titleText);
        for (const auto& scoreText : scoreTexts) {
            window.draw(scoreText);
        }

        window.display();

        // Wait for the user to press Enter to go back to the menu
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            break; // Exit the highscore screen
        }
    }
}

void randFruit() {
    fruitX = rand() % width;
    fruitY = rand() % height;
    for (int i = 0; i < ntail; i++) {
        if (Xtail[i] == fruitX && Ytail[i] == fruitY) {
            fruitX = rand() % width;
            fruitY = rand() % height;
        }
    }
}

void Draw(sf::RenderWindow& window) {
    window.clear();

    // Draw the background
    sf::Sprite background(backgroundTexture);
    window.draw(background);

    // Draw the snake's head
    sf::Sprite head(snakeHeadTexture);
    head.setPosition(x * block, y * block);
    window.draw(head);

    // Draw the snake's body
    for (int i = 0; i < ntail; i++) {
        sf::Sprite tail(snakeBodyTexture);
        tail.setPosition(Xtail[i] * block, Ytail[i] * block);
        window.draw(tail);
    }

    // Draw the fruit
    sf::Sprite fruit(fruitTexture);
    fruit.setPosition(fruitX * block, fruitY * block);
    window.draw(fruit);

    // Draw the score
    sf::Font font;
    font.loadFromFile("snake-png/arial.ttf");
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString("Score: " + std::to_string(score));
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);
    window.draw(scoreText);

    if (gameOver) {
        UpdateHighScores(score);  // Update the high score when game over
        // Save score to high scores if game over
        highScores.push_back(score);
        std::sort(highScores.rbegin(), highScores.rend());  // Sort in descending order
        SaveHighScores();

        // Display the "GAME OVER!" message
        sf::Text gameOverText;
        gameOverText.setFont(font);
        gameOverText.setString("GAME OVER!");
        gameOverText.setCharacterSize(40);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setPosition(window.getSize().x / 2 - gameOverText.getLocalBounds().width / 2, window.getSize().y / 2 - 20);
        window.draw(gameOverText);

        // Display the score at the center of the screen
        sf::Text finalScoreText;
        finalScoreText.setFont(font);
        finalScoreText.setString("Your Score: " + std::to_string(score));
        finalScoreText.setCharacterSize(30);
        finalScoreText.setFillColor(sf::Color::Black);
        finalScoreText.setPosition(window.getSize().x / 2 - finalScoreText.getLocalBounds().width / 2, window.getSize().y / 2 + 40);
        window.draw(finalScoreText);

        // Prompt to restart or quit
        sf::Text restartText;
        restartText.setFont(font);
        restartText.setString("Press R to Restart or ESC to Quit");
        restartText.setCharacterSize(20);
        restartText.setFillColor(sf::Color::Black);
        restartText.setPosition(window.getSize().x / 2 - restartText.getLocalBounds().width / 2, window.getSize().y / 2 + 80);
        window.draw(restartText);
    }

    window.display();
}


void Input(sf::RenderWindow& window) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && dir != DOWN) dir = UP;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && dir != UP) dir = DOWN;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && dir != LEFT) dir = RIGHT;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && dir != RIGHT) dir = LEFT;
}

void Logic() {
    updateTailPos();
    updateHeadPos();
    isFruitEaten();
    isTailHit();
    boundry();
}

void updateTailPos() {
    if (dir != STOP) {
        int prevX = Xtail[0];
        int prevY = Ytail[0];
        Xtail[0] = x;
        Ytail[0] = y;
        int prev2X, prev2Y;
        for (int i = 1; i < ntail; i++) {
            prev2X = Xtail[i];
            prev2Y = Ytail[i];
            Xtail[i] = prevX;
            Ytail[i] = prevY;
            prevX = prev2X;
            prevY = prev2Y;
        }
    }
}

void updateHeadPos() {
    switch (dir) {
    case UP: y--; break;
    case DOWN: y++; break;
    case RIGHT: x++; break;
    case LEFT: x--; break;
    case STOP: break;
    }
}

void isFruitEaten() {
    if (x == fruitX && y == fruitY) {
        score += 10;
        ntail++;
        randFruit();
    }
}

void isTailHit() {
    for (int i = 0; i < ntail; i++) {
        if (x == Xtail[i] && y == Ytail[i])
            gameOver = true;
    }
}

void boundry() {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        gameOver = true;
    }
}

void RestartGame() {
    // Reset all game variables
    x = width / 2;
    y = height / 2;
    dir = STOP;
    score = 0;
    ntail = 1;
    gameOver = false;
    randFruit();
    Xtail[0] = x - 1;
    Ytail[0] = y;
}

int menu(sf::RenderWindow& window) {
    const char* options[] = { "Play", "Highscore", "Exit" };
    int selectedOption = 0;
    int numOptions = 3;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);

        sf::Font font;
        font.loadFromFile("snake-png/arial.ttf");
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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            if (selectedOption == 1) {
                DisplayHighScores(window);
            }
            return selectedOption;
        }
    }
    return -1;
}



