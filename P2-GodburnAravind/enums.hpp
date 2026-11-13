/*
* enums.hpp - Header file for game enumerations and color definitions
 * Author: Ryan Godburn & Arjun Aravind
 * Date: 1/31/25
 * Description: This header file defines the color enumeration used in the game
 *              and provides helper functions for output.
 */

#ifndef ENUMS_HPP
#define ENUMS_HPP

#include "tools.hpp"

// Enumeration for player colors in the game
enum class ECcolor { white, orange, yellow, green, blue };

// Extern declaration for the color names array
extern const string colorNames[];

// Helper function to print color enum with formatting
inline ostream& operator<<(ostream& out, ECcolor color) {
    return out << "Color: \t" << colorNames[(int)color];
}

#endif