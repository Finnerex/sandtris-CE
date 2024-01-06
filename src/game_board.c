#include <string.h>
#include <debug.h>
#include "game_board.h"

#define can_move_side(fd) (y > 0 && (x + fd >= 0 && x + fd < GAME_WIDTH))

// bouta be slow as hell
void update_columns(pixel_t pixels[GAME_WIDTH][GAME_HEIGHT]) {
    // memcpy(old_pixels, pixels, sizeof (pixel_t) * 56 * 118);
    static int fall_direction = 1; // 1 for right, -1 for left

    for (int x = 0; x < GAME_WIDTH; x++) {

        bool below_is_empty = false; // there is something stopping them at the bottom

        for (int y = 0; y < GAME_HEIGHT; y++) { // there is no way this will ever be fast ever

            pixel_t* pixel = &/* old_ */pixels[x][y];
            
            int x_change = 0;
            int y_change = 0;

            if (pixel->color != NONE) {
                if (below_is_empty)
                    y_change = -1;

                // if the one below to the side is free then the one above it should also be free (hopefully)
                // but like maybe not if it hasnt updated it yet (to the right) not anymore hahaha
                else if (can_move_side(fall_direction) && pixels[x + fall_direction][y - 1].color == NONE && pixels[x + fall_direction][y].color == NONE) {
                    x_change = fall_direction;
                    y_change = -1;

                } else if (can_move_side(-fall_direction) && pixels[x - fall_direction][y - 1].color == NONE && pixels[x - fall_direction][y].color == NONE) {
                    x_change = -fall_direction;
                    y_change = -1;

                }
            }

            
            if (!pixel->has_moved && (x_change != 0 || y_change != 0)) {
                /* pixel_t* new_pixel = &pixels[x][y]; */

                dbg_printf("Moved from (%i, %i) to (%i, %i)\n", x, y, x + x_change, y + y_change);

                pixels[x + x_change][y + y_change].color = pixel->color;
                pixels[x + x_change][y + y_change].has_moved = true;
                /* new_ */pixel->color = NONE;
            }

            pixel->has_moved = false;
                

            below_is_empty = pixels[x][y].color == NONE;

            // now maybe check for l/r connection idk how tf im sposta do that
            if (pixel->color == NONE) continue;
            
            if (x == 0) {
                pixel->is_connected_left = true;
                continue;
            }

            // c u u // how the hell do i determine if the bottom center one is connect left
            // c u u // if i dont know that the one above is connected left
            // x c u // maybe i just ignore it and hope in never happens (it will happens)
            
            pixel->is_connected_left =
                (pixels[x - 1][y].is_connected_left && pixels[x - 1][y].color == pixel->color) ||
                (pixels[x][y - 1].is_connected_left && pixels[x][y - 1].color == pixel->color) /*|| 
                (pixels[x][y + 1].is_connected_left && pixels[x][y + 1].t == pixel->t)*/;

            if (pixel->is_connected_left && x >= GAME_WIDTH - 1)
                clear_from(x, y, pixel->color, pixels);
            
        }
    }

    fall_direction *= -1; // swap direction every frame
  
}

// this will be slow but that should be okay hopefully
void clear_from(int x, int y, pixel_color color, pixel_t pixels[GAME_WIDTH][GAME_HEIGHT]) {
    pixel_t* pixel = &pixels[x][y];

    if (pixel->color != color)
        return;

    pixel->color = NONE;
    pixel->was_cleared = true;

    // should have to check right side because of goofy overhang things that could happen
    // make this so it doesnt check the one that called this eventually (probably using a parameter)
    clear_from(x, y + 1, color, pixels);
    clear_from(x, y - 1, color, pixels);
    clear_from(x + 1, y, color, pixels);
    clear_from(x - 1, y, color, pixels);
    // maybe adding another layer of indirection will make this faster????? idk how programming works
  
}