/*
* BadPlayer.hpp - Header file for exception class hierarchy
 * Author: Ryan Godburn & Arjun Aravind
 * Date: 4/4/25
 * Description: This header defines the BadPlayer and BadChoice
 *              exception hierarchies for handling player and
 *              gameplay errors.
 */

#ifndef BADPLAYER_HPP
#define BADPLAYER_HPP

#pragma once

#include "tools.hpp"
#include "enums.hpp"

// ------------------------------------------------------------------

// BadPlayer exception class
class BadPlayer {
protected:
    string name;    // Name input by user
    ECcolor color;  // Color chosen by user

public:
    BadPlayer(string n, ECcolor c);
    virtual ~BadPlayer() = default;
    void basePrint(ostream& out) const;
    virtual void print(ostream& out) const;
};

// ------------------------------------------------------------------

// BadName exception class
class BadName : public BadPlayer {
public:
    BadName(string n, ECcolor c);
    void print(ostream& out) const override;
};

// ------------------------------------------------------------------

// BadColor exception class
class BadColor : public BadPlayer {
public:
    BadColor(string n, ECcolor c);
    void print(ostream& out) const override;
};

// ------------------------------------------------------------------

// BadChoice exception class
class BadChoice {
protected:
    string input;  // User's invalid input

public:
    BadChoice(string i);
    virtual ~BadChoice() = default;
    void basePrint(ostream& out) const;
    virtual void print(ostream& out) const;
};

// ------------------------------------------------------------------

// BadSlot exception class
class BadSlot : public BadChoice {
public:
    BadSlot(string i);
    void print(ostream& out) const override;
};

// ------------------------------------------------------------------

// DuplicateSlot exception class
class DuplicateSlot : public BadChoice {
public:
    DuplicateSlot(string i);
    void print(ostream& out) const override;
};

// ------------------------------------------------------------------

#endif