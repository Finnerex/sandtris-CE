#include <graphx.h>
#include <keypadc.h>
#include <ti/screen.h>
#include "gfx/gfx.h"

#include "game_board.h"

void begin();
void end();
bool step();
void draw();

int main() {
    begin();                // No rendering allowed!

    gfx_Begin();
    gfx_SetPalette(global_palette, sizeof_global_palette, 0);

    gfx_SetDrawBuffer();    // Draw to the buffer to avoid rendering artifacts

    while (step()) {        // No rendering allowed in step!
        draw();             // As little non-rendering logic as possible
        gfx_SwapDraw();     // Queue the buffered frame to be displayed
    }

    gfx_End();
    end();

    return 0;
}


pixel_t pixels[GAME_WIDTH][GAME_HEIGHT];
// pixel_t old_pixels[GAME_WIDTH][GAME_HEIGHT];

int placer_x = 30;
int placer_y = 80;
pixel_color placer_type = RED;

void begin() {
    // for (int i = 0; i < 56; i++) {
    //     for (int j = 0; j < 118; j++) {
    //         pixel_t* pixel = &pixels[i][j];
            
    //         pixel->x = i;
    //         pixel->y = j;
    //     }
    // }
}

bool step() {
    
    kb_Scan();

    if (kb_Data[6] & kb_Clear)
        return false;


    if (kb_Data[7] & kb_Up)
        placer_y++;
    if (kb_Data[7] & kb_Down)
        placer_y--;

    if (kb_Data[7] & kb_Right)
        placer_x++;
    if (kb_Data[7] & kb_Left)
        placer_x--;


    if (kb_Data[6] & kb_Enter) {
        if (placer_type == RED) {
            for (int ix = 0; ix <= 12; ix++) {
                for (int iy = 4; iy >= 0; iy--) {
                    int x_point = placer_x + ix;
                    int y_point = placer_y + iy;
                    if (x_point >= GAME_WIDTH || x_point < 0)
                        continue;
                    pixels[x_point][y_point].color = placer_type;
                }
            }

            for (int ix = 0; ix <= 4; ix++) {
                for (int iy = -1; iy >= -5; iy--) {
                    int x_point = placer_x + ix;
                    int y_point = placer_y + iy;
                    if (x_point >= GAME_WIDTH || x_point < 0)
                        continue;
                    pixels[x_point][y_point].color = placer_type;
                }
            }
        } else if (placer_type == YELLOW) {
            for (int ix = 0; ix < 8; ix++) {
                for (int iy = 0; iy < 8; iy++) {
                    int x_point = placer_x + ix;
                    int y_point = placer_y + iy;
                    if (x_point >= GAME_WIDTH || x_point < 0)
                        continue;
                    pixels[x_point][y_point].color = placer_type;
                }
            }
        } else if (placer_type == BLUE) {
            for (int ix = 4; ix < 8; ix++) {
                for (int iy = 0; iy < 4; iy++) {
                    int x_point = placer_x + ix;
                    int y_point = placer_y + iy;
                    if (x_point >= GAME_WIDTH || x_point < 0)
                        continue;
                    pixels[x_point][y_point].color = placer_type;
                }
            }
            for (int ix = 0; ix < 12; ix++) {
                for (int iy = 4; iy < 8; iy++) {
                    int x_point = placer_x + ix;
                    int y_point = placer_y + iy;
                    if (x_point >= GAME_WIDTH || x_point < 0)
                        continue;
                    pixels[x_point][y_point].color = placer_type;
                }
            }
        } else if (placer_type == GREEN) {
            for (int ix = 4; ix < 12; ix++) {
                for (int iy = 0; iy < 4; iy++) {
                    int x_point = placer_x + ix;
                    int y_point = placer_y + iy;
                    if (x_point >= GAME_WIDTH || x_point < 0)
                        continue;
                    pixels[x_point][y_point].color = placer_type;
                }
            }
            for (int ix = 0; ix < 8; ix++) {
                for (int iy = 4; iy < 8; iy++) {
                    int x_point = placer_x + ix;
                    int y_point = placer_y + iy;
                    if (x_point >= GAME_WIDTH || x_point < 0)
                        continue;
                    pixels[x_point][y_point].color = placer_type;
                }
            }
        }
    }

    static bool lsecond, lalpha;
    bool second = kb_Data[1] & kb_2nd;
    bool alpha = kb_Data[2] & kb_Alpha;

    if (second && !lsecond)
        placer_type += placer_type >= 4 ? 0 : 1;

    if (alpha && !lalpha)
        placer_type -= placer_type <= 1 ? 0 : 1;

    lsecond = second;
    lalpha = alpha;

    update_columns(pixels);

    return true;

}

void draw() {

    for (int x = 0; x < GAME_WIDTH; x++) { // another big ahh double for loop
        for (int y = 0; y < GAME_HEIGHT; y++) {
            pixel_t* pixel = &pixels[x][y];

            // if (pixel->was_cleared) {
            //     pixel->was_cleared = false;
            //     gfx_SetColor(5); // white
            
            // } else

            gfx_SetColor(pixel->color); // color indecies 1-4 are the pixel colors (maybe add variation later)

            // if (pixel->color != NONE) // this might actually be slower than just drawing nones as black
            gfx_Rectangle_NoClip(x * 2 + 2, GFX_LCD_HEIGHT - (y * 2 + 4/*?*/), 2, 2);
            
            
        }
    }

    gfx_SetColor(placer_type);
    gfx_Rectangle_NoClip(placer_x * 2 + 2, GFX_LCD_HEIGHT - (placer_y * 2 + 4), 2, 2);

}

void end() {

}

