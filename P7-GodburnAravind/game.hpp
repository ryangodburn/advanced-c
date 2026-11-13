/*
* game.hpp - Header file for the Game class
 * Author: Ryan Godburn & Arjun Aravind
 * Date: 3/13/25
 * Description: This header defines the Game class which acts as the
 *              controller for the Can't Stop application.
 */

#ifndef GAME_HPP
#define GAME_HPP

#pragma once

#include "dice.hpp"
#include "player.hpp"
#include "board.hpp"
#include "clist.hpp"
#include <memory>

class Game {
private:
    unique_ptr<Dice> dice;    // Set of 4 dice
    Board board;              // Game board
    unique_ptr<player> curPlayer; // Current player

    // Function to get player data
    player getNewPlayer();

    // Helper functions for turn actions
    bool handleRoll();
    bool handleStop();
    bool handleResign();

    // Helper functions for roll action
    const int* rollAndDisplayDice();
    bool getDicePairSelection(string& pairInput);
    void calculatePairValues(const int* dvals, string pairInput,
                             int& pair1, int& pair2);
    bool makeMovesAndCheckBust(int pair1, int pair2);

public:
    Game();
    ~Game() = default;  // Smart pointers handle cleanup

    // Game turn function
    void oneTurn();

    // Unit test for CList class
    void testCList();
};

#endif