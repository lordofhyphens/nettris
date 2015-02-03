/* Board.c - Functions for board.h
 */
#include "board.h"
#include "block.h"
#include "shapes.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int __n_board_create (struct __netris_board* board, int width, int height, int x, int y) {

	int temp;

	board->width = width;
	board->height = height;
	board->x = x;
	board->y = y;
	// Assign memory for the array of pointes.
	board->board = (int**)malloc(sizeof(int*)*height);
	if (board->board == NULL) {
		return 1;
	}
	for (temp = 0; temp < height; temp++) {
		// Assign memory for a given row.
		board->board[temp] = (int*)calloc(width, sizeof(int));

		// If the memory alloc fails, baoktrack and free and then return error (-1).
		if (board->board[temp] == NULL) {
			temp--;
			while (temp > -1) {
				free(board->board[temp]);
				temp--;
			}
			free (board->board);
			return 1;
		}
	}
	return 0;
}
/* __n_board_copy () 
 * Copies, member for member, the contents of one board to another.
 * The contents of dest are destroyed (if they exist).
 * If the board creation fails,  
 */ 

int __n_board_copy (struct __netris_board* source, struct __netris_board* dest) {
	int temp; 

	if (dest->board != NULL) {
		// Oh great, there's something here to take care of.
		__n_board_destroy(dest);
	}
	if (__n_board_create(dest, source->width, source->height, source->x, source->y)) {
		return 1;
	}
	for (temp = 0; temp < source->height; temp++) {
		memcpy(dest->board[temp], source->board[temp], sizeof(int)*source->width);
	}

	return 0;
}
/* __n_board_destroy()
 * Takes - Address of an board.
 * Returns: 0 if board destroys properly.
 * Completely destroys the passed __netris_board
 */
int __n_board_destroy (struct __netris_board* board) {
	int temp;
	for (temp = 0; temp < board->height; temp++) {
		free(board->board[temp]);
		board->board[temp] = NULL;
	}
	free(board->board);
	board->width = 0;
	board->height = 0;
		
	return 0;
}

/* __n_board_set_block()
 * Takes: A board address, a block address, and the x and y coordinates of the block
 * being placed.
 * Returns: 0 if the block is placed successfully, 1 if there is already a block in
 * that position (not 0).
 */
int __n_board_set_block(struct __netris_board* board, struct __netris_block* block, int x, int y) {
//	fprintf(stderr, "Block Color: %d\n", block->color);
	if (board->board[y][x] == 0) {
		board->board[y][x] = block->color;
	} else {
		return 1;
	}
	return 0;
}

/* __n_board_unset_block()
 * Takes: the address of a board, x and y coordinates of the block we want to unset.
 * Returns: 0 if the block was successfully unset, 1 otherwise.
 */
int __n_board_unset_block(struct __netris_board* board, int x, int y) {
	if (x >= board->width || y >= board->height) {
		return 1;
	} 
	if (board->board[y][x] != 0) {
		board->board[y][x] = 0;
	} else {
		return 1;
	} 
	return 0;
}
/* __n_board_shift_row - Shift a row of blocks down exactly one. This
 * would be a destructive removal; overwriting existing data.
 * If the row is the top row, it is just deleted.
 * Returns: 1 if the row was shifted successfully, 0 otherwise.
 */
int __n_board_shift_row(struct __netris_board* board, int row) {
	if (row >= board->height) {
		return 1;
	}
	if (row == 0) {
		__n_board_remove_row(board, 0);
	}
	memcpy(board->board[row], board->board[row-1], sizeof(int)*board->width);
	return 0;
}

/* __n_board_remove_row()
 * Removes a row 
 */
int __n_board_remove_row(struct __netris_board* board, int row) {
	free(board->board[row]);
	board->board[row] = (int*)calloc(board->width, sizeof(int));
	return 0;
}

int __n_board_check_row(struct __netris_board* board) {
	int i, j;
	int is_complete = 1;
	for (i = board->height-1; i > 0; i--) {
		for (j = 0; j < board->width; j++) {
			if (board->board[i][j] == 0) {
				is_complete = 0;
			}
			fprintf(stderr,"i: %d; j: %d; is_complete: %d\n", i, j, is_complete);
		}
		if (is_complete == 1) {
			return i;
		}
		is_complete = 1;
	}
	return -1;
}

int __netris_board_grid_print(struct __netris_board* board) {
	int i, j;
	for (i = 0; i < board->height; i++) {
		for (j = 0; j <  board->width; j++) {
			fprintf(stderr, "%d", board->board[i][j]);
		}
		fprintf(stderr,"\n");
	}
	return 0;
}
