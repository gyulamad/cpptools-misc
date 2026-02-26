#pragma once

#include <signal.h>
#include <atomic>
#include <functional>

using namespace std;

/**
 * @brief SignalHandler - A universal signal handler for graceful shutdown
 * 
 * This is a singleton-style class designed to be instantiated once globally
 * in the application. It provides a clean abstraction over signal handling
 * that works well with C++ RAII principles.
 * 
 * DESIGN PHILOSOPHY:
 * Signal handling is a cross-cutting concern that sits at the boundary
 * between the application and the OS. Similar to logging, it's a system-level
 * concern that benefits from a universal approach.
 * 
 * USAGE:
 * 1. Create a global instance: SignalHandler signalHandler;
 * 2. Set callbacks: signalHandler.setSigIntHandler(myCallback);
 * 3. Check flag: if (signalHandler.signalReceived()) { shutdown(); }
 * 
 * The destructor automatically restores previous signal handlers.
 */
inline class SignalHandler {
public:
    using SignalCallback = function<void(SignalHandler*, int)>;

    SignalHandler() {
        mPreviousSigIntHandler = SIG_DFL;
        mPreviousSigTermHandler = SIG_DFL;

        // Setup signal handlers for graceful shutdown using the SignalHandler class
        setSigIntHandler(onSignalReceived);
        setSigTermHandler(onSignalReceived);
    }

    ~SignalHandler() {
        restoreHandlers();
    }

    // Non-copyable
    SignalHandler(const SignalHandler&) = delete;
    SignalHandler& operator=(const SignalHandler&) = delete;

    // Move constructor
    SignalHandler(SignalHandler&& other) {
        mSigIntCallback = ::move(other.mSigIntCallback);
        mSigTermCallback = ::move(other.mSigTermCallback);
        mPreviousSigIntHandler = other.mPreviousSigIntHandler;
        mPreviousSigTermHandler = other.mPreviousSigTermHandler;
        other.mPreviousSigIntHandler = SIG_DFL;
        other.mPreviousSigTermHandler = SIG_DFL;
    }

    // Move assignment
    SignalHandler& operator=(SignalHandler&& other) {
        if (this != &other) {
            restoreHandlers();
            mSigIntCallback = ::move(other.mSigIntCallback);
            mSigTermCallback = ::move(other.mSigTermCallback);
            mPreviousSigIntHandler = other.mPreviousSigIntHandler;
            mPreviousSigTermHandler = other.mPreviousSigTermHandler;
            other.mPreviousSigIntHandler = SIG_DFL;
            other.mPreviousSigTermHandler = SIG_DFL;
        }
        return *this;
    }

    /**
     * @brief Set a callback for SIGINT
     * @param callback The callback function to call when SIGINT is received
     */
    void setSigIntHandler(SignalCallback callback) {
        mSigIntCallback = callback;
        setupHandler(SIGINT, &mPreviousSigIntHandler);
    }

    /**
     * @brief Set a callback for SIGTERM
     * @param callback The callback function to call when SIGTERM is received
     */
    void setSigTermHandler(SignalCallback callback) {
        mSigTermCallback = callback;
        setupHandler(SIGTERM, &mPreviousSigTermHandler);
    }

    /**
     * @brief Check if a signal was received
     * @return true if SIGINT or SIGTERM was received
     */
    bool signalReceived() const {
        return mSignalReceived.load(memory_order_acquire);
    }

    /**
     * @brief Get the received signal number
     * @return The signal number (SIGINT or SIGTERM), or 0 if none received
     */
    int getReceivedSignal() const {
        return mReceivedSignal.load(memory_order_acquire);
    }

    /**
     * @brief Reset the signal state
     */
    void reset() {
        mSignalReceived.store(false, memory_order_release);
        mReceivedSignal.store(0, memory_order_release);
    }

    /**
     * @brief Restore previous signal handlers
     */
    void restoreHandlers() {
        if (mPreviousSigIntHandler != SIG_DFL && mPreviousSigIntHandler != SIG_IGN) {
            struct sigaction sa;
            sa.sa_handler = mPreviousSigIntHandler;
            sigemptyset(&sa.sa_mask);
            sa.sa_flags = SA_RESTART;
            sigaction(SIGINT, &sa, nullptr);
        }
        if (mPreviousSigTermHandler != SIG_DFL && mPreviousSigTermHandler != SIG_IGN) {
            struct sigaction sa;
            sa.sa_handler = mPreviousSigTermHandler;
            sigemptyset(&sa.sa_mask);
            sa.sa_flags = SA_RESTART;
            sigaction(SIGTERM, &sa, nullptr);
        }
        mPreviousSigIntHandler = SIG_DFL;
        mPreviousSigTermHandler = SIG_DFL;
    }

    bool isSignalReceived() {
        return _signalReceived;
    }

private:
    static void signalHandlerWrapper(int sig) {
        // Get the static instance
        static SignalHandler instance;
        
        // Set the atomic flag
        instance.mSignalReceived.store(true, memory_order_release);
        instance.mReceivedSignal.store(sig, memory_order_release);

        // Call the appropriate callback
        if (sig == SIGINT && instance.mSigIntCallback) {
            instance.mSigIntCallback(&instance, sig);
        } else if (sig == SIGTERM && instance.mSigTermCallback) {
            instance.mSigTermCallback(&instance, sig);
        }

        // Call previous handler if it wasn't SIG_DFL or SIG_IGN
        void (*prevHandler)(int) = SIG_DFL;
        if (sig == SIGINT) {
            prevHandler = instance.mPreviousSigIntHandler;
        } else if (sig == SIGTERM) {
            prevHandler = instance.mPreviousSigTermHandler;
        }
        
        if (prevHandler != SIG_DFL && prevHandler != SIG_IGN) {
            prevHandler(sig);
        }
    }

    void setupHandler(int signum, void (**prevHandler)(int)) {
        struct sigaction sa;
        sa.sa_handler = signalHandlerWrapper;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;
        
        if (sigaction(signum, &sa, reinterpret_cast<struct sigaction*>(prevHandler)) < 0) {
            // Handle error - could throw or log
        }
    }

    SignalCallback mSigIntCallback;
    SignalCallback mSigTermCallback;
    void (*mPreviousSigIntHandler)(int);
    void (*mPreviousSigTermHandler)(int);

    atomic<bool> mSignalReceived{false};
    atomic<int> mReceivedSignal{0};

    atomic<bool> _signalReceived;

    // Signal callback function
    static void onSignalReceived(SignalHandler* self, int sig) {
        (void)sig;
        self->_signalReceived = true;
    }
} signalHandler;

// Global instance - use this in your main application
// SignalHandler signalHandler;





