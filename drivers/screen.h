#pragma once
#include <stdint.h>

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f // Attributes byte
#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5

void print(char* message);
void clear_screen();
void print_at(char* message, int16_t col, int16_t row) ;
void print_backspace();
