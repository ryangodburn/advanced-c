/*
* game.hpp - Header file for the Game class
 * Author: Ryan Godburn & Arjun Aravind
 * Date: 4/1/25
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
    unique_ptr<Dice> dice;          // Polymorphic dice object
                                    // (Dice, CSDice, or FakeDice)
    Board board;                    // Game board
    CList players;                  // List of players
    player* curPlayer;              // Current player pointer
    bool gameOver;                  // Flag to track if game is over

    // Player creation
    unique_ptr<player> getNewPlayer();

    // Turn actions
    bool handleRoll();
    bool handleStop();
    bool handleResign();

    // Helper functions for roll action
    const int* rollAndDisplayDice();
    bool getDicePairSelection(string& pairInput);
    void calculatePairValues(const int* dvals, string pairInput,
                             int& pair1, int& pair2);
    bool makeMovesAndCheckBust(int pair1, int pair2);

    // Game control
    void nextPlayer();              // Move to next player

    // Separate gameplay methods
    void playGameWithFakeDice();    // Automated gameplay
    void playGameWithCSDice();      // Interactive gameplay

public:
    Game();
    ~Game() = default;              // Smart pointers handle cleanup

    // Main game function
    void playGame();
};

#endif