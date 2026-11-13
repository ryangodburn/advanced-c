/*
* main.cpp - A test program for the Player class.
 * Author: Ryan Godburn & Arjun Aravind
 * Date: 1/31/25
 * Description: This program tests the Player class by writing results
 *              to a file and printing them to the console.
 */

// Libraries
#include "dice.hpp"
#include "player.hpp"
#include "enums.hpp"

// Function to test the Dice class
void unitDice() {

    // Open test file
    ofstream out("diceTest.txt", ios::app); // Open file in append mode
    if(!out) {
        fatal("Cannot open test file"); // Handle file open error
    }

    cout << "---------- Starting Unit Dice Test - Godburn & Aravind ----------\n";
    out << "---------- Starting Unit Dice Test - Godburn & Aravind ----------\n";

    // Test 1: Default constructor
    cout << "Test 1: Default constructor\n";
    out << "Test 1: Default constructor\n";
    Dice d1;               // Create a Dice object with default constructor
    d1.roll();             // Roll the dice
    cout << "Single die roll: " << d1 << "\n";
    out << "Single die roll: " << d1 << "\n";

    // Test 2: Multiple dice with repeated rolls
    cout << "\nTest 2: Multiple dice\n";
    out << "\nTest 2: Multiple dice\n";
    Dice d2(5);            // Create a Dice object with 5 dice
    for(int i = 0; i < 10; i++) {
        d2.roll();         // Roll the dice
        cout << "Roll " << i + 1 << ": " << d2 << "\n";
        out << "Roll " << i + 1 << ": " << d2 << "\n";
    }

    // Test 3: Range verification with large set
    cout << "\nTest 3: Range verification\n";
    out << "\nTest 3: Range verification\n";
    Dice d3(100);          // Create a Dice object with 100 dice
    d3.roll();             // Roll the dice
    cout << "100 dice roll: " << d3 << "\n";
    out << "100 dice roll: " << d3 << "\n";

    cout << "---------- End Unit Dice Test ----------\n";
    out << "---------- End Unit Dice Test ----------\n";

    // Close the test file
    out.close();
}

// Function to test the Player class
void unitPlayer() {

    // Open test file
    ofstream out("playerTest.txt", ios::app); // Open file in append mode
    if(!out) {
        fatal("Cannot open test file"); // Handle file open error
    }

    cout << "---------- Starting Unit Player Test - Godburn & Aravind ----------\n";
    out << "---------- Starting Unit Player Test - Godburn & Aravind ----------\n";

    // Test player creation and basic operations
    player wolf("RedWolf", ECcolor::blue);
    player ruby("RubyMan", ECcolor::green);

    cout << "Initial state:\n";
    out << "Initial state:\n";
    cout << wolf << endl;
    out << wolf << endl;
    cout << ruby << endl;
    out << ruby << endl;

    // Test winning columns
    cout << "\nWinning columns:\n";
    out << "\nWinning columns:\n";
    wolf.wonColumn(3);
    cout << wolf << endl;
    out << wolf << endl;

    wolf.wonColumn(1);
    wolf.wonColumn(4);
    cout << wolf << " (should have won)\n";
    out << wolf << " (should have won)\n";

    // Test color access demonstrating enum output
    cout << "\nPlayer colors:\n";
    out << "\nPlayer colors:\n";
    cout << "Wolf " << wolf.color() << endl;  // Uses the operator<< for ECcolor
    out << "Wolf " << wolf.color() << endl;
    cout << "Ruby " << ruby.color() << endl;  // Uses the operator<< for ECcolor
    out << "Ruby " << ruby.color() << endl;

    cout << "---------- End Unit Player Test ----------\n";
    out << "---------- End Unit Player Test ----------\n";

    // Close the test file
    out.close();

}

// Main function
int main() {
    srand(time(NULL));
    banner();
    unitPlayer();
    bye();
    return 0;
}