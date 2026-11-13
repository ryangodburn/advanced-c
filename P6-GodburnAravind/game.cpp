/*
* game.cpp - Implementation file for the Game class
 * Author: Ryan Godburn & Arjun Aravind
 * Date: 3/13/25
 * Description: This source file implements the Game class methods
 *              defined in game.hpp.
 */

#include "game.hpp"

// Constructor
Game::Game() {
    // Create game components using smart pointers
    dice = make_unique<Dice>(4);  // Create 4 dice

    // Initialize player
    cout << "Enter data for Player:\n";
    curPlayer = make_unique<player>(getNewPlayer());
}

// Get player info
player Game::getNewPlayer() {
    string name;
    int color;

    // Get valid player name
    for(;;) {
        cout << "Enter player name (no spaces): ";
        cin >> name;

        if(name.length() > 0 && name.length() <= 20) break;
        cout << "Invalid name or too long. Try again.\n";
    }

    // Get valid color selection
    for(;;) {
        cout << "Enter color:\n"
             << "1 - Orange\n"
             << "2 - Yellow\n"
             << "3 - Green\n"
             << "4 - Blue\n"
             << "Choice: ";

        if(cin >> color && color >= 1 && color <= 4) break;
        // Clear error state
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid color. Try again.\n";
    }

    return player(name, ECcolor(color));
}

// Execute one turn of the game
void Game::oneTurn() {
    // Start the turn on the board using the current player
    board.startTurn(curPlayer.get());

    // Menu loop
    bool turnOver = false;
    while(!turnOver) {
        cout << "\n-- Turn Options --\n"
             << "1. Roll\n"
             << "2. Stop\n"
             << "3. Resign\n"
             << "Choice: ";

        int choice;
        cin >> choice;

        switch(choice) {
            case 1: // Roll
                turnOver = handleRoll();
                break;

            case 2: // Stop
                turnOver = handleStop();
                break;

            case 3: // Resign
                turnOver = handleResign();
                break;

            default:
                cout << "Invalid choice. Please select 1, 2, or 3.\n";
                // Clear input stream in case of invalid input
                cin.clear();
                cin.ignore(10000, '\n');
        }
    }
}

// Roll dice and display values
const int* Game::rollAndDisplayDice() {
    const int* dvals = (*dice).roll();
    cout << "Dice: A=" << dvals[0] << ", B=" << dvals[1]
         << ", C=" << dvals[2] << ", D="
         << dvals[3] << endl;
    return dvals;
}

// Get valid dice pair selection from user
bool Game::getDicePairSelection(string& pairInput) {
    cout << "Select two letters for first pair (e.g., AB): ";
    cin >> pairInput;

    // Validate input
    if(pairInput.length() != 2) {
        cout << "Invalid input. Please enter exactly two letters.\n";
        return false;
    }

    // Convert to uppercase
    pairInput[0] = toupper(pairInput[0]);
    pairInput[1] = toupper(pairInput[1]);

    // Validate letters
    if(pairInput[0] < 'A' || pairInput[0] > 'D' ||
       pairInput[1] < 'A' || pairInput[1] > 'D' ||
       pairInput[0] == pairInput[1]) {
        cout << "Invalid dice selection. Please use A, B, C, or D "
                "without repeating.\n";
        return false;
    }

    return true;
}

// Calculate column values from dice pairs
void Game::calculatePairValues(const int* dvals, string pairInput,
                               int& pair1, int& pair2) {
    // Map letters to dice indices
    int Idx1 = pairInput[0] - 'A';
    int Idx2 = pairInput[1] - 'A';

    // Calculate first pair total
    pair1 = dvals[Idx1] + dvals[Idx2];

    // Find the remaining two dice for second pair
    int Idx3 = -1, Idx4 = -1;
    for(int Idx = 0; Idx < 4; Idx++) {
        if(Idx != Idx1 && Idx != Idx2) {
            if(Idx3 == -1)
                Idx3 = Idx;
            else
                Idx4 = Idx;
        }
    }
    pair2 = dvals[Idx3] + dvals[Idx4];

    cout << "First pair: " << pair1 << ", Second pair: " << pair2 << endl;
}

// Make moves to columns and check for bust
bool Game::makeMovesAndCheckBust(int pair1, int pair2) {
    // Validate column values before calling move
    bool move1Valid = (pair1 >= 2 && pair1 <= 12);
    bool move2Valid = (pair2 >= 2 && pair2 <= 12);

    // Try moves
    bool move1Success = move1Valid && board.move(pair1);
    cout << "Move to column " << pair1 << ": "
         << (move1Success ? "Success" : "Failed") << endl;

    bool move2Success = move2Valid && board.move(pair2);
    cout << "Move to column " << pair2 << ": "
         << (move2Success ? "Success" : "Failed") << endl;

    // Display board
    cout << board << endl;

    // Check for bust
    if(!move1Success && !move2Success) {
        cout << "*** BUST! ***\n";
        board.bust();
        return true; // Return true to indicate the turn is over
    }

    return false; // Return false to indicate the turn is not over yet
}

// Handle roll action
bool Game::handleRoll() {
    // Roll dice and display values
    const int* dvals = rollAndDisplayDice();

    // Get valid dice pair selection
    string pairInput;
    if(!getDicePairSelection(pairInput)) return false;

    // Calculate pair values
    int pair1, pair2;
    calculatePairValues(dvals, pairInput, pair1, pair2);

    // Make moves and check for bust
    return makeMovesAndCheckBust(pair1, pair2);
}

// Handle stop action
bool Game::handleStop() {
    board.stop();
    cout << "Turn ended, towers converted to tiles:\n" << board << endl;
    return true; // Return true to indicate the turn is over
}

// Handle resign action
bool Game::handleResign() {
    cout << "Resign not implemented yet.\n";
    return false; // Return false to indicate the turn is not over yet
}