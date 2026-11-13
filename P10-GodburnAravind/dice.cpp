/*
* dice.cpp - Implementation file for the Dice class hierarchy.
 * Author: Ryan Godburn & Arjun Aravind
 * Date: 4/12/25
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
    for(int k = 0; k < nDice; k++) {
        dvals[k] = 1;
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

// Get user's dice pair selection
bool CSDice::getDicePairSelection(string& input) {
    cout << "Select two letters for first pair (e.g., AB): ";
    cin >> input;

    // Validate input length
    if (input.length() != 2) {
        cout << "Invalid input. Please enter exactly two letters.\n";
        return false;
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
        return false;
    }

    return true;
}

// Calculate and display dice pairs
void CSDice::calculatePairs(const string& input) {
    // Map letters to dice indices
    int idx1 = input[0] - 'A';
    int idx2 = input[1] - 'A';

    // Calculate first pair total
    pairs[0] = dvals[idx1] + dvals[idx2];

    // Find remaining two dice for second pair
    int idx3 = -1, idx4 = -1;
    for (int k = 0; k < 4; k++) {
        if (k != idx1 && k != idx2) {
            if (idx3 == -1)
                idx3 = k;
            else
                idx4 = k;
        }
    }

    // Calculate second pair total
    pairs[1] = dvals[idx3] + dvals[idx4];

    cout << "First pair: " << pairs[0]
    << ", Second pair: " << pairs[1] << endl;
}

// Display dice values
void CSDice::displayDiceValues() {
    cout << "Dice: A=" << dvals[0] << ", B=" << dvals[1]
         << ", C=" << dvals[2] << ", D=" << dvals[3] << endl;
}

// Override roll method
const int* CSDice::roll() {
    // Call base class roll to get random dice values
    Dice::roll();

    // Display dice values to the player
    displayDiceValues();

    // Get user input for dice pairing
    string input;
    bool validInput = false;

    // Keep trying until we get valid input
    while (!validInput) {
        validInput = getDicePairSelection(input);
    }

    // Calculate and display the pairs
    calculatePairs(input);

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

// Read dice values from file
bool FakeDice::readDiceValues() {
    // Read four dice values from file
    for (int k = 0; k < 4; k++) {
        if (!(infile >> dvals[k])) {
            // End of file or error
            cout << flush;  // Ensure all previous output is displayed
            fatal("Error reading from fake dice file or end of file reached");
            return false;
        }

        // Validate dice values (should be 1-6)
        if (dvals[k] < 1 || dvals[k] > 6) {
            cout << flush;  // Ensure all previous output is displayed
            fatal("Invalid dice value in fake dice file");
            return false;
        }
    }
    return true;
}

// Read move code from file
bool FakeDice::readMoveCode() {
    if (!(infile >> moveCode)) {
        // End of file or error when reading move code
        cout << flush;  // Ensure all previous output is displayed
        fatal("Error reading from fake dice file or end of file reached");
        return false;
    }

    // Validate move code (must be R, S, or Q)
    if (moveCode != FAKE_ROLL && moveCode != FAKE_STOP && moveCode != FAKE_RESIGN) {
        cout << flush;
        fatal("Invalid move code in fake dice file. Valid codes: R (roll), S (stop), Q (resign)");
        return false;
    }

    return true;
}

// Calculate pairs from file values
void FakeDice::calculateFileBasedPairs() {
    // First pair: first two dice, Second pair: last two dice
    pairs[0] = dvals[0] + dvals[1];
    pairs[1] = dvals[2] + dvals[3];

    cout << "First pair: " << pairs[0] <<
        ", Second pair: " << pairs[1] << endl;

    // Display the move code with description
    cout << "Move code from file: " << moveCode;
    if (moveCode == FAKE_ROLL) cout << " (Roll)";
    else if (moveCode == FAKE_STOP) cout << " (Stop)";
    else if (moveCode == FAKE_RESIGN) cout << " (Resign)";
    cout << endl;
}

// Override roll method
const int* FakeDice::roll() {
    // Read dice values from file
    if (!readDiceValues()) {
        // Error handling is done in readDiceValues with fatal()
        return pairs;
    }

    // Read move code
    if (!readMoveCode()) {
        // Error handling is done in readMoveCode with fatal()
        return pairs;
    }

    // Display dice values
    displayDiceValues();

    // Calculate and display pairs based on file data
    calculateFileBasedPairs();

    // Return pair totals
    return pairs;
}