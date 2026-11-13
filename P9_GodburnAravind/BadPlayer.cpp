/*
* BadPlayer.cpp - Implementation for exception class hierarchy
 * Author: Ryan Godburn & Arjun Aravind
 * Date: 4/4/25
 * Description: This source file implements the BadPlayer and BadChoice
 *              exception hierarchies for handling player and
 *              gameplay errors.
 */

#include "BadPlayer.hpp"

// ------------------------------------------------------------------

// BadPlayer implementation
BadPlayer::BadPlayer(string n, ECcolor c) : name(n), color(c) {}

void BadPlayer::basePrint(ostream& out) const {
    out << "Input: Name = " << name << ", Color = " << color << endl;
}

void BadPlayer::print(ostream& out) const {
    out << "Error: Both name and color are already in use!" << endl;
    basePrint(out);
}

// ------------------------------------------------------------------


// BadName implementation
BadName::BadName(string n, ECcolor c) : BadPlayer(n, c) {}

void BadName::print(ostream& out) const {
    out << "Error: The name '" << name << "' is already in use!" << endl;
    basePrint(out);
}

// BadColor implementation
BadColor::BadColor(string n, ECcolor c) : BadPlayer(n, c) {}

void BadColor::print(ostream& out) const {
    out << "Error: The color " << color << " is already in use!" << endl;
    basePrint(out);
}

// ------------------------------------------------------------------

// BadChoice implementation
BadChoice::BadChoice(string i) : input(i) {}

void BadChoice::basePrint(ostream& out) const {
    out << "Invalid input: " << input << endl;
}

void BadChoice::print(ostream& out) const {
    out << "Error: Invalid choice!" << endl;
    basePrint(out);
}

// ------------------------------------------------------------------

// BadSlot implementation
BadSlot::BadSlot(string i) : BadChoice(i) {}

void BadSlot::print(ostream& out) const {
    out << "Error: Invalid dice selection!" << endl;
    basePrint(out);
}

// ------------------------------------------------------------------

// DuplicateSlot implementation
DuplicateSlot::DuplicateSlot(string i) : BadChoice(i) {}

void DuplicateSlot::print(ostream& out) const {
    out << "Error: Cannot select the same die twice!" << endl;
    basePrint(out);
}