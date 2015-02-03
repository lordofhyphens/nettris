#include "board.h"
#include "block.h"

/* __netris_create_block(__netris_block*, int, int, int)
 * Initialize a netris_block.
 * Takes the width and height of an individual block (in pizels) and
 * then a color.
 */
int __netris_create_block(struct __netris_block* block, int x, int y, int color) {
	block->x = x;
	block->y = y;
	block->color = color;
	return 0;
}

/* __netris_block_will_collide() 
 * Checks to see if a particular block structure passed is in collision with
 * any set blocks on a given gameboard. 
 * Returns 0 if false, 1 if true, and -1 on error.
 */
int __netris_block_will_collide(struct __netris_board* board, struct __netris_block* block) {
	if (block->x >= board->width || block->y >= board->height) {
		// Attempting to check a piece outside of the board's dimensions 
		return -1;
	}
	if (board->board[block->y][block->x] > 0) {
		return 1;
	}
	return 0;
}

/* __netris_destroy_block(__netris_block*) {
 * Destroys a netris_block, effectively setting everything related it to 0.
 * returns 1 on error, 0 on success.
 */
int __netris_destroy_block(struct __netris_block* block) {
	block->color = 0;
	block->x = 0;
	block->y = 0;
	return 0;
}

/* __netris_test_collide()
 * Arbitarily check to see if a particular blockspace is set.
 * Returns 0 on false (no set block), 1 on true (set block), and -1 
 * on error (usually out of bounds of the board).
 */
int __netris_test_collide(struct __netris_board* board, int x, int y) {
    if (x >= board->width || y >= board->height) {
        // Attempting to check a piece outside of the board's dimensions
        return -1;
    }
	if (board->board[y][x] > 0) {
        return 1;
    }
    return 0;
}
