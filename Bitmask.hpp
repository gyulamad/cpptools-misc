#pragma once

using namespace std;

// Bitmask class to manage configuration flags
class Bitmask {
private:
    int mask; // Stores the current state of all flags

public:
    // Constructor: Initialize with a starting value (default 0)
    Bitmask(int initial = 0) : mask(initial) {}

    // Set one or multiple flags using bitwise OR
    void set(int flags) {
        mask |= flags;
    }

    // Clear one or multiple flags using bitwise AND with NOT
    void clear(int flags) {
        mask &= ~flags;
    }

    // Check if one or more flags are set using bitwise AND
    bool get(int flags) const {
        return (mask & flags) == flags;
    }

    // Get the raw mask value
    int value() const {
        return mask;
    }
};
