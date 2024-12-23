#pragma once

#include <SFML/Graphics.hpp>


extern bool gameOver;   // main flag for game, if true -> game ends

void Setup();       // function prototypes
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
