#pragma once

#include <string>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

static pair<int, int> get_terminal_size() {
    // Open the controlling terminal device directly.
    int fd = open("/dev/tty", O_RDONLY);
    if (fd >= 0) {
        struct winsize w{};
        if (ioctl(fd, TIOCGWINSZ, &w) == 0 && w.ws_col > 0 && w.ws_row > 0) {
            close(fd);
            return {(int)w.ws_col, (int)w.ws_row};
        }
        close(fd);
    }

    // Fallback to environment variables.
    char* cols = getenv("COLUMNS");
    char* rows = getenv("LINES");
    int c = cols ? stoi(cols) : 80;
    int r = rows ? stoi(rows) : 24;
    return {c, r};
}
