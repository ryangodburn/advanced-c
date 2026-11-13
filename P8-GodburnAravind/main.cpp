/*
* main.cpp - Main program for Can't Stop game
 * Author: Ryan Godburn & Arjun Aravind
 * Date: 4/1/25
 * Description: This program creates and launches a full
 *              game of Can't Stop.
 */

// Libraries
#include "game.hpp"

// Main function
int main() {
    srand(time(NULL));
    banner();

    // Create game instance and play game
    Game game;
    game.playGame();

    bye();
    return 0;
}