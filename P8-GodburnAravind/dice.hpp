/*
* dice.hpp - Header file for the Dice class hierarchy.
 * Author: Ryan Godburn & Arjun Aravind
 * Date: 4/1/25
 * The class simulates a set of dice with derived classes for
 * the Can't Stop game and for testing with predetermined values.
 */

#ifndef DICE_HPP
#define DICE_HPP

#pragma once

#include "tools.hpp"
#include <fstream>

// Define file name for fake dice rolls
#define FAKE_DICE_FILE "fake_dice.txt"
// Define move codes for fake dice
#define FAKE_ROLL "R"
#define FAKE_STOP "S"

class Dice {
protected:
    int nDice;     // Number of dice
    int* dvals;    // Pointer to dynamically allocate array of dice values
public:
    Dice(int n = 1);
    virtual ~Dice();
    ostream& print(ostream&);
    virtual const int* roll();
};

// Overloaded stream insertion operator to print the Dice object
inline ostream& operator<<(ostream& os, Dice& d) {
    return d.print(os);
}

// CantStopDice class - derived from Dice
class CSDice : public Dice {
protected:
    int pairs[2];  // Array to store the two dice pair totals
public:
    CSDice();
    virtual ~CSDice() = default;
    const int* roll() override;
};

// FakeDice class - derived from CSDice for testing
class FakeDice : public CSDice {
private:
    ifstream infile;  // Input file stream for fake dice rolls
    string moveCode;  // Store the roll/stop code from the file
public:
    FakeDice();
    virtual ~FakeDice();
    const int* roll() override;

    // Accessor for the move code
    string getMoveCode() const { return moveCode; }
};

#endif