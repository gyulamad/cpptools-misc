#pragma once

class Persistable {
public:

    virtual ~Persistable() = default;

    // Shall reset everything to the original state.
    virtual void reset() = 0;

    // Save state (for eg. to file, in case we have to restart...
    virtual void store() = 0;

    // Load state (for eg. from file to continue where it left...
    virtual void restore() = 0;
};
