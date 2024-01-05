#include "game_board.h"

// bouta be slow as hell
void update_columns(pixel_t pixels[56][118]) {
    static int fall_direction = 1; // 1 for right, -1 for left

    for (int x = 0; x < 56; x++) {

        bool below_is_empty = false; // there is something stopping them at the bottom

        for (int y = 0; y < 118; y++) { // there is no way this will ever be fast ever

            pixel_t* pixel = &pixels[x][y];

            int x_change = 0;
            int y_change = -1;

            // if the one below to the side is free then the one above that should also be free (hopefully)
            // but like maybe not if it hasnt updated it yet (to the right)
            if (!below_is_empty && pixels[x + fall_direction][y - 1].color == none) // is this slower?
                x_change = fall_direction;
            
            else if (!below_is_empty && pixels[x - fall_direction][y - 1].color == none)
                x_change = -fall_direction;
            
            else
                y_change = 0;

            if (x_change != 0 || y_change != 0) {
                pixels[x + x_change][y + y_change].color = pixel->color;
                pixel->color = none;
            }
            
            below_is_empty = pixels[x][y].color == none;

            // now maybe check for l/r connection idk how tf im sposta do that
            if (pixel->color == none) continue;
            
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

            if (pixel->is_connected_left && x == 53)
                clear_from(x, y, pixel->color, pixels);
            
        }
    }

    fall_direction *= -1; // swap direction every frame
  
}

// this will be slow but that should be okay hopefully
void clear_from(int x, int y, pixel_color color, pixel_t pixels[56][118]) {
    pixel_t* pixel = &pixels[x][y];

    if (pixel->color != color)
        return;

    pixel->color = none;
    pixel->was_cleared = true;

    // should have to check right side because of goofy overhang things that could happen
    // make this so it doesnt check the one that called this eventually (probably using a parameter)
    clear_from(x, y + 1, color, pixels);
    clear_from(x, y - 1, color, pixels);
    clear_from(x + 1, y, color, pixels);
    clear_from(x - 1, y, color, pixels);
    // maybe adding another layer of indirection will make this faster????? idk how programming works
  
}