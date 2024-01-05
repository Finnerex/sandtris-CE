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


pixel_t pixels[56][118];


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

    static int placer_x = 0;
    static int placer_y = 0;

    static pixel_color placer_type = red;

    if (kb_Data[7] & kb_Up)
        placer_y++;
    if (kb_Data[7] & kb_Down)
        placer_y--;

    if (kb_Data[7] & kb_Right)
        placer_x++;
    if (kb_Data[7] & kb_Left)
        placer_x--;


    if (kb_Data[6] & kb_Enter)
        pixels[placer_x][placer_y].color = placer_type;

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
    for (int x = 0; x < 56; x++) { // another big ahh double for loop
        for (int y = 0; y < 118; y++) {
            pixel_t* pixel = &pixels[x][y];

            if (pixel->was_cleared) {
                pixel->was_cleared = false;
                gfx_SetColor(5); // white
            
            } else
                gfx_SetColor(pixel->color); // color indecies 1-4 are the pixel colors (maybe add variation later)

            gfx_Rectangle_NoClip(x * 2, GFX_LCD_HEIGHT - y * 2, 2, 2);
            
        }
    }
}

void end() {

}

