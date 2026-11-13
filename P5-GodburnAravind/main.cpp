/*
* main.cpp - A test program for the Game & Board class.
 * Author: Ryan Godburn & Arjun Aravind
 * Date: 2/25/25
 * Description: This program tests the Game class and board class
 *              by writing results to a file and printing them to
 *              the console.
 */

// Libraries
#include "dice.hpp"
#include "player.hpp"
#include "enums.hpp"
#include "column.hpp"
#include "game.hpp"

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

    cout << "------ Starting Unit Player Test - Godburn & Aravind ------\n";

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

// Function to test the Board generation and class
void unitBoard() {

    /*
    // Open test file
    ofstream out("BoardTest.txt", ios::app); // Open file in append mode
    if(!out) {
        fatal("Cannot open test file"); // Handle file open error
    }
    */


    cout << "------ Starting Unit Board Test  - Godburn & Aravind ------\n";

    // Create a board and players
    Board board;
    player p1("TestPlayer1", ECcolor::orange);
    player p2("TestPlayer2", ECcolor::blue);

    // Test 1: Initial state
    cout << "Test 1: Initial Board State\n"
         << board << endl;

    // Test 2: Start turn and move to columns
    cout << "Test 2: Player 1 Turn - Moving to columns\n";
    board.startTurn(&p1);

    // Try moving to different columns
    cout << "Move to column 2: "
         << (board.move(2) ? "Success" : "Failed") << endl;
    cout << "Move to column 7: "
         << (board.move(7) ? "Success" : "Failed") << endl;
    cout << "Move to column 12: "
         << (board.move(12) ? "Success" : "Failed") << endl;
    // Try too many towers
    cout << "Move to column 4 (should fail): "
         << (board.move(4) ? "Success" : "Failed") << endl;
    cout << board << endl;

    // Test 3: Stop turn and check tiles
    cout << "Test 3: Stopping Turn - Converting towers to tiles\n";
    board.stop();
    cout << board << endl;

    // Test 4: Start new turn and bust
    cout << "Test 4: Player 2 Turn with Bust\n";
    board.startTurn(&p2);
    board.move(3);
    board.move(5);
    cout << "Before bust:\n" << board << endl;
    board.bust();
    cout << "After bust:\n" << board << endl;

    // Test 5: Try moving to captured column
    cout << "Test 5: Attempting to move to captured column\n";
    board.startTurn(&p2);
    cout << "Move to column 2 (captured): "
         << (board.move(2) ? "Success" : "Failed") << endl;

    cout << "---------- End Board Test ----------\n";

    // Close the test file
    //out.close();

}

// Function to test the Game class
void testGame(Game& game) {

    /*
    // Open test file
    ofstream out("GameTest.txt", ios::app); // Open file in append mode
    if(!out) {
    fatal("Cannot open test file"); // Handle file open error
    }
    */


    cout << "\n------ Starting Game Test  - Godburn & Aravind ------\n\n";

    // Test 1: Verify Dice construction and functionality
    cout << "Test 1: Testing Dice Construction and Rolling\n";
    Dice& gameDice = *game.getDice();
    gameDice.roll();
    cout << "Dice Rolls: " << gameDice << endl << endl;

    // Test 2: Verify Player construction
    cout << "Test 2: Verifying Player Construction\n";
    player& p1 = *game.getPlayer();
    cout << "Player: " << p1 << endl << endl;

    // Test 3: Verify Board functionality
    cout << "Test 3: Testing Board Operations\n";
    Board& board = *game.getBoard();

    // Start turn
    cout << "Starting turn for player\n";
    board.startTurn(&p1);

    // Try moving to columns
    cout << "Move to column 3: "
         << (board.move(3) ? "Success" : "Failed") << endl;
    cout << "Move to column 8: "
         << (board.move(8) ? "Success" : "Failed") << endl;
    cout << "Board state after moves:\n" << board << endl;

    // Stop turn
    cout << "Stopping turn:\n";
    board.stop();
    cout << "Board after stop:\n" << board << endl;

    // Test bust functionality
    cout << "Starting new turn and testing bust:\n";
    board.startTurn(&p1);
    board.move(5);
    cout << "Board before bust:\n" << board << endl;
    board.bust();
    cout << "Board after bust:\n" << board << endl;

    cout << "\n--------- End Game Test ---------\n";

    // Close the test file
    //out.close();

    }


// Main function
int main() {
    srand(time(NULL));
    banner();

    // Create game instance to test the game and test the board
    Game game;
    testGame(game);
    unitBoard();

    bye();
    return 0;
}