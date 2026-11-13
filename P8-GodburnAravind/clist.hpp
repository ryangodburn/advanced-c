/*
* clist.hpp - Header file for the CList circular linked list class
 * Author: Ryan Godburn & Arjun Aravind
 * Date: 3/13/25
 * Description: This header defines the CList class, a circular linked list
 *              container that can hold unique pointers to Player objects,
 *              and a private Cell class that serves as its helper.
 */

#ifndef CLIST_HPP
#define CLIST_HPP

#pragma once

#include "tools.hpp"
#include "player.hpp"
#include <memory>

// Forward declaration for CList
class CList;

// Cell class - servant class for CList
class Cell {
private:
    // Make CList a friend to allow access to Cell's private members
    friend class CList;

    // Define the type alias for unique_ptr<player>
    using upp = std::unique_ptr<player>;

    // Data members
    upp data;       // Pointer to the data stored in the Cell
    Cell* next;     // Pointer to the next Cell in the list

    // Constructor
    Cell(upp p, Cell* n = nullptr) : data(std::move(p)), next(n) {}

    // Destructor
    ~Cell() = default;  // Smart pointer handles cleanup of data
};

// ------------------------------------------------------------------------

// CList class - circular linked list container
class CList {
private:
    int counter;    // Number of Cells in the list
    Cell* head;     // Pointer to the first Cell in the list
    Cell* tail;     // Pointer to the last Cell in the list (optional)
    Cell* current;  // Pointer to the current Cell

    // Define the type alias for unique_ptr<player>
    using upp = std::unique_ptr<player>;

public:
    // Constructor and destructor
    CList();
    ~CList();

    // Accessors
    int count() const { return counter; }
    bool empty() const { return counter == 0; }

    // List operations
    void addCell(upp it);
    void init();
    player* next();  // Return pointer to the player in the next Cell
    void remove();

    // Output
    ostream& print(ostream& out);
};

// Stream operator for CList output
inline ostream& operator<<(ostream& out, CList& list) {
    return list.print(out);
}

#endif