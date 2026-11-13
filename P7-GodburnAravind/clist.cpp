/*
* clist.cpp - Implementation file for the CList circular linked list class
 * Author: Ryan Godburn & Arjun Aravind
 * Date: 3/13/25
 * Description: This source file implements the CList class methods
 *              defined in clist.hpp.
 */

#include "clist.hpp"

// Constructor - Initialize an empty list
CList::CList() : counter(0), head(nullptr), tail(nullptr), current(nullptr) {}

// Destructor - Clean up all Cells in the list
CList::~CList() {
    if(empty()) return;  // Nothing to clean up

    // Start at the head and delete each Cell
    Cell* temp = head;
    Cell* next_cell;

    // Loop through the circular list, stopping when we reach the head again
    do {
        next_cell = temp->next;
        delete temp;
        temp = next_cell;
    } while(temp != head);

    // Reset list state
    head = nullptr;
    tail = nullptr;
    current = nullptr;
    counter = 0;
}

// Add a new Cell to the list
void CList::addCell(upp it) {
    // Create a new Cell with the provided player
    Cell* new_cell = new Cell(std::move(it));

    if(empty()) {
        // Special case: first Cell in the list
        head = new_cell;
        tail = new_cell;
        new_cell->next = new_cell;  // Point to itself (circular)
    } else {
        // Add to the end of the list
        new_cell->next = head;  // Point to head (circular)
        tail->next = new_cell;  // Current tail points to new Cell
        tail = new_cell;        // Update tail pointer
    }

    // Increment count
    counter++;
}

// Initialize the current pointer to the head of the list
void CList::init() {
    current = head;  // Start at the head of the list
}

// Move to the next Cell and return pointer to its player
player* CList::next() {
    if(empty()) return nullptr;  // Handle empty list

    current = current->next;  // Move to the next Cell
    return current->data.get();  // Return raw pointer to player
}

// Remove the current Cell from the list
void CList::remove() {
    if(empty()) return;  // Handle empty list

    if(counter == 1) {
        // Special case: last Cell in the list
        delete head;
        head = nullptr;
        tail = nullptr;
        current = nullptr;
        counter = 0;
        return;
    }

    // Find the Cell before current
    Cell* prev = head;
    while(prev->next != current) prev = prev->next;

    // Update pointers
    if(current == head) {
        head = current->next;  // Update head if removing head
    }

    if(current == tail) {
        tail = prev;  // Update tail if removing tail
    }

    // Link previous Cell to next Cell
    prev->next = current->next;

    // Store the next Cell before deleting current
    Cell* next_cell = current->next;

    // Delete current Cell
    delete current;

    // Update current to next Cell
    current = next_cell;

    // Decrement count
    counter--;
}

// Print all Cells in the list
ostream& CList::print(ostream& out) {
    if(empty()) {
        out << "Empty list";
        return out;
    }

    // Start at the head
    Cell* temp = head;

    // Loop through the circular list, stopping when we reach the head again
    do {
        // Print the player data (using player's print method)
        player* p = temp->data.get();
        out << *p << endl;

        temp = temp->next;
    } while(temp != head);

    return out;
}