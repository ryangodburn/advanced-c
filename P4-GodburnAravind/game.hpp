/*
* game.hpp - Header file for the Game class
 * Author: Ryan Godburn & Arjun Aravind
 * Date: 2/12/25
 * Description: This header defines the Game class which acts as the
 *              controller for the Can't Stop application.
 */

#ifndef GAME_HPP
#define GAME_HPP

#pragma once

#include "dice.hpp"
#include "player.hpp"
#include "column.hpp"

class Game {
private:
    Dice* dice;       // Set of 4 dice
    player p1, p2;
    Column col1, col2;

    // Function to get player data
    player getNewPlayer();

public:
    Game();
    ~Game();

    // Getters for testing
    Dice* getDice() const { return dice; }
    player* getP1() { return &p1; }
    player* getP2() { return &p2; }
    Column& getCol1() { return col1; }
    Column& getCol2() { return col2; }
};

#endif