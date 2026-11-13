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
#include "column.hpp"

// Function to test the Dice class
void unitDice() {
    /*
    // Open test file
    ofstream out("diceTest.txt", ios::app); // Open file in append mode
    if(!out) {
        fatal("Cannot open test file"); // Handle file open error
    }
    */

    cout << "-------- Starting Unit Dice Test - Godburn & Aravind --------\n";

    // Test 1: Default constructor
    cout << "Test 1: Default constructor\n";
    Dice d1;               // Create a Dice object with default constructor
    d1.roll();             // Roll the dice
    cout << "Single die roll: " << d1 << "\n";

    // Test 2: Multiple dice with repeated rolls
    cout << "\nTest 2: Multiple dice\n";
    Dice d2(5);            // Create a Dice object with 5 dice
    for(int i = 0; i < 10; i++) {
        d2.roll();         // Roll the dice
        cout << "Roll " << i + 1 << ": " << d2 << "\n";
    }

    // Test 3: Range verification with large set
    cout << "\nTest 3: Range verification\n";
    Dice d3(100);          // Create a Dice object with 100 dice
    d3.roll();             // Roll the dice
    cout << "100 dice roll: " << d3 << "\n";

    cout << "---------- End Unit Dice Test ----------\n";

    /*
    // Close the test file
    out.close();
    */
}

// Function to test the Player class
void unitPlayer() {

    /*
    // Open test file
    ofstream out("playerTest.txt", ios::app); // Open file in append mode
    if(!out) {
        fatal("Cannot open test file"); // Handle file open error
    }
    */

    cout << "------- Starting Unit Player Test - Godburn & Aravind -------\n";

    // Test player creation and basic operations
    player wolf("RedWolf", ECcolor::blue);
    player ruby("RubyMan", ECcolor::green);

    // Print player states on single lines
    wolf.print(cout);
    cout << endl;
    ruby.print(cout);
    cout << endl;

    /// Test winning columns
    cout << "\nWinning columns:\n";
    wolf.wonColumn(3);
    wolf.print(cout);
    cout << endl;
    wolf.wonColumn(1);
    wolf.wonColumn(4);
    wolf.print(cout);
    cout << " (should have won)" << endl;

    // Test color access demonstrating enum output
    cout << "\nPlayer colors:\n" << "Wolf " << wolf.color() << endl
        << "Ruby " << ruby.color() << endl;

    cout << "---------- End Unit Player Test ----------\n";

    /*
    // Close the test file
    out.close();
    */

}

// Function to test the Column class
void unitColumn() {

/*
    // Open test file
    ofstream out("ColumnTest.txt", ios::app); // Open file in append mode
    if(!out) {
        fatal("Cannot open test file"); // Handle file open error
    }
*/

    cout << "------ Starting Unit Column Test - Godburn & Aravind -------\n";

    // Sample Players for testing
    player p1("Test1", ECcolor::blue);
    player p2("Test2", ECcolor::green);

    // Test 1: Create Columns
    Column col3(3);
    Column col7(7);
    cout << "\nInitial state:\n" << col3 << endl <<col7 << endl;

    // Test 2: Place Towers
    cout <<"\nPlacing Towers:\n";
    col3.startTower(&p1);
    col7.startTower(&p2);
    cout << col3 << endl << col7 << endl;

    // Test 3: Move Towers
    cout << "\nMoving Towers:\n";
    col3.move();
    col7.move();
    cout << col3 << endl << col7 << endl;

    // Test 4: Stop and Convert to tiles
    cout << "\nStopping moves:\n";
    col3.stop(&p1);
    col7.stop(&p2);
    cout << col3 << endl << col7 << endl;

    cout << "\n---------- End Column Test ----------\n";

    // Close the test file
    // out.close();

}

// Main function
int main() {
    srand(time(NULL));
    banner();
    unitColumn();
    bye();
    return 0;
}