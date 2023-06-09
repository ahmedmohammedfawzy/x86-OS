#include "screen.h"
#include "../libc/mem.h"
#include "port.h"
#include <stdint.h>

/* Declaration of private functions */
uintptr_t get_cursor_offset();
void set_cursor_offset(uintptr_t offset);
uintptr_t get_offset(int16_t col, int16_t row);
uintptr_t print_char(char character, int16_t col, int16_t row,
                     uint8_t attribute_byte);
void print_at(char *message, int16_t col, int16_t row);
uint16_t get_row_from_offset(uintptr_t offset);
uint16_t get_col_from_offest(uintptr_t offset);
uintptr_t handle_scrolling(uintptr_t offset);

void print(char *message) { print_at(message, -1, -1); }

void clear_screen() {
  int screen_size = MAX_COLS * MAX_ROWS;
  int i;
  char *screen = (char *)VIDEO_ADDRESS;

  for (i = 0; i < screen_size; i++) {
    screen[i * 2] = ' ';
    screen[i * 2 + 1] = WHITE_ON_BLACK;
  }
  set_cursor_offset(get_offset(0, 0));
}

void print_backspace() {
  uintptr_t offset = get_cursor_offset() - 2;
  uint16_t row = get_row_from_offset(offset);
  uint16_t col = get_col_from_offest(offset);
  print_char(0x08, col, row, WHITE_ON_BLACK);
}

void print_at(char *message, int16_t col, int16_t row) {
  /* Set cursor if col/row are negative */
  uint16_t offset;
  if (col >= 0 && row >= 0)
    offset = get_offset(col, row);
  else {
    offset = get_cursor_offset();
    row = get_row_from_offset(offset);
    col = get_col_from_offest(offset);
  }

  /* Loop through message and print it */
  int i = 0;
  while (message[i] != 0) {
    offset = print_char(message[i++], col, row, WHITE_ON_BLACK);
    /* Compute row/col for next iteration */
    row = get_row_from_offset(offset);
    col = get_col_from_offest(offset);
  }
}

/**********************************************************
 * Private kernel functions                               *
 **********************************************************/

/**
 * Innermost print function for our kernel, directly accesses the video memory
 *
 * If 'col' and 'row' are negative, we will print at current cursor location
 * If 'attr' is zero it will use 'white on black' as default
 * Returns the offset of the next character
 * Sets the video cursor to the returned offset
 */

uintptr_t print_char(char character, int16_t col, int16_t row,
                     uint8_t attribute_byte) {
  // This function will print character to specific location
  // or append it at the cursor position if col and row is negative
  unsigned char *video_memory = (unsigned char *)VIDEO_ADDRESS;

  if (attribute_byte == 0) {
    attribute_byte = WHITE_ON_BLACK;
  }

  int offset;
  if (col >= 0 && row >= 0) {
    offset = get_offset(col, row);
  } else {
    offset = get_cursor_offset();
  }

  if (character == '\n') {
    int row = get_row_from_offset(offset);
    offset = get_offset(0, row + 1);
  } else if (character == 0x08) // Backspace
  {
    video_memory[offset] = ' ';
    video_memory[offset + 1] = attribute_byte;
  } else {
    video_memory[offset] = character;
    video_memory[offset + 1] = attribute_byte;
    offset += 2;
  }

  offset = handle_scrolling(offset);

  set_cursor_offset(offset);
  return offset;
}

uintptr_t get_cursor_offset() {
  // The device uses its control register as an index
  // to select its internal registers , of which we are
  // interested in :
  // reg 14: which is the high byte of the cursor ’s offset
  // reg 15: which is the low byte of the cursor ’s offset
  // Once the internal register has been selected , we may read or
  // write a byte on the data register.

  port_byte_out(REG_SCREEN_CTRL, 14);
  int offset = port_byte_in(REG_SCREEN_DATA) << 8;
  port_byte_out(REG_SCREEN_CTRL, 15);
  offset += port_byte_in(REG_SCREEN_DATA);

  // Since the cursor offset reported by the VGA hardware is the
  // number of characters , we multiply by two to convert it to
  // a character cell offset .
  return offset * 2;
}

void set_cursor_offset(uintptr_t offset) {
  /* Similar to get_cursor_offset, but instead of reading we write data */
  offset /= 2;
  port_byte_out(REG_SCREEN_CTRL, 14);
  port_byte_out(REG_SCREEN_DATA, (offset >> 8));
  port_byte_out(REG_SCREEN_CTRL, 15);
  port_byte_out(REG_SCREEN_DATA, (offset & 0xff));
}

uintptr_t handle_scrolling(uintptr_t offset) {
  if (offset >= MAX_COLS * MAX_ROWS * 2) {
    for (int i = 1; i <= MAX_ROWS; i++) {
      memory_copy((char *)VIDEO_ADDRESS + get_offset(0, i - 1),
                  (char *)VIDEO_ADDRESS + get_offset(0, i), MAX_COLS * 2);
    }

    char *last_line = (char *)(get_offset(0, MAX_ROWS - 1) + VIDEO_ADDRESS);
    for (int i = 0; i < MAX_COLS * 2; i++)
      last_line[i] = 0;

    offset -= 2 * MAX_COLS;
  }

  return offset;
}

uintptr_t get_offset(int16_t col, int16_t row) {
  return 2 * (row * MAX_COLS + col);
}
uint16_t get_row_from_offset(uintptr_t offset) {
  return offset / (2 * MAX_COLS);
}
uint16_t get_col_from_offest(uintptr_t offset) {
  return (offset - (get_row_from_offset(offset) * 2 * MAX_COLS)) / 2;
}
