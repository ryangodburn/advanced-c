/*
* dice.cpp - Implementation file for the Dice class hierarchy.
 * Author: Ryan Godburn & Arjun Aravind
 * Date: 4/1/25
 * Description: This source file implements the methods in Dice.hpp.
 *              It includes implementations for Dice, CSDice,
 *              and FakeDice classes.
 */

#include "dice.hpp"

// -------- Dice class implementation --------

// Constructor
Dice::Dice(const int n) {
    nDice = n;           // Initialize number of dice
    dvals = new int[nDice]; // Dynamically allocate memory for dice values

    // Initialize dice values to 1
    for(int i = 0; i < nDice; i++) {
        dvals[i] = 1;
    }
}

// Destructor
Dice::~Dice() {
    delete[] dvals;  // Release allocated memory
}

// Roll dice
const int* Dice::roll() {
    for(int diceNum = 0; diceNum < nDice; diceNum++) {
        dvals[diceNum] = rand() % 6 + 1; // Generate value between 1 and 6
    }
    return dvals; // Return array of dice values
}

// Print values
ostream& Dice::print(ostream& os) {
    for(int diceNum = 0; diceNum < nDice; diceNum++) {
        os << dvals[diceNum];    // Print each dice value
        if(diceNum < nDice - 1)  // Print space between values
                                 // except after the last value
            os << " ";
    }
    return os; // Return ostream object
}

// -------- CSDice class implementation --------

// Constructor
CSDice::CSDice() : Dice(4) {
    // Initialize pair totals array
    pairs[0] = 0;
    pairs[1] = 0;
}

// Override roll method
const int* CSDice::roll() {
    // Call base class roll to get random dice values
    Dice::roll();

    // Display dice values to the player
    cout << "Dice: A=" << dvals[0] << ", B=" << dvals[1]
         << ", C=" << dvals[2] << ", D=" << dvals[3] << endl;

    // Prompt user to select the first pair
    string input;
    bool validInput = false;
    int idx1, idx2;

    while (!validInput) {
        cout << "Select two letters for first pair (e.g., AB): ";
        cin >> input;

        // Validate input
        if (input.length() != 2) {
            cout << "Invalid input. Please enter exactly two letters.\n";
            continue;
        }

        // Convert to uppercase
        input[0] = toupper(input[0]);
        input[1] = toupper(input[1]);

        // Validate letters
        if (input[0] < 'A' || input[0] > 'D' ||
            input[1] < 'A' || input[1] > 'D' ||
            input[0] == input[1]) {
            cout << "Invalid dice selection. Please use A, B, C, or D "
                 << "without repeating.\n";
            continue;
        }

        validInput = true;
    }

    // Map letters to dice indices
    idx1 = input[0] - 'A';
    idx2 = input[1] - 'A';

    // Calculate first pair total
    pairs[0] = dvals[idx1] + dvals[idx2];

    // Find remaining two dice for second pair
    int idx3 = -1, idx4 = -1;
    for (int i = 0; i < 4; i++) {
        if (i != idx1 && i != idx2) {
            if (idx3 == -1)
                idx3 = i;
            else
                idx4 = i;
        }
    }

    // Calculate second pair total
    pairs[1] = dvals[idx3] + dvals[idx4];

    cout << "First pair: " << pairs[0]
    << ", Second pair: " << pairs[1] << endl;

    // Return pair totals
    return pairs;
}

// -------- FakeDice class implementation --------

// Constructor
FakeDice::FakeDice() : CSDice() {
    // Open the fake dice file
    infile.open(FAKE_DICE_FILE);
    if (!infile) {
        cout << flush;  // Ensure all previous output is displayed
        fatal("Cannot open fake dice file");
    }
    moveCode = FAKE_ROLL; // Default to roll
}

// Destructor
FakeDice::~FakeDice() {
    // Close the file if it's open
    if (infile.is_open()) {
        infile.close();
    }
}

// Override roll method
const int* FakeDice::roll() {
    // Read four dice values from file
    for (int i = 0; i < 4; i++) {
        if (!(infile >> dvals[i])) {
            // End of file or error
            cout << flush;  // Ensure all previous output is displayed
            fatal("Error reading from fake dice file or end of file reached");
        }

        // Validate dice values (should be 1-6)
        if (dvals[i] < 1 || dvals[i] > 6) {
            cout << flush;  // Ensure all previous output is displayed
            fatal("Invalid dice value in fake dice file");
        }
    }

    // Read the roll/stop code
    if (!(infile >> moveCode)) {
        // End of file or error when reading move code
        cout << flush;  // Ensure all previous output is displayed
        fatal("Error reading move code from fake dice file");
    }

    // Display dice values
    cout << "Dice: A=" << dvals[0] << ", B=" << dvals[1]
         << ", C=" << dvals[2] << ", D=" << dvals[3] << endl;

    // First pair: first two dice, Second pair: last two dice
    pairs[0] = dvals[0] + dvals[1];
    pairs[1] = dvals[2] + dvals[3];

    cout << "First pair: " << pairs[0] <<
        ", Second pair: " << pairs[1] << endl;

    cout << "Move code from file: " << moveCode <<
        (moveCode == FAKE_ROLL ? " (Roll)" : " (Stop)") << endl;

    // Return pair totals
    return pairs;
}