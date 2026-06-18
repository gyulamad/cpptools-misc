#pragma once

// EGA color palette
static const unsigned int EGA_COLORS[16] = {
    0x00000000, // Black
    0x0000AA00, // Blue
    0x00AA0000, // Green
    0x00AAAA00, // Cyan
    0xAA000000, // Red
    0xAA00AA00, // Magenta
    0xAA550000, // Brown
    0xAAAAAA00, // Light Gray
    0x55555500, // Dark Gray
    0x5555FF00, // Light Blue
    0x55FF5500, // Light Green
    0x55FFFF00, // Light Cyan
    0xFF555500, // Light Red
    0xFF55FF00, // Light Magenta
    0xFFFF5500, // Yellow
    0xFFFFFF00,  // White
};
enum EGA { BLACK = 0, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, LIGHT_GRAY, DARK_GRAY, LIGHT_BLUE, LIGHT_GREEN, LIGHT_CYAN, LIGHT_RED, LIGHT_MAGENTA, YELLOW, WHITE };

#define EGA_BLACK           EGA_COLORS[EGA::BLACK]
#define EGA_BLUE            EGA_COLORS[EGA::BLUE]
#define EGA_GREEN           EGA_COLORS[EGA::GREEN]
#define EGA_CYAN            EGA_COLORS[EGA::CYAN]
#define EGA_RED             EGA_COLORS[EGA::RED]
#define EGA_MAGENTA         EGA_COLORS[EGA::MAGENTA]
#define EGA_BROWN           EGA_COLORS[EGA::BROWN]
#define EGA_LIGHT_GRAY      EGA_COLORS[EGA::LIGHT_GRAY]
#define EGA_DARK_GRAY       EGA_COLORS[EGA::DARK_GRAY]
#define EGA_LIGHT_BLUE      EGA_COLORS[EGA::LIGHT_BLUE]
#define EGA_LIGHT_GREEN     EGA_COLORS[EGA::LIGHT_GREEN]
#define EGA_LIGHT_CYAN      EGA_COLORS[EGA::LIGHT_CYAN]
#define EGA_LIGHT_RED       EGA_COLORS[EGA::LIGHT_RED]
#define EGA_LIGHT_MAGENTA   EGA_COLORS[EGA::LIGHT_MAGENTA]
#define EGA_YELLOW          EGA_COLORS[EGA::YELLOW]
#define EGA_WHITE           EGA_COLORS[EGA::WHITE]

