/* video.c - draws surfaces on an sdl window
 *
 * Team Members:
 * Chris Nelson
 * Joe Lenox
 * John VanVoltenburg 
 *
 */

#include "video.h"

static SDL_Surface *surfaces[11];
SDL_Surface *screen;

int loadsurfaces()
{
	screen = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);

	if(screen == NULL)
	{
		return 1;
	}

	SDL_WM_SetCaption("Netris", NULL);

	//surfaces[0] = load_image("img/block_blank.png");
	surfaces[1] = load_image("img/block_pink.png");
	surfaces[2] = load_image("img/block_blue.png");
	surfaces[3] = load_image("img/block_red.png");
	surfaces[4] = load_image("img/block_teal.png");
	surfaces[5] = load_image("img/block_purple.png");
	surfaces[6] = load_image("img/block_yellow.png");
	surfaces[7] = load_image("img/block_green.png");
	surfaces[8] = load_image("img/block_gray.png");
	surfaces[9] = load_image("img/winner.png");
	surfaces[10] = load_image("img/netris_bg.png");
	
	return 0;
}

SDL_Surface *load_image(char *filepath)
{
	SDL_Surface *Loaded_image = NULL;
	SDL_Surface *Converted_image = NULL;

	Loaded_image = IMG_Load(filepath);

	if(Loaded_image != NULL)
	{
		Converted_image = SDL_DisplayFormat(Loaded_image);
		SDL_FreeSurface(Loaded_image);
	}

	return Converted_image;
}

int apply_image(SDL_Surface *src, SDL_Surface *dest, int x, int y, int w, int h)
{
	SDL_Rect area;

	area.x = x;
	area.y = y;
//	area.w = w;
//	area.h = h; 

	SDL_BlitSurface(src, &area, dest, &area);
	return 0;	
}

int drawbg()
{
//	apply_image(surfaces[10], screen, NULL, NULL, NULL, NULL);
	SDL_BlitSurface(surfaces[10], NULL, screen, NULL);
//	//flipscreen();
	return 0;
}

int drawblock(struct __netris_block* block, struct __netris_board* board, int x, int y)
{
	int draw_x = (block->x + x)*15 + board->x;
	int draw_y = (block->y + y)*15 + board->y;

	draw_behind_block(draw_x,draw_y, 15, 15);
	draw_fore_block(block, board, draw_x, draw_y); 	
//	apply_image(surfaces[10], screen, draw_x, draw_y, 20, 20);
//	apply_image(surfaces[block->color], screen, draw_x, draw_y, 15, 15);
	return 0;
}

int draw_behind_block(int x, int y, int w, int h) {
	SDL_Rect dest;

	dest.x = x;
	dest.y = y;

	SDL_Rect dest2;
	dest2.x = x;
	dest2.y = y;
	dest2.w = w;
	dest2.h = h;
	SDL_BlitSurface(surfaces[10], &dest2, screen, &dest);
	return 0;
}
int draw_fore_pos(int x, int y, int color) {
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	SDL_BlitSurface(surfaces[color], NULL, screen, &dest);
	return 0;
}
int draw_fore_block(struct __netris_block* block, struct __netris_board* board, int x, int y) {
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	SDL_BlitSurface(surfaces[block->color], NULL, screen, &dest);
	return 0;
}

int drawshape(struct __n_shapes* shape, struct __netris_board* board)
{
	int i;
	for(i=0; i<4; i++)
	{
		drawblock(shape->piece+i, board, shape->x_offset, shape->y_offset);
	}
//	//flipscreen();
	return 0;
}

int drawbin(struct __netris_board* board)
{
	int i,j;
	struct __netris_block temp_block;
	for(i=0; i < board->height; i++)
	{
		for(j=0; j < board->width; j++)
		{
		//	fprintf(stderr,"(%d,%d)-%d\n", i, j,board->board[i][j]);
			if (board->board[i][j] != 0) {
				draw_behind_block((j*15)+board->x,(i*15)+board->y,15,15);
				draw_fore_pos((j*15)+board->x, (i*15)+board->y, board->board[i][j]);
			}
		}
	}
//	//flipscreen();
	return 0;
}

int drawnext(struct __n_shapes* shape, struct __netris_board* board)
{
	int x_offset = 130;
	int y_offset = -70;

	board->x += x_offset;
	board->y += y_offset;
	
	drawshape(shape, board);
	return 0;
}

int winner(struct __netris_board* board)
{
	SDL_Rect offset;
	offset.x = board->x+55;
	offset.y = board->y+100;

	SDL_BlitSurface(surfaces[9], NULL, screen, &offset);
	//flipscreen();
	return 0;
}

int flipscreen()
{
	if(SDL_Flip(screen) == -1) 
	{ 
		return 1; 
	}
	return 0;
}

int freesurfaces()
{
	int i;
	for(i=1; i<=10; i++)
	{
		SDL_FreeSurface(surfaces[i]);
	}

	return 0;
}
