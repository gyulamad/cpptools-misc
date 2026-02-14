#pragma once

#ifdef TEST

#include "../TEST.hpp"
#include "../TeeStreamBuf.hpp"
#include "../capture_cout.hpp"
#include <sstream>

using namespace std;

// Test for TeeStreamBuf constructor
TEST(test_TeeStreamBuf_constructor) {
    stringstream console_buffer;
    stringstream capture_buffer;
    TeeStreamBuf buf(console_buffer.rdbuf(), capture_buffer.rdbuf());
    // Constructor should work
}

// Test for overflow method with non-EOF character
TEST(test_TeeStreamBuf_overflow_non_eof) {
    stringstream console_buffer;
    stringstream capture_buffer;
    TeeStreamBuf buf(console_buffer.rdbuf(), capture_buffer.rdbuf());
    
    // Write a character
    int result = buf.sputc('A');
    
    // Verify the character was written to both buffers
    assert(result == 'A' && "sputc should return the character written");
    
    // Check console buffer
    console_buffer.flush();
    assert(console_buffer.str() == "A" && "Console buffer should contain 'A'");
    
    // Check capture buffer
    capture_buffer.flush();
    assert(capture_buffer.str() == "A" && "Capture buffer should contain 'A'");
}

// Test class that exposes protected methods
class TestTeeStreamBuf : public TeeStreamBuf {
public:
    TestTeeStreamBuf(streambuf* console, streambuf* capture)
        : TeeStreamBuf(console, capture) {}
    
    int testOverflow(int c) {
        return overflow(c);
    }
};

// Test for overflow method with EOF
TEST(test_TeeStreamBuf_overflow_eof) {
    stringstream console_buffer;
    stringstream capture_buffer;
    TestTeeStreamBuf buf(console_buffer.rdbuf(), capture_buffer.rdbuf());
    
    // Call overflow directly with EOF
    int result = buf.testOverflow(EOF);
    
    // Verify EOF is returned
    assert(result == EOF && "overflow should return EOF when called with EOF");
}

// Test for sync method
TEST(test_TeeStreamBuf_sync) {
    stringstream console_buffer;
    stringstream capture_buffer;
    TeeStreamBuf buf(console_buffer.rdbuf(), capture_buffer.rdbuf());
    
    // Write some data
    buf.sputc('X');
    buf.sputc('Y');
    buf.sputc('Z');
    
    // Sync the buffers
    int result = buf.pubsync();
    
    // Verify sync succeeded
    assert(result == 0 && "sync should return 0 on success");
    
    // Check both buffers have the data
    console_buffer.flush();
    capture_buffer.flush();
    assert(console_buffer.str() == "XYZ" && "Console buffer should contain 'XYZ'");
    assert(capture_buffer.str() == "XYZ" && "Capture buffer should contain 'XYZ'");
}

// Test for multiple characters written
TEST(test_TeeStreamBuf_multiple_writes) {
    stringstream console_buffer;
    stringstream capture_buffer;
    TeeStreamBuf buf(console_buffer.rdbuf(), capture_buffer.rdbuf());
    
    // Write multiple characters
    buf.sputc('H');
    buf.sputc('e');
    buf.sputc('l');
    buf.sputc('l');
    buf.sputc('o');
    
    // Sync to flush
    buf.pubsync();
    
    // Check both buffers
    assert(console_buffer.str() == "Hello" && "Console buffer should contain 'Hello'");
    assert(capture_buffer.str() == "Hello" && "Capture buffer should contain 'Hello'");
}

// Test for TeeStreamBuf with capture_cout helper
TEST(test_TeeStreamBuf_with_capture_cout) {
    string output = capture_cout([]() {
        cout << "Test message";
    });
    // Verify capture worked
    assert(output.find("Test message") != string::npos && "Capture should contain 'Test message'");
}

#endif