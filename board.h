/* board.h - data structures and function headers pertaining
 * to the state of the game board.  */

/* Set the board height snd width */
#ifndef __N_BOARD_H
#define __N_BOARD_H


struct __netris_block;

struct __netris_board {
	int width; // in blocks
	int height; // in blocks
	int x; // in pixels
	int y; // in pixels
	int** board; // Array
};

int __n_board_create (struct __netris_board* board, int width, int height, int x, int y);

int __n_board_copy (struct __netris_board* source, struct __netris_board* dest);

int __n_board_destroy (struct __netris_board* board);

/* __n_board_set_block()
 * Takes: A board address, a block address, and the x and y coordinates of the block
 * being placed.
 * Returns: 0 if the block is placed successfully, 1 if there is already a block in
 * that position (not 0).
 */
int __n_board_set_block(struct __netris_board* board, struct __netris_block* block, int x, int y);

int __n_board_unset_block(struct __netris_board* board, int x, int y);
/* __n_board_shift_row - Shift a row of blocks down exactly one. This 
 * would be a destructive removal; overwriting existing data.
 */
int __n_board_shift_row(struct __netris_board* board, int row);

int __n_board_remove_row(struct __netris_board* board, int row);
/* __n_board_check_row()
 * Checks to see if a row is complete. 
 * Returns -1 if no rows complete, or the first complete row it runs into.
 */
int __n_board_check_row(struct __netris_board* board);

int __netris_board_grid_print(struct __netris_board* board);

#endif

