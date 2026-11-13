/*
* main.cpp - A test program for the Game & Board class.
 * Author: Ryan Godburn & Arjun Aravind
 * Date: 3/13/25
 * Description: This program tests the Game class by playing
 *              a single turn
 */

// Libraries
#include "game.hpp"

// Main function
int main() {
    srand(time(NULL));
    banner();

    // Create game instance and play one turn
    Game game;
    game.oneTurn();

    bye();
    return 0;
}