/*
* game.hpp - Header file for the Game class
 * Author: Ryan Godburn & Arjun Aravind
 * Date: 4/12/25
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
#include "BadPlayer.hpp"
#include "enums.hpp"
#include <memory>
#include <cstring>

class Game {
private:
    Board board;                  // Game board
    CList players;                // List of players
    Dice* dice;                   // Pointer to dynamically allocated dice
    const int* pairVals;          // Current pair-values of the dice
    ECgame gameState;             // Game state (begun, done, quit)

    // Player creation
    void getPlayers();
    unique_ptr<player> getNewPlayer();
    bool checkData(string name, ECcolor color);
    string promptForPlayerName();
    ECcolor promptForPlayerColor();

    // Turn validation functions
    int validateTurnChoice();
    bool validateResignConfirmation();
    bool getDicePairSelection(string& pairInput);

    // Turn actions
    ECgame oneTurn(player* p);
    ECgame handleFakeDiceTurn(player* p);
    ECgame handleInteractiveTurn(player* p);
    ECgame processPlayerChoice(player* p, int choice, bool& turnOver);
    bool handleRoll();
    bool validateDiceValues(int pair1, int pair2);
    bool handleStop(player* p);
    bool handleResign();
    ECgame handleAfterRollResult(player* p, bool busted, bool& turnOver);
    ECgame processFakeDiceResult(player* p, bool busted, bool& turnOver);
    ECgame processFakeDiceStop(player* p);
    ECgame processFakeDiceContinue();
    ECgame processFakeDiceBust(bool& turnOver);
    ECgame processFakeDiceResign(player* p);

    // Helper functions for roll action
    const int* rollAndDisplayDice();
    void calculatePairValues(const int* dvals, string pairInput,
                           int& pair1, int& pair2);
    bool makeMovesAndCheckBust(int pair1, int pair2);
    ECgame checkWinCondition(player* p);
    void handleChoiceError(BadChoice& e);

    // Game control
    void playInit(player*& curPlayer);
    bool checkNoPlayersLeft();
    void handlePlayerTurn(player*& curPlayer);
    void announceWinner(player* winner);

public:
    Game();
    ~Game();
    void play();
};

#endif