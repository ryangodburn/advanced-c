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
#include "board.hpp"
#include <memory>

class Game {
private:
    unique_ptr<Dice> dice;   // Set of 4 dice
    unique_ptr<Board> board; // Game board
    player player1;               // Single player for testing

    // Function to get player data
    player getNewPlayer();

public:
    Game();
    ~Game() = default;  // Smart pointers handle cleanup

    // Getters for testing
    Dice* getDice() const { return dice.get(); }
    Board* getBoard() const { return board.get(); }
    player* getPlayer() { return &player1; }
};

#endif