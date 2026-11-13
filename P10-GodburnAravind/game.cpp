/*
* game.cpp - Implementation file for the Game class
 * Author: Ryan Godburn & Arjun Aravind
 * Date: 4/12/25
 * Description: This source file implements the Game class methods
 *              defined in game.hpp with exception handling.
 */

#include "game.hpp"

// Constructor - initialize game parts and get players
Game::Game() : pairVals(nullptr), gameState(ECgame::begun) {
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
    case 1: dice = new CSDice(); break;
    case 2: dice = new FakeDice(); break;
    }

    // Get player information
    getPlayers();
}

// Destructor - clean up dynamically allocated dice
Game::~Game() {
    delete dice;
}

// getPlayers() - input a list of players
void Game::getPlayers() {
    int numPlayers;

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

    players.init();
    cout << "\nPlayers in game:\n" << players << endl;
}

// Check data validity - throw exceptions for duplicate name/color
bool Game::checkData(string name, ECcolor color) {
    players.init();
    int count = players.count();
    bool nameUsed = false;
    bool colorUsed = false;

    // Check against each existing player
    for(int k = 0; k < count; k++) {
        player* p = players.next();
        if((*p).getName() == name) nameUsed = true;
        if((*p).color() == color) colorUsed = true;
    }

    // Throw appropriate exception
    if(nameUsed && colorUsed) {
        throw BadPlayer(name, color);
    } else if(nameUsed) {
        throw BadName(name, color);
    } else if(colorUsed) {
        throw BadColor(name, color);
    }

    return true;  // Data is valid
}

// Get a valid player name from user
string Game::promptForPlayerName() {
    string name;
    for(;;) {
        cout << "Enter player name (no spaces): ";
        cin >> name;

        if(name.length() > 0 && name.length() <= 20)
            break;
        cout << "Invalid name or too long. Try again.\n";
    }
    return name;
}

// Get a valid color selection from user
ECcolor Game::promptForPlayerColor() {
    int color;
    for(;;) {
        cout << "Enter color:\n"
             << "1 - Orange\n"
             << "2 - Yellow\n"
             << "3 - Green\n"
             << "4 - Blue\n"
             << "Choice: ";

        if(cin >> color && color >= 1 && color <= 4)
            break;
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid color. Try again.\n";
    }
    return ECcolor(color);
}

// Get player info with exception handling
unique_ptr<player> Game::getNewPlayer() {
    while(true) {
        try {
            string name = promptForPlayerName();
            ECcolor playerColor = promptForPlayerColor();

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

// Main game function - play an entire game
void Game::play() {
    player* curPlayer = nullptr;

    // Initialize the game and get first player
    playInit(curPlayer);

    // Main game loop
    while (gameState == ECgame::begun) {
        // Check if there are no players left
        if (checkNoPlayersLeft()) {
            break;
        }

        // Handle current player's turn
        handlePlayerTurn(curPlayer);
    }

    cout << "\nGame Over!\n";
}

// Initialize the game and set the first player
void Game::playInit(player*& curPlayer) {
    // Initialize the player list
    players.init();

    // Get last player in the list
    for(int k = 0; k < players.count() - 1; k++) {
        players.next();
    }

    // Now next() will give the first player
    curPlayer = players.next();
}

// Check if there are no players left
bool Game::checkNoPlayersLeft() {
    if (players.count() == 0) {
        cout << "No players left. Game over.\n";
        gameState = ECgame::quit;
        return true;
    }
    return false;
}

// Handle a player's turn
void Game::handlePlayerTurn(player*& curPlayer) {
    // Do one turn for current player
    gameState = oneTurn(curPlayer);

    // If the current player wins, the game is done
    if (gameState == ECgame::done) {
        announceWinner(curPlayer);
    } else if (gameState == ECgame::begun) {
        // If game not done, move to next player
        curPlayer = players.next();
    }
}

// Announce the winner
void Game::announceWinner(player* winner) {
    cout << "\n*******************************************\n";
    cout << "*** WINNER: " << *winner << " ***\n";
    cout << "*******************************************\n";
}

// oneTurn - determine dice type and delegate to appropriate handler
ECgame Game::oneTurn(player* p) {
    cout << "\n=== " << *p << "'s turn ===\n";

    // Tell the Board that Player p is starting a new turn
    board.startTurn(p);

    // Print player info and board
    cout << "Name: " << (*p).getName()
         << ", Color: " << (*p).color()
         << ", Score: " << (*p).score() << endl;
    cout << board << endl;

    // Check if we're using FakeDice
    FakeDice* fakeDice = dynamic_cast<FakeDice*>(dice);

    if (fakeDice != nullptr) {
        return handleFakeDiceTurn(p);
    } else {
        return handleInteractiveTurn(p);
    }
}

// Handle automated turn with FakeDice
ECgame Game::handleFakeDiceTurn(player* p) {
    FakeDice* fakeDice = dynamic_cast<FakeDice*>(dice);
    bool turnOver = false;

    while (!turnOver) {
        // Automatically roll the dice - flush output before rolling
        cout << "AUTO: Rolling dice...\n" << flush;
        pairVals = rollAndDisplayDice();

        // Make moves and check if bust
        bool busted = makeMovesAndCheckBust(pairVals[0], pairVals[1]);

        // Process the result
        ECgame result = processFakeDiceResult(p, busted, turnOver);
        if (result != ECgame::begun) {
            return result;
        }
    }

    return ECgame::begun; // Continue game with next player
}

// Process the result of a FakeDice turn
ECgame Game::processFakeDiceResult(player* p, bool busted, bool& turnOver) {
    if (busted) {
        return processFakeDiceBust(turnOver);
    }

    // Check move code (roll, stop, or resign)
    FakeDice* fakeDice = dynamic_cast<FakeDice*>(dice);
    string moveCode = (*fakeDice).getMoveCode();

    if (moveCode == FAKE_STOP) {
        turnOver = true;
        return processFakeDiceStop(p);
    } else if (moveCode == FAKE_RESIGN) {
        turnOver = true;
        return processFakeDiceResign(p);
    } else {
        return processFakeDiceContinue();
    }
}

// Process FakeDice bust
ECgame Game::processFakeDiceBust(bool& turnOver) {
    turnOver = true;
    cout << "AUTO: Turn over due to bust.\n" << flush;
    return ECgame::begun;
}

// Process FakeDice Stop command
ECgame Game::processFakeDiceStop(player* p) {
    // Stop the turn
    board.stop();
    cout << "AUTO: Stopping turn based on file instruction.\n" << flush;
    cout << "Turn ended, towers converted to tiles:\n" <<
        board << endl << flush;

    // Check if player won
    if ((*p).score() >= 3) {
        return ECgame::done;
    }

    return ECgame::begun;
}

// Process FakeDice continue rolling
ECgame Game::processFakeDiceContinue() {
    cout << "AUTO: Continuing turn based on file instruction.\n" << flush;
    return ECgame::begun;
}

// Process FakeDice Resign command (new function)
ECgame Game::processFakeDiceResign(player* p) {
    cout << "AUTO: Player " << (*p).getName()
        << " resigns based on file instruction.\n" << flush;
    players.remove();
    return checkWinCondition(p);
}

// Handle interactive turn with CSDice
ECgame Game::handleInteractiveTurn(player* p) {
    bool turnOver = false;

    while (!turnOver) {
        try {
            // Present menu for player action
            int choice = validateTurnChoice();

            // Process the choice
            ECgame result = processPlayerChoice(p, choice, turnOver);
            if (result != ECgame::begun) {
                return result;
            }
        }
        catch (BadChoice& e) {
            handleChoiceError(e);
        }
        catch (...) {
            cout << "Unknown error in choice selection. Try again.\n";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }

    return ECgame::begun; // Continue with next player
}

// Process player's menu choice
ECgame Game::processPlayerChoice(player* p, int choice, bool& turnOver) {
    switch (choice) {
        case 1: // Roll
            pairVals = rollAndDisplayDice();
            return handleAfterRollResult(p,
                  makeMovesAndCheckBust(pairVals[0], pairVals[1]),
                  turnOver);

        case 2: // Stop
            turnOver = handleStop(p);
            if ((*p).score() >= 3) {
                return ECgame::done;
            }
            break;

        case 3: // Resign
            if (validateResignConfirmation()) {
                cout << (*p).getName() << " has resigned.\n";
                players.remove();
                return checkWinCondition(p);
            }
            break;
    }

    return ECgame::begun;
}

// Handle the result after rolling dice
ECgame Game::handleAfterRollResult(player* p, bool busted, bool& turnOver) {
    // If bust, end turn
    if (busted) {
        cout << "Turn over due to bust.\n";
        turnOver = true;
    }

    return ECgame::begun;
}

// Handle errors in player choice
void Game::handleChoiceError(BadChoice& e) {
    e.print(cout);

    cin.clear();
    cin.ignore(10000, '\n');
}

// Validate ROLL/STOP/QUIT choice
int Game::validateTurnChoice() {
    cout << "\n-- Turn Options --\n"
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

// Roll dice and display values
const int* Game::rollAndDisplayDice() {
    // Flush cout before rolling to ensure all previous output is displayed
    cout << flush;
    return (*dice).roll();
}

// Validate dice pair selection
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
    if(!strchr(validChars, pairInput[0]) ||
       !strchr(validChars, pairInput[1])) {
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
            if(Idx3 == -1) Idx3 = Idx;
            else Idx4 = Idx;
        }
    }
    pair2 = dvals[Idx3] + dvals[Idx4];

    cout << "First pair: " << pair1 << ", Second pair: " << pair2 << endl;
}

// Make moves to columns and check for bust
bool Game::makeMovesAndCheckBust(int pair1, int pair2) {
    // Flush output before continuing
    cout << flush;

    // Validate column values
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
        return true; // Return true to indicate bust
    }

    return false; // Return false to indicate no bust
}

// Handle roll action
bool Game::handleRoll() {
    try {
        // Roll dice and get values
        pairVals = rollAndDisplayDice();

        // Validate and make moves
        if (!validateDiceValues(pairVals[0], pairVals[1])) {
            return false;  // Let player try again
        }

        // Make moves and check for bust
        return makeMovesAndCheckBust(pairVals[0], pairVals[1]);
    }
    catch(BadChoice& e) {
        e.print(cout);
        cin.clear();
        cin.ignore(10000, '\n');
        return false;
    }
    catch(...) {
        cout << "Unknown error in dice roll. Try again.\n";
        cin.clear();
        cin.ignore(10000, '\n');
        return false;
    }
}

// Validate column values from dice
bool Game::validateDiceValues(int pair1, int pair2) {
    // Validate column values
    bool move1Valid = (pair1 >= 2 && pair1 <= 12);
    bool move2Valid = (pair2 >= 2 && pair2 <= 12);

    if (!move1Valid && !move2Valid) {
        cout << "Error: Both calculated column values invalid.\n";
        return false;
    }

    return true;
}

// Handle stop action with player parameter for checking win
bool Game::handleStop(player* p) {
    board.stop();
    cout << "Turn ended, towers converted to tiles:\n" << board << endl;
    return true; // Return true to indicate the turn is over
}

// Handle resign action
bool Game::handleResign() {
    try {
        bool confirmed = validateResignConfirmation();

        if(confirmed) {
            cout << "Player has resigned from the game.\n";
            players.remove();
            return true; // Turn is over
        } else {
            cout << "Resignation canceled.\n";
            return false; // Turn continues
        }
    }
    catch(BadChoice& e) {
        e.print(cout);
        cin.clear();
        cin.ignore(10000, '\n');
        return false;  // Let player try again
    }
    catch(...) {
        cout << "Unknown error in resign confirmation. Try again.\n";
        cin.clear();
        cin.ignore(10000, '\n');
        return false;  // Let player try again
    }
}

// Check if there's a winner by default (only 1 player left)
ECgame Game::checkWinCondition(player* p) {
    if (players.count() == 1) {
        players.init();
        player* winner = players.next();
        cout << "\n*******************************************\n";
        cout << "*** WINNER by default: " << *winner << " ***\n";
        cout << "*******************************************\n";
        return ECgame::quit; // Game over
    }
    return ECgame::begun; // Continue with next player
}