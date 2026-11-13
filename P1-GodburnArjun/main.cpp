/*
* main.cpp - A test program for the Dice class.
 * Author: Ryan Godburn & Arjun Aravind
 * Date: 1/24/25
 * Description: This program tests the Dice class by writing results
 *              to a file and printing them to the console.
 */

// Libraries
#include "Dice.hpp"
#include "tools.hpp"
#include <fstream>    

// Function to test the Dice class
void unitDice() {
    // Open test file
    ofstream out("diceTest.txt", ios::app); // Open file in append mode
    if(!out) {
        fatal("Cannot open test file"); // Handle file open error
    }

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

    // Close the test file
    out.close();
}

// Main function
int main() {
    banner();        // Display start message
    unitDice();      // Run the unit test for Dice class
    bye();           // Display end message
    return 0;
}
