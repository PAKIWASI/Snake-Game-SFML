#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>


extern bool gameOver;


void Setup();
void randFruit();
void Draw(sf::RenderWindow& window);
void Input(sf::RenderWindow& window);
void RestartGame();         
void Logic();
void updateTailPos();
void updateHeadPos();
void isFruitEaten();
void isTailHit();
void boundry();
int menu(sf::RenderWindow& window);

void LoadHighScores();
void SaveHighScores();
void UpdateHighScores(int newScore);
void DisplayHighScores(sf::RenderWindow& window);

