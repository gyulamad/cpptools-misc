#pragma once

// Text formatting
#define F_BOLD "\033[1m"
#define F_DIM "\033[2m"
#define F_UNDERLINE "\033[4m"
#define F_BLINK "\033[5m"
#define F_REVERSE "\033[7m"
#define F_HIDDEN "\033[8m"

#define F_RESET "\033[0m"

// Text colors
#define F_BLACK "\033[30m"
#define F_RED "\033[31m"
#define F_GREEN "\033[32m"
#define F_YELLOW "\033[33m"
#define F_BLUE "\033[34m"
#define F_MAGENTA "\033[35m"
#define F_CYAN "\033[36m"
#define F_WHITE "\033[37m"

// Bright/Bold colors
#define F_BB_BLACK "\033[1;30m"
#define F_BB_RED "\033[1;31m"
#define F_BB_GREEN "\033[1;32m"
#define F_BB_YELLOW "\033[1;33m"
#define F_BB_BLUE "\033[1;34m"
#define F_BB_MAGENTA "\033[1;35m"
#define F_BB_CYAN "\033[1;36m"
#define F_BB_WHITE "\033[1;37m"

// Background colors
#define F_BG_BLACK "\033[40m"
#define F_BG_RED "\033[41m"
#define F_BG_GREEN "\033[42m"
#define F_BG_YELLOW "\033[43m"
#define F_BG_BLUE "\033[44m"
#define F_BG_MAGENTA "\033[45m"
#define F_BG_CYAN "\033[46m"
#define F_BG_WHITE "\033[47m"

// Bright background colors
#define F_BBG_BLACK "\033[100m"
#define F_BBG_RED "\033[101m"
#define F_BBG_GREEN "\033[102m"
#define F_BBG_YELLOW "\033[103m"
#define F_BBG_BLUE "\033[104m"
#define F_BBG_MAGENTA "\033[105m"
#define F_BBG_CYAN "\033[106m"
#define F_BBG_WHITE "\033[107m"

// User-specific theme
#define F_SUCCESS F_BOLD F_GREEN
#define F_WARNING F_BOLD F_YELLOW
#define F_ALERT F_RED
#define F_ERROR F_BOLD F_RED
#define F_INFO F_BOLD F_BLUE
#define F_NOTE F_BLUE
#define F_DEBUG F_RESET F_CYAN
#define F_STUB F_RESET F_YELLOW
#define F_FILE F_RESET F_CYAN
#define F_FUNC F_BOLD F_WHITE
#define F_HIGHLIGHT F_BOLD F_WHITE
#define F_REASON F_RESET

#define F(fmt, txt) (std::string(F_RESET) + (fmt) + (txt) + F_RESET)
#define F_FILE_LINE(file, line) F(F_FILE, (std::string(file)) + ":" + std::to_string(line))
#define F_CALL(func, file, line) F(F_FUNC, (std::string(func)) + "()" + F_RESET + " at " + F_FILE_LINE((file), (line)))
