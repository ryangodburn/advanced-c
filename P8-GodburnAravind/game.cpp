/*
* game.cpp - Implementation file for the Game class
 * Author: Ryan Godburn & Arjun Aravind
 * Date: 3/13/25
 * Description: This source file implements the Game class methods
 *              defined in game.hpp.
 */

#include "game.hpp"

// Constructor - handles setup
Game::Game() : gameOver(false), curPlayer(nullptr) {
    int numPlayers;
    int diceType;

    // Select dice type
    cout << "Select dice type:\n"
         << "1 - Can't Stop dice (manual pairing)\n"
         << "2 - Fake dice (from file)\n"
         << "Choice: ";

    while (!(cin >> diceType) || diceType < 1 || diceType > 2) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input. Enter dice type (1-2): ";
    }

    // Create appropriate dice type
    switch (diceType) {
        case 1:
            dice = make_unique<CSDice>();
            break;
        case 2:
            dice = make_unique<FakeDice>();
            break;
    }

    // Get number of players
    cout << "Enter number of players (2-4): ";
    while (!(cin >> numPlayers) || numPlayers < 2 || numPlayers > 4) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input. Enter number of players (2-4): ";
    }

    // Get info for each player
    for (int k = 0; k < numPlayers; k++) {
        cout << "\nEnter data for Player " << (k+1) << ":\n";
        players.addCell(getNewPlayer());
    }

    // Initialize player list and set current player
    players.init();
    curPlayer = players.next();

    cout << "\nPlayers in game:\n" << players << endl;
}

// Automated game mode with FakeDice
void Game::playGameWithFakeDice() {
    // Main game loop
    while (!gameOver) {
        cout << "\n=== " << *curPlayer << "'s turn ===\n";

        // Start the turn on the board using the current player
        board.startTurn(curPlayer);

        // Display board at start of turn
        cout << board << endl;

        // Automated turn with FakeDice
        bool turnOver = false;
        FakeDice* fakeDice = dynamic_cast<FakeDice*>(dice.get());

        while (!turnOver) {
            // Automatically roll the dice
            cout << "AUTO: Rolling dice...\n";
            const int* pairVals = rollAndDisplayDice();

            // Make moves based on the pair values
            int pair1 = pairVals[0];
            int pair2 = pairVals[1];

            // Try the moves and check if we bust
            bool busted = makeMovesAndCheckBust(pair1, pair2);

            if (busted) {
                turnOver = true;
                cout << "AUTO: Turn over due to bust.\n";
            } else {
                // Check the move code to decide what to do next
                string moveCode = fakeDice->getMoveCode();

                if (moveCode == FAKE_STOP) {
                    // Stop the turn
                    board.stop();
                    cout << "AUTO: Stopping turn based on file "
                            "instruction.\n";
                    cout << "Turn ended, towers converted to "
                            "tiles:\n" << board << endl;
                    turnOver = true;

                    // Check if player won
                    if (curPlayer->score() >= 3) {
                        cout << "\n************************"
                                "*******************\n";
                        cout << "*** WINNER: " << *curPlayer << " ***\n";
                        cout << "***************************"
                                "****************\n";
                        gameOver = true;
                        break;
                    }
                } else {
                    // Continue rolling
                    cout << "AUTO: Continuing turn based "
                            "on file instruction.\n";
                }
            }
        }

        if (!gameOver) {
            // Move to next player
            nextPlayer();
        }
    }

    cout << "\nGame Over!\n";
}

// Interactive game mode with CSDice
void Game::playGameWithCSDice() {
    // Main game loop
    while (!gameOver) {
        cout << "\n=== " << *curPlayer << "'s turn ===\n";

        // Start the turn on the board using the current player
        board.startTurn(curPlayer);

        // Display board at start of turn
        cout << board << endl;

        // Interactive turn with CSDice
        bool turnOver = false;
        bool stoppedSuccessfully = false;

        while(!turnOver) {
            cout << "\n-- Turn Options --\n"
                 << "1. Roll\n"
                 << "2. Stop\n"
                 << "3. Resign\n"
                 << "Choice: ";

            int choice;
            cin >> choice;

            switch(choice) {
                case 1: // Roll
                    turnOver = handleRoll();
                    break;

                case 2: // Stop
                    turnOver = handleStop();
                    // Track if player stopped successfully
                    stoppedSuccessfully = turnOver;
                    break;

                case 3: // Resign
                    turnOver = handleResign();
                    break;

                default:
                    cout << "Invalid choice. Please select 1, 2, or 3.\n";
                    // Clear input stream in case of invalid input
                    cin.clear();
                    cin.ignore(10000, '\n');
            }
        }

        // Check if player won after stopping successfully
        if (stoppedSuccessfully && curPlayer->score() >= 3) {
            cout << "\n*******************************************\n";
            cout << "*** WINNER: " << *curPlayer << " ***\n";
            cout << "*******************************************\n";
            gameOver = true;
        } else {
            // Move to next player
            nextPlayer();
        }
    }

    cout << "\nGame Over!\n";
}

// Get player info
unique_ptr<player> Game::getNewPlayer() {
    string name;
    int color;

    // Get valid player name
    for(;;) {
        cout << "Enter player name (no spaces): ";
        cin >> name;

        if(name.length() > 0 && name.length() <= 20) break;
        cout << "Invalid name or too long. Try again.\n";
    }

    // Get valid color selection
    for(;;) {
        cout << "Enter color:\n"
             << "1 - Orange\n"
             << "2 - Yellow\n"
             << "3 - Green\n"
             << "4 - Blue\n"
             << "Choice: ";

        if(cin >> color && color >= 1 && color <= 4) break;
        // Clear error state
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid color. Try again.\n";
    }

    return make_unique<player>(name, ECcolor(color));
}

// Move to next player
void Game::nextPlayer() {
    curPlayer = players.next();
}

// Main game loop
void Game::playGame() {
    // Check which type of dice we're using and call appropriate method
    if (dynamic_cast<FakeDice*>(dice.get()) != nullptr) {
        playGameWithFakeDice();
    } else {
        playGameWithCSDice();
    }
}

// Roll dice and display values
const int* Game::rollAndDisplayDice() {
    return (*dice).roll();
}

// Get valid dice pair selection from user
bool Game::getDicePairSelection(string& pairInput) {
    cout << "Select two letters for first pair (e.g., AB): ";
    cin >> pairInput;

    // Validate input
    if(pairInput.length() != 2) {
        cout << "Invalid input. Please enter exactly two letters.\n";
        return false;
    }

    // Convert to uppercase
    pairInput[0] = toupper(pairInput[0]);
    pairInput[1] = toupper(pairInput[1]);

    // Validate letters
    if(pairInput[0] < 'A' || pairInput[0] > 'D' ||
       pairInput[1] < 'A' || pairInput[1] > 'D' ||
       pairInput[0] == pairInput[1]) {
        cout << "Invalid dice selection. Please use A, B, C, or D "
                "without repeating.\n";
        return false;
    }

    return true;
}

// Calculate column values from dice pairs
void Game::calculatePairValues(const int* dvals, string pairInput,
                               int& pair1, int& pair2) {
    // Map letters to dice indices
    int Idx1 = pairInput[0] - 'A';
    int Idx2 = pairInput[1] - 'A';

    // Calculate first pair total
    pair1 = dvals[Idx1] + dvals[Idx2];

    // Find the remaining two dice for second pair
    int Idx3 = -1, Idx4 = -1;
    for(int Idx = 0; Idx < 4; Idx++) {
        if(Idx != Idx1 && Idx != Idx2) {
            if(Idx3 == -1)
                Idx3 = Idx;
            else
                Idx4 = Idx;
        }
    }
    pair2 = dvals[Idx3] + dvals[Idx4];

    cout << "First pair: " << pair1 << ", Second pair: " << pair2 << endl;
}

// Make moves to columns and check for bust
bool Game::makeMovesAndCheckBust(int pair1, int pair2) {
    // Validate column values before calling move
    bool move1Valid = (pair1 >= 2 && pair1 <= 12);
    bool move2Valid = (pair2 >= 2 && pair2 <= 12);

    // Try moves
    bool move1Success = move1Valid && board.move(pair1);
    cout << "Move to column " << pair1 << ": "
         << (move1Success ? "Success" : "Failed") << endl;

    bool move2Success = move2Valid && board.move(pair2);
    cout << "Move to column " << pair2 << ": "
         << (move2Success ? "Success" : "Failed") << endl;

    // Display board
    cout << board << endl;

    // Check for bust
    if(!move1Success && !move2Success) {
        cout << "*** BUST! ***\n";
        board.bust();
        return true; // Return true to indicate the turn is over
    }

    return false; // Return false to indicate the turn is not over yet
}

// Handle roll action
bool Game::handleRoll() {
    // Roll dice and get pair values
    // (already displayed in polymorphic roll)
    const int* pairVals = rollAndDisplayDice();

    // With CSDice or FakeDice, the roll() method
    // now returns pair values directly
    int pair1 = pairVals[0];
    int pair2 = pairVals[1];

    // Make moves and check for bust
    return makeMovesAndCheckBust(pair1, pair2);
}

// Handle stop action
bool Game::handleStop() {
    board.stop();
    cout << "Turn ended, towers converted to tiles:\n" << board << endl;
    return true; // Return true to indicate the turn is over
}

// Handle resign action
bool Game::handleResign() {
    cout << "Resign not implemented yet.\n";
    return false; // Return false to indicate the turn is not over yet
}