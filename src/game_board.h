// not a great name for the file but oh well

#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>

typedef enum { none, red, yellow, blue, green } pixel_color;

typedef struct {
//   int x; // might not even need these jawns
//   int y; // ^^^
  pixel_color color;
  bool is_connected_left;
  bool was_cleared;
} pixel_t;



void update_columns(pixel_t pixels[56][118]);
void clear_from(int x, int y, pixel_color color, pixel_t pixels[56][118]);

#endif