#include "functions.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Snake Game"); // Create the window
    window.setFramerateLimit(10); // Set the frame rate to 10 FPS (for snake speed)

    while (window.isOpen()) {
        int choice = menu(window); // Show the menu

        if (choice == 0) {
            // Play the game
            Setup(); // Initialize the game setup
            while (!gameOver && window.isOpen()) {
                Input(window);
                Logic();
                Draw(window);
            }
        }
        else if (choice == 1) {
            // Display Highscore (You can extend this to save/load highscore)
            sf::Text highscoreText;
            highscoreText.setString("Highscore: 0"); // Placeholder, implement as needed
            highscoreText.setCharacterSize(24);
            highscoreText.setFillColor(sf::Color::Black);
            highscoreText.setPosition(350, 250);

            while (window.isOpen()) {
                sf::Event event;
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed) {
                        window.close();
                    }
                }

                window.clear(sf::Color::White);
                window.draw(highscoreText);
                window.display();

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) break; // Press Enter to return
            }
        }
        else if (choice == 2) {
            // Exit the game
            window.close();
        }
    }

    return 0;
}
