/* video.h - defines shape structures
 *
 * Team Members:
 * Chris Nelson
 * Joe Lenox
 * John VanVoltenburg
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "shapes.h"
#include "board.h"
#include "block.h"

#ifndef VIDEO_H
#define VIDEO_H

// apply_image - rudimentary test for blitting surfaces.
int apply_image(SDL_Surface *src, SDL_Surface *dest, int x, int y, int h, int w);

// SDL image loader. Converts picture files to SDL_Surfaces
SDL_Surface *load_image(char *filename);

// Draws a single block on the screen
int drawblock(struct __netris_block* block, struct __netris_board* board, int x, int y);

int draw_behind_block(int, int, int, int);
int draw_fore_block(struct __netris_block*, struct __netris_board*, int, int);

// Draws a shape on the screen
int drawshape(struct __n_shapes* shape, struct __netris_board* board);

// Redraws the entire playing field of a player
int drawbin(struct __netris_board* board);

// Draws the next shape in the space above the player's bin
int drawnext(struct __n_shapes* shape, struct __netris_board* board);

// Fills the winners bin up with gray blocks and displays a message declaring them the // winner
int winner(struct __netris_board* board);

// Draws the background on the screen.
int drawbg();

// Creates the primary SDL window, and initializes all png models created as SDL_Surfaces
int loadsurfaces();

// applies all draw commands to the screen using SDL_Flip();
int flipscreen();

// Frees all surfaces declared to prevent memory leaks. Used at program termination
int freesurfaces();

#endif
