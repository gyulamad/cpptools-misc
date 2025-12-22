#pragma once

#include "ERROR.hpp"
#include <vector>

using namespace std;

class Stopper {
public:
    Stopper(bool start = true) { if (start) this->start(); }
    virtual ~Stopper() {}

    // Start the stopwatch (throws if already running)
    void start() {
        if (started)
            throw ERROR("Stopper start error, stopper is already started.");
        started = true;
        startTime = chrono::high_resolution_clock::now();
    }

    // Pause the stopwatch (throws if not running)
    void pause() {
        if (!started)
            throw ERROR("Stopper pause error, stopper is not started.");
        paused = true;
        elapsed += chrono::duration_cast<chrono::nanoseconds>(
            chrono::high_resolution_clock::now() - startTime
        ).count();
    }

    // Resume the stopwatch (throws if already running)
    void resume() {
        if (!paused)
            throw ERROR("Stopper resume error, stopper is not paused");
        paused = false;
        startTime = chrono::high_resolution_clock::now();
    }

    // Stop the stopwatch and return elapsed time in milliseconds (throws if not running)
    double stop() {
        if (!started)
            throw ERROR("Stopper stop error, stopper is not started");
        started = false;
        elapsed += chrono::duration_cast<chrono::nanoseconds>(
            chrono::high_resolution_clock::now() - startTime
        ).count();
        double elapsedMs = elapsed / 1'000'000.0; // Convert nanoseconds to milliseconds
        // reset(); // Reset for next use
        return elapsedMs;
    }

    // Get elapsed time so far in milliseconds (works whether running or paused)
    double getElapsedMs() const {
        //if (started && !paused) {
        if (isRunning()) {
            auto current = elapsed + 
                chrono::duration_cast<chrono::nanoseconds>(
                    chrono::high_resolution_clock::now() - startTime
                ).count();
            return current / 1'000'000.0; // Convert to milliseconds
        }
        return elapsed / 1'000'000.0; // Convert to milliseconds
    }

    // Reset the stopwatch to initial state
    void reset() {
        started = true;
        paused = false;
        elapsed = 0;
        startTime = chrono::high_resolution_clock::now();
    }

    // Check if the stopwatch is running
    bool isRunning() const {
        return started && !paused;
    }

    string toString() const {
        double elapsedMs = getElapsedMs();
        // Round to nearest millisecond
        long long total_milliseconds = static_cast<long long>(elapsedMs + 0.5);

        if (total_milliseconds == 0) {
            return "0 milliseconds";
        }

        long long seconds_total = total_milliseconds / 1000;
        long long milliseconds = total_milliseconds % 1000;

        long long minutes_total = seconds_total / 60;
        long long seconds = seconds_total % 60;

        long long hours_total = minutes_total / 60;
        long long minutes = minutes_total % 60;

        long long days_total = hours_total / 24;
        long long hours = hours_total % 24;

        long long years = days_total / 365;
        long long days_remaining = days_total % 365;

        long long months = days_remaining / 30;
        days_remaining %= 30;

        long long weeks = days_remaining / 7;
        long long days = days_remaining % 7;

        vector<pair<long long, string>> parts;

        if (years > 0) 
            parts.push_back({years, years == 1 ? "year" : "years"});
        if (months > 0) 
            parts.push_back({months, months == 1 ? "month" : "months"});
        if (weeks > 0) 
            parts.push_back({weeks, weeks == 1 ? "week" : "weeks"});
        if (days > 0) 
            parts.push_back({days, days == 1 ? "day" : "days"});
        if (hours > 0) 
            parts.push_back({hours, hours == 1 ? "hour" : "hours"});
        if (minutes > 0) 
            parts.push_back({minutes, minutes == 1 ? "minute" : "minutes"});
        if (seconds > 0) 
            parts.push_back({seconds, seconds == 1 ? "second" : "seconds"});
        if (milliseconds > 0) 
            parts.push_back({milliseconds, milliseconds == 1 ? "millisecond" : "milliseconds"});

        string result;
        for (size_t i = 0; i < parts.size(); i++) {
            if (i > 0) {
                result += ", ";
            }
            result += to_string(parts[i].first) + " " + parts[i].second;
        }
        return result;
    }

protected:
    bool started = false, paused = false;
    long long elapsed = 0; // Accumulated time in nanoseconds
    chrono::time_point<chrono::high_resolution_clock> startTime;
};
