/*
* game.cpp - Implementation file for the Game class
 * Author: Ryan Godburn & Arjun Aravind
 * Date: 4/4/25
 * Description: This source file implements the Game class methods
 *              defined in game.hpp with exception handling.
 */

#include "game.hpp"

// Constructor - handles setup
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

    // Initialize player list
    players.init();

    // Position at the player before the first player
    // In a circular list, this is the last player
    for (int k = 0; k < players.count() - 1; k++) players.next();

    // Now the next call to next() will give us the first player
    curPlayer = players.next();

    cout << "\nPlayers in game:\n" << players << endl;
}

// Check data validity - throw exceptions for duplicate name/color
bool Game::checkData(string name, ECcolor color) {
    // Start at head of player list
    players.init();
    int count = players.count();

    bool nameUsed = false;
    bool colorUsed = false;

    // Check against each existing player
    for(int i = 0; i < count; i++) {
        player* p = players.next();

        // Check if name is already in use
        if(p->getName() == name) {
            nameUsed = true;
        }

        // Check if color is already in use
        if(p->color() == color) {
            colorUsed = true;
        }
    }

    // Throw appropriate exception based on what's duplicated
    if(nameUsed && colorUsed) {
        throw BadPlayer(name, color);
    } else if(nameUsed) {
        throw BadName(name, color);
    } else if(colorUsed) {
        throw BadColor(name, color);
    }

    return true;  // Data is valid
}

// Get player info with exception handling
unique_ptr<player> Game::getNewPlayer() {
    string name;
    int color;
    ECcolor playerColor;

    while(true) {
        try {
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

            playerColor = ECcolor(color);

            // Validate that name and color are unique
            checkData(name, playerColor);

            // If we get here, data is valid
            return make_unique<player>(name, playerColor);
        }
        catch(BadPlayer& e) {
            e.print(cout);
            cout << "Please try again.\n";
        }
        catch(...) {
            cout << "Unknown error occurred. Aborting.\n";
            fatal("Exception in getNewPlayer()");
        }
    }
}

// Validate ROLL/STOP/QUIT choice
int Game::validateTurnChoice() {
    cout << "\nCurrent Player: " << *curPlayer << endl;
    cout << "-- Turn Options --\n"
         << "1. Roll\n"
         << "2. Stop\n"
         << "3. Resign\n"
         << "Choice: ";

    string input;
    cin >> input;

    // Validate choice
    if(input.length() != 1 || (input[0] != '1'
        && input[0] != '2' && input[0] != '3')) {
        throw BadChoice(input);
        }

    return input[0] - '0';  // Convert to integer
}

// Validate resign confirmation
bool Game::validateResignConfirmation() {
    cout << "Are you sure you want to resign? (y/n): ";

    string input;
    cin >> input;

    // Convert to lowercase for easier comparison
    if(input.length() == 1) {
        input[0] = tolower(input[0]);
    }

    // Validate choice
    if(input.length() != 1 || (input[0] != 'y' && input[0] != 'n')) {
        throw BadChoice(input);
    }

    return (input[0] == 'y');  // Return true for yes, false for no
}

// Automated game mode with FakeDice
void Game::oneTurn_FakeDice() {
    // Main game loop
    while (!gameOver) {
        cout << "\n=== " << *curPlayer << "'s turn ===\n";

        // Start the turn on the board using the current player
        board.startTurn(curPlayer);

        // Display board at start of turn
        cout << board << endl;

        // Automated turn with FakeDice
        bool turnOver = false;
        bool busted = false;
        bool stoppedSuccessfully = false;
        FakeDice* fakeDice = dynamic_cast<FakeDice*>(dice.get());

        while (!turnOver) {
            // Automatically roll the dice
            cout << "AUTO: Rolling dice...\n";
            const int* pairVals = rollAndDisplayDice();

            // Make moves based on the pair values
            int pair1 = pairVals[0];
            int pair2 = pairVals[1];

            // Try the moves and check if we bust
            busted = makeMovesAndCheckBust(pair1, pair2);

            if (busted) {
                turnOver = true;
                cout << "AUTO: Turn over due to bust. Moving to next player.\n";
            } else {
                // Check the move code to decide what to do next
                string moveCode = fakeDice->getMoveCode();

                if (moveCode == FAKE_STOP) {
                    // Stop the turn
                    board.stop();
                    stoppedSuccessfully = true;
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
void Game::oneTurn() {
    // Main game loop
    while(!gameOver) {
        cout << "\n=== " << *curPlayer << "'s turn ===\n";

        // Start the turn on the board using the current player
        board.startTurn(curPlayer);

        // Display board at start of turn
        cout << board << endl;

        // Interactive turn with CSDice
        bool turnOver = false;
        bool stoppedSuccessfully = false;
        bool busted = false;

        while(!turnOver) {
            try {
                // Get and validate player choice with separated
                // validation function
                int choice = validateTurnChoice();

                switch(choice) {
                    case 1: // Roll
                        busted = handleRoll();
                        if (busted) {
                            cout << "Turn over due to bust. "
                                    "Moving to next player.\n";
                            turnOver = true;
                        }
                        break;

                    case 2: // Stop
                        turnOver = handleStop();
                        // Track if player stopped successfully
                        stoppedSuccessfully = turnOver;
                        break;

                    case 3: // Resign
                        turnOver = handleResign();
                        break;
                }
            }
            catch(BadChoice& e) {
                e.print(cout);
                // Clear input buffer
                cin.clear();
                cin.ignore(10000, '\n');
            }
            catch(...) {
                cout << "Unknown error in choice selection. Try again.\n";
                // Clear input buffer
                cin.clear();
                cin.ignore(10000, '\n');
            }
        }

        // Check if player won after stopping successfully
        if(stoppedSuccessfully && curPlayer->score() >= 3) {
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

// Move to next player
void Game::nextPlayer() {
    curPlayer = players.next();
}

// Main game loop
void Game::playGame() {
    // Check which type of dice we're using and call appropriate method
    if (dynamic_cast<FakeDice*>(dice.get()) != nullptr) {
        oneTurn_FakeDice();
    } else {
        oneTurn();
    }
}

// Roll dice and display values
const int* Game::rollAndDisplayDice() {
    return (*dice).roll();
}

// Validate dice pair selection using strchr
bool Game::getDicePairSelection(string& pairInput) {
    cout << "Select two letters for first pair (e.g., AB): ";
    cin >> pairInput;

    // Validate input length
    if(pairInput.length() != 2) {
        throw BadSlot(pairInput);
    }

    // Convert to uppercase
    pairInput[0] = toupper(pairInput[0]);
    pairInput[1] = toupper(pairInput[1]);

    // Validate letters using strchr as required
    const char* validChars = "ABCD";
    if(!strchr(validChars, pairInput[0]) || !strchr(validChars, pairInput[1])) {
        throw BadSlot(pairInput);
    }

    // Check for duplicates
    if(pairInput[0] == pairInput[1]) {
        throw DuplicateSlot(pairInput);
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
        return true; // Return true to indicate the turn is over due to bust
    }

    return false; // Return false to indicate the turn is not over yet
}

// Handle roll action with exception handling
bool Game::handleRoll() {
    try {
        // Roll dice and get values - for CSDice, this already
        // handles user input and returns the calculated pair
        // values in an array
        const int* pairVals = rollAndDisplayDice();

        // For all dice types, use the returned pair values directly
        int pair1 = pairVals[0];
        int pair2 = pairVals[1];

        // Validate column values before calling move
        bool move1Valid = (pair1 >= 2 && pair1 <= 12);
        bool move2Valid = (pair2 >= 2 && pair2 <= 12);

        if (!move1Valid && !move2Valid) {
            cout << "Error: Both calculated column values (" << pair1 <<
                " and " << pair2 << ") are invalid. "
                "Must be between 2-12.\n";
            return false;  // Let player try again
        }

        // Make moves and check for bust
        return makeMovesAndCheckBust(pair1, pair2);
    }
    catch(BadChoice& e) {
        e.print(cout);
        // Clear input buffer
        cin.clear();
        cin.ignore(10000, '\n');
        return false;  // Let player try again
    }
    catch(...) {
        cout << "Unknown error in dice roll. Try again.\n";
        // Clear input buffer
        cin.clear();
        cin.ignore(10000, '\n');
        return false;  // Let player try again
    }
}

// Handle stop action
bool Game::handleStop() {
    board.stop();
    cout << "Turn ended, towers converted to tiles:\n" << board << endl;
    return true; // Return true to indicate the turn is over
}

// Handle resign action with exception handling
bool Game::handleResign() {
    try {
        bool confirmed = validateResignConfirmation();

        if(confirmed) {
            cout << *curPlayer << " has resigned from the game.\n";

            // Remove player from list
            players.remove();

            // Check if only one player remains
            if(players.count() == 1) {
                players.init();
                player* winner = players.next();
                cout << "\n*******************************************\n";
                cout << "*** WINNER by default: " << *winner << " ***\n";
                cout << "*******************************************\n";
                gameOver = true;
            } else {
                // Set current player to next player
                curPlayer = players.next();
            }

            return true; // Turn is over
        } else {
            cout << "Resignation canceled.\n";
            return false; // Turn continues
        }
    }
    catch(BadChoice& e) {
        e.print(cout);
        // Clear input buffer
        cin.clear();
        cin.ignore(10000, '\n');
        return false;  // Let player try again
    }
    catch(...) {
        cout << "Unknown error in resign confirmation. Try again.\n";
        // Clear input buffer
        cin.clear();
        cin.ignore(10000, '\n');
        return false;  // Let player try again
    }
}