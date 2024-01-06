// not a great name for the file but oh well

#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>

#define GAME_WIDTH  56
#define GAME_HEIGHT 118

typedef enum { NONE, RED, YELLOW, BLUE, GREEN } pixel_color;

typedef struct {
//   int x; // might not even need these jawns
//   int y; // ^^^
  pixel_color color;
  bool is_connected_left;
  bool was_cleared;
  bool has_moved;
} pixel_t;



void update_columns(pixel_t pixels[GAME_WIDTH][GAME_HEIGHT]);
void clear_from(int x, int y, pixel_color color, pixel_t pixels[GAME_WIDTH][GAME_HEIGHT]);

#endif