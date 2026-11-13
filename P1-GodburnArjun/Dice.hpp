/*
* Dice.hpp - A header file defining the Dice class and its methods.
 * Author: Ryan Godburn & Arjun Aravind
 * Date: 1/24/25
 * Description: This header file defines a Die class, which simulates the rolling 
 *              of dice. It includes methods for rolling the dice and printing 
 * 		their values.
 */

#ifndef DICE_HPP
#define DICE_HPP

// Libraries
#include <iostream>

using namespace std;

// Definition of the Dice class
class Dice {
private:
    int nDice;     // Number of dice
    int* dvals;    // Pointer to dynamically allocate array holding dice values
public:
    // Constructor to initialize the Dice object
    // Default parameter initializes with 1 die
    Dice(int n = 1);
    // Destructor to release allocated memory
    ~Dice();
    // Method to print the dice values
    ostream& print(ostream&);
    // Method to roll the dice and return the values
    const int* roll();
};

#endif

// Overloaded stream insertion operator to print the Dice object
inline ostream& operator<<(ostream& os, Dice& d) {
    return d.print(os);
}
