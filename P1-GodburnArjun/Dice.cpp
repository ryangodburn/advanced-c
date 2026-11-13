/*
* Dice.cpp - Implementation file for the Dice class.
 * Author: Ryan Godburn & Arjun Aravind
 * Date: 1/24/25
 * Description: This source file implements the methods defined in Dice.hpp.
 *              It includes the constructor, destructor, roll method, and print 
 *              method for simulating and printing the dice values.
 */

// Libraries
#include "Dice.hpp"
#include <ctime>
#include <cstdlib>

// Constructor
Dice::Dice(const int n) {
    nDice = n;           // Initialize number of dice
    dvals = new int[nDice]; // Dynamically allocate memory for dice values
    srand(time(NULL));   // Seed the random number generator
}

// Destructor
Dice::~Dice() {
    delete[] dvals;  // Release allocated memory
}

// Roll dice
const int* Dice::roll() {
    for(int i = 0; i < nDice; i++) {
        dvals[i] = rand() % 6 + 1; // Generate random value between 1 and 6
    }
    return dvals; // Return array of dice values
}

// Print values
ostream& Dice::print(ostream& os) {
    for(int i = 0; i < nDice; i++) {
        os << dvals[i];    // Print each dice value
        if(i < nDice - 1)  // Print space between values except last value
            os << " ";
    }
    return os; // Return ostream object
}
