/*
* game.cpp - Implementation file for the Game class
 * Author: Ryan Godburn & Arjun Aravind
 * Date: 2/12/25
 * Description: This source file implements the Game class methods
 *              defined in game.hpp.
 */

#include "game.hpp"

// Constructor
Game::Game() : col1(2), col2(7),
               p1("temp", ECcolor::white),
               p2("temp", ECcolor::white) {

    dice = new Dice(4);    // Create 4 dice

    cout << "Enter data for Player 1:\n";
    p1 = getNewPlayer();
    cout << "Enter data for Player 2:\n";
    p2 = getNewPlayer();
}

// Destructor
Game::~Game() {
    delete dice;           // Free dice memory
}

// Get player info from keyboard with robust input handling
player Game::getNewPlayer() {
    string name;
    string input;
    int color;
    bool validInput = false;

    // Get and validate player name
    do {
        cout << "Enter player name (no spaces): ";
        if(getline(cin >> ws, name)) {  // ws removes leading whitespace
            // Check for spaces in name
            if(name.find(' ') != string::npos) {
                cout << "Error: Name cannot contain spaces\n";
                continue;
            }
            // Check name length
            if(name.empty()) {
                cout << "Error: Name cannot be empty\n";
                continue;
            }
            if(name.length() > 20) {  // reasonable max length
                cout << "Error: Name too long (max 20 chars)\n";
                continue;
            }
            validInput = true;
        } else {
            cout << "Invalid input. Please try again.\n";
        }
    } while(!validInput);

    // Get and validate color choice
    validInput = false;
    do {
        cout << "Enter color:\n"
             << "0 - White\n"
             << "1 - Orange\n"
             << "2 - Yellow\n"
             << "3 - Green\n"
             << "4 - Blue\n"
             << "Choice: ";

        if(getline(cin >> ws, input)) {
            try {
                color = stoi(input);
                if(color >= 0 && color <= 4) {
                    validInput = true;
                } else {
                    cout << "Error: Color must be between 0 and 4\n";
                }
            } catch(...) {
                cout << "Invalid input. Please enter a number.\n";
            }
        } else {
            cout << "Invalid input. Please try again.\n";
        }
    } while(!validInput);

    return player(name, ECcolor(color));
}