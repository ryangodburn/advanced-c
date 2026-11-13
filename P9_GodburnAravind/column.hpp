/*
* column.hpp - Header file for the Column class.
 * Author: Ryan Godburn & Arjun Aravind
 * Date: 2/5/25
 * Description: This header file defines the Column class which
 *              represents a single column in the game board. Each
 *              column can contain colored tiles and a tower marker,
 *              and can be in different states (available, pending,
 *              or captured).
 */

#ifndef COLUMN_HPP
#define COLUMN_HPP

#pragma once

#include "tools.hpp"
#include "enums.hpp"
#include "player.hpp"

class Column {
private:
    static const int lens[13];  // Column lengths
    const int num;
    ECstate cstate = ECstate::avail;
    int pos[5] = {0, 0, 0, 0, 0};  // Initialized with constexpr
    int columnLength;

public:
    Column(int n);
    ~Column() = default;

    ECstate state() const { return cstate; }
    bool startTower(player* p);
    bool move();
    void stop(player* p);
    void bust();
    ostream& print(ostream& out);
};

// Stream operator
inline ostream& operator<<(ostream& out, Column& col) {
    return col.print(out);
}

#endif