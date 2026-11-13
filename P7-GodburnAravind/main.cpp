/*
* main.cpp - A test program for the Game & Board class.
 * Author: Ryan Godburn & Arjun Aravind
 * Date: 3/13/25
 * Description: This program tests the Game class by playing
 *              a single turn and tests the CList class
 */

// Libraries
#include "game.hpp"

// Main function
int main() {
    srand(time(NULL));
    banner();

    // Create game instance
    Game game;

    // Test options
    cout << "\nSelect test to run:\n"
         << "1. Play one turn\n"
         << "2. Test CList implementation\n"
         << "Choice: ";

    int choice;
    cin >> choice;

    switch(choice) {
    case 1:
        // Play one turn
            game.oneTurn();
        break;

    case 2:
        // Run the CList unit test
            game.testCList();
        break;

    default:
        cout << "Invalid choice.\n";
    }

    bye();
    return 0;
}