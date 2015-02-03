/* Shapes.c - funnctions for individual shapes
*/
#include "block.h"
#include "board.h"
#include "shapes.h"
#include <stdlib.h>
#include <stdio.h>

int __n_shapes_create(struct __n_shapes* piece, int type) {
	piece->piece = (struct __netris_block*)malloc(sizeof(struct __netris_block)*4);
	if (piece->piece == NULL) {
		return 1;
	}
	piece->x_offset = 0;
	piece->y_offset = 0;
	piece->type = type;
	piece->color = type+1;
	piece->position = 0;
	switch (type) {
		case 0:
			__netris_create_block(piece->piece,   0, 1, type+1);
			__netris_create_block(piece->piece+1, 1, 1, type+1);
			__netris_create_block(piece->piece+2, 0, 2, type+1);
			__netris_create_block(piece->piece+3, 1, 2, type+1);
			break;
		case 1:
			__netris_create_block(piece->piece,   1, 0, type+1);
			__netris_create_block(piece->piece+1, 1, 1, type+1);
			__netris_create_block(piece->piece+2, 1, 2, type+1);
			__netris_create_block(piece->piece+3, 2, 2, type+1);
			break;
		case 2:
			__netris_create_block(piece->piece,   1, 0, type+1);
			__netris_create_block(piece->piece+1, 1, 1, type+1);
			__netris_create_block(piece->piece+2, 1, 2, type+1);
			__netris_create_block(piece->piece+3, 0, 2, type+1);
			break;
		case 3:
			__netris_create_block(piece->piece,   0, 1, type+1);
			__netris_create_block(piece->piece+1, 1, 1, type+1);
			__netris_create_block(piece->piece+2, 2, 1, type+1);
			__netris_create_block(piece->piece+3, 3, 1, type+1);
			break;
		case 4:
			__netris_create_block(piece->piece,   2, 1, type+1);
			__netris_create_block(piece->piece+1, 1, 1, type+1);
			__netris_create_block(piece->piece+2, 1, 2, type+1);
			__netris_create_block(piece->piece+3, 0, 2, type+1);
			break;
		case 5:
			__netris_create_block(piece->piece,   0, 1, type+1);
			__netris_create_block(piece->piece+1, 1, 1, type+1);
			__netris_create_block(piece->piece+2, 1, 2, type+1);
			__netris_create_block(piece->piece+3, 2, 2, type+1);
			break;
		case 6: 
			__netris_create_block(piece->piece,   0, 1, type+1);
			__netris_create_block(piece->piece+1, 1, 1, type+1);
			__netris_create_block(piece->piece+2, 1, 0, type+1);
			__netris_create_block(piece->piece+3, 1, 2, type+1);
			break;
		default :
			free(piece->piece); 
			piece->piece = NULL;
			return 1;
			break;
		}
}

/* __n_shapes_rotate()
 * Rotates a given piece on a given board.
 * Returns: 0 on success, 1 on failure
 */
int __n_shapes_rotate(struct __netris_board* board, struct __n_shapes* piece) {
	fprintf(stderr, "Results of test collide: %d\n",__n_shapes_test_collide(board, piece, piece->x_offset, piece->y_offset, piece->position+1));

	switch (piece->type) {
		case 0: // Square block
				// It's the square block, no rotating!
				break;
		case 1: // L-shaped block
			switch (piece->position) {
				case 0: /*   1      
					     *   2  to 321
					     *   34    4  
					     */
					if (__n_shapes_test_collide(board, piece, piece->x_offset, piece->y_offset, 1) == 0) {
						((piece->position))++;
						piece->piece[0].x++;
						piece->piece[0].y++;

						piece->piece[2].x--;
						piece->piece[2].y--;
						
						piece->piece[3].x-=2;
					}
					else {
						return 1;
					}
					break;
				case 1:  /*        43
					      * 321 to  2  
					      * 4       1
					      */
					if (__n_shapes_test_collide(board, piece, piece->x_offset, piece->y_offset, 2) == 0) {
						(piece->position)++;
						piece->piece[0].x--;
						piece->piece[0].y++;

						piece->piece[2].x++;
						piece->piece[2].y--;
						
						piece->piece[3].y-=2;
					}
					else {
						return 1;
					}

					break;
				case 2: /* 43       4
					     *  2  to 123
 					     *  1       
					     */
					if (__n_shapes_test_collide(board, piece, piece->x_offset, piece->y_offset, 3) == 0) {
						(piece->position)++;
						piece->piece[0].x--;
						piece->piece[0].y--;

						piece->piece[2].x++;
						piece->piece[2].y++;
						
						piece->piece[3].x+=2;
					}
					else {
						return 1;
					}
					break;
				case 3: /*   4    1 
					     * 123 to 2 
					     *        34
					     */
					if (__n_shapes_test_collide(board, piece, piece->x_offset, piece->y_offset, 0) == 0) {
						(piece->position) = 0;
						piece->piece[0].x++;
						piece->piece[0].y--;

						piece->piece[2].x--;
						piece->piece[2].y++;
						
						piece->piece[3].y+=2;
					}
					else {
						return 1;
					}

					break;
				default :
					break;
			}
			break;

		case 2: // Reverse-L block
			switch (piece->position) {
				case 0:
					if (__n_shapes_test_collide(board, piece, piece->x_offset, piece->y_offset, 1) == 0) {
						piece->position = 1;
						piece->piece[0].x++;
						piece->piece[0].y++;

						piece->piece[2].x--;
						piece->piece[2].y--;
						
						piece->piece[3].y-=2;
					}
					else {
						return 1;
					}
					break;
				case 1:
					if (__n_shapes_test_collide(board, piece, piece->x_offset, piece->y_offset, 2) == 0) {
						piece->position = 2;
						piece->piece[0].x--;
						piece->piece[0].y++;

						piece->piece[2].x++;
						piece->piece[2].y--;
						
						piece->piece[3].x+=2;
					}
					else {
						return 1;
					}

					break;
				case 2:
					if (__n_shapes_test_collide(board, piece, piece->x_offset, piece->y_offset, 3) == 0) {
						piece->position = 3;
						piece->piece[0].x--;
						piece->piece[0].y--;

						piece->piece[2].x++;
						piece->piece[2].y++;
						
						piece->piece[3].y+=2;
					}
					else {
						return 1;
					}
					break;
				case 3:
					if (__n_shapes_test_collide(board, piece, piece->x_offset, piece->y_offset, 0) == 0) {
						piece->position = 0;
						piece->piece[0].x++;
						piece->piece[0].y--;

						piece->piece[2].x--;
						piece->piece[2].y++;
						
						piece->piece[3].x-=2;
					}
					else {
						return 1;
					} 
					break;
				default :
					break;
			}
			break;
		case 3: // | block
			switch (piece->position) {
				case 0:
					if (__n_shapes_test_collide(board, piece, piece->x_offset, piece->y_offset, 1) == 0) {
						piece->position = 1;
						piece->piece[0].x++;
						piece->piece[0].y--;

						piece->piece[2].x--;
						piece->piece[2].y++;
						
						piece->piece[3].x-=2;
						piece->piece[3].y+=2;
					}
					else {
						return 1;
					} 
	
					break;
				case 1:
					if (__n_shapes_test_collide(board, piece, piece->x_offset, piece->y_offset, 0) == 0) {
						piece->position = 0;
						piece->piece[0].x--;
						piece->piece[0].y++;

						piece->piece[2].x++;
						piece->piece[2].y--;
						
						piece->piece[3].x+=2;
						piece->piece[3].y-=2;
					}
					else {
						return 1;
					} 
					break;
				default :
					break;
			}
			break;
		case 4: // Zig-block (pointing right)
			switch (piece->position) {
				case 0:
		if (__n_shapes_test_collide(board, piece, piece->x_offset, piece->y_offset, 1) == 0) {
						piece->position = 1;
						piece->piece[0].x--;
						piece->piece[0].y--;

						piece->piece[2].x++;
						piece->piece[2].y--;
						
						piece->piece[3].x+=2;
					}
					else {
						return 1;
					} 	
					break;
				case 1:
					if (__n_shapes_test_collide(board, piece, piece->x_offset, piece->y_offset, 0) == 0) {
						piece->position = 0;
						piece->piece[0].x++;
						piece->piece[0].y++;

						piece->piece[2].x--;
						piece->piece[2].y++;
						
						piece->piece[3].x-=2;
					}
					else {
						return 1;
					}
					break;
				default :
					break;
			}
			break;
		case 5: // Zag block (pointing left)
			switch (piece->position) {
				case 0:
					if (__n_shapes_test_collide(board, piece, piece->x_offset, piece->y_offset, 1) == 0) {
						piece->position = 1;
						piece->piece[0].x++;
						piece->piece[0].y--;

						piece->piece[2].x--;
						piece->piece[2].y--;
						
						piece->piece[3].x-=2;
					}
					else {
						return 1;
					} 	

					break;
				case 1:
					if (__n_shapes_test_collide(board, piece, piece->x_offset, piece->y_offset, 0) == 0) {
						piece->position = 0;
						piece->piece[0].x--;
						piece->piece[0].y++;

						piece->piece[2].x++;
						piece->piece[2].y++;
						
						piece->piece[3].x+=2;
					}
					else {
						return 1;
					} 	

					break;
				default :
					break;
			}
			break;
		case 6: // T block
			switch (piece->position) {
				case 0:
					if (__n_shapes_test_collide(board, piece, piece->x_offset, piece->y_offset, 1) == 0) {
						piece->position = 1;
						piece->piece[0].x++;
						piece->piece[0].y--;

						piece->piece[2].x++;
						piece->piece[2].y++;
						
						piece->piece[3].x--;
						piece->piece[3].y--;
					}
					else {
						return 1;
					} 	

					break;
				case 1:
					if (__n_shapes_test_collide(board, piece, piece->x_offset, piece->y_offset, 2) == 0) {
						piece->position = 2;
						piece->piece[0].x++;
						piece->piece[0].y++;

						piece->piece[2].x--;
						piece->piece[2].y++;
						
						piece->piece[3].x++;
						piece->piece[3].y--;
					}
					else {
						return 1;
					} 	

					break;
				case 2:
					if (__n_shapes_test_collide(board, piece, piece->x_offset, piece->y_offset, 3) == 0) {
						piece->position = 3;
						piece->piece[0].x--;
						piece->piece[0].y++;

						piece->piece[2].x--;
						piece->piece[2].y--;

						piece->piece[3].x++;
						piece->piece[3].y++;
					}
					else {
						return 1;
					} 	

					break;
				case 3:
					if (__n_shapes_test_collide(board, piece, piece->x_offset, piece->y_offset, 0) == 0) {
						piece->position = 0;
						piece->piece[0].x--;
						piece->piece[0].y--;

						piece->piece[2].x++;
						piece->piece[2].y--;
						
						piece->piece[3].x--;
						piece->piece[3].y++;
					}
					else {
						return 1;
					} 	

					break;
				default :
					break;
			}
			break;
		default:
			break;
	}
	return 0;
}

int __n_shapes_move (struct __netris_board* board, struct __n_shapes* piece, int x, int y) {
	int i;
/*	for (i = 0; i < 4; i++) {
		fprintf(stderr, "Block %d: %d, %d\n", i, piece->piece[i].x, piece->piece[i].y);
	}*/
	if (__n_shapes_test_collide(board, piece, x, y, piece->position) == 1) {
//		fprintf(stderr, "Collision! at %d, %d\n", x, y);
		return 1;
	}
//	fprintf(stderr, "No Collision\n");
	piece->x_offset = x;
	piece->y_offset = y;
	return 0;
}

int __n_shapes_test_collide(struct __netris_board* board, struct __n_shapes* piece, int x, int y, int pos) {
	switch (piece->type) {
		case 0: // Square block
			if (__netris_test_collide(board, x, y+1) != 0 ||
			    __netris_test_collide(board, x, y+2) != 0 ||
			    __netris_test_collide(board, x+1, y+1) != 0 ||
			    __netris_test_collide(board, x+1, y+2) != 0) {
				return 1;
			}
			break;
		case 1: // L-shaped block
			switch (pos) {
				case 0:
					if (__netris_test_collide(board, x+1, y) != 0 ||
					    __netris_test_collide(board, x+1, y+1) != 0 ||
					    __netris_test_collide(board, x+1, y+2) != 0 ||
					    __netris_test_collide(board, x+2, y+2) != 0) {
						return 1;
					}
					break;
				case 1:
					if (__netris_test_collide(board, x+2, y+1) != 0 ||
					    __netris_test_collide(board, x+1, y+1) != 0 ||
					    __netris_test_collide(board, x, y+1) != 0 ||
					    __netris_test_collide(board, x, y+2) != 0) {
						return 1;
					}
					break;
				case 2:
					if (__netris_test_collide(board, x+1, y+2) != 0 ||
					    __netris_test_collide(board, x+1, y+1) != 0 ||
					    __netris_test_collide(board, x+1, y) != 0 ||
					    __netris_test_collide(board, x, y) != 0) {
						return 1;
					}
					break;
				case 3:
					if (__netris_test_collide(board, x, y+1) != 0 ||
					    __netris_test_collide(board, x+1, y+1) != 0 ||
					    __netris_test_collide(board, x+2, y+1) != 0 ||
					    __netris_test_collide(board, x+2, y) != 0) {
						return 1;
					}
					break;
				default: 
					break;
			}
			break;
		case 2: // Reverse L
			switch (pos) {
				case 0:
					if (__netris_test_collide(board, x+1, y) != 0 ||
					    __netris_test_collide(board, x+1, y+1) != 0 ||
					    __netris_test_collide(board, x+1, y+2) != 0 ||
					    __netris_test_collide(board, x, y+2) != 0) {
						return 1;
					}
					break;
				case 1:
					if (__netris_test_collide(board, x+2, y+1) != 0 ||
					    __netris_test_collide(board, x+1, y+1) != 0 ||
					    __netris_test_collide(board, x, y+1) != 0 ||
					    __netris_test_collide(board, x, y) != 0) {
						return 1;
					}
					break;
				case 2:
					if (__netris_test_collide(board, x+1, y+2) != 0 ||
					    __netris_test_collide(board, x+1, y+1) != 0 ||
					    __netris_test_collide(board, x+1, y) != 0 ||
					    __netris_test_collide(board, x+2, y) != 0) {
						return 1;
					}
					break;
				case 3:
					if (__netris_test_collide(board, x, y+1) != 0 ||
					    __netris_test_collide(board, x+1, y+1) != 0 ||
					    __netris_test_collide(board, x+2, y+1) != 0 ||
					    __netris_test_collide(board, x+2, y+2) != 0) {
						return 1;
					}
					break;
				default :
					return -1;
			}
			break;
		case 3:
			switch (pos) {
				case 0:
					if (__netris_test_collide(board, x, y+1) != 0 ||
					    __netris_test_collide(board, x+1, y+1) != 0 ||
					    __netris_test_collide(board, x+2, y+1) != 0 ||
					    __netris_test_collide(board, x+3, y+1) != 0) {
						return 1;
					}
					break;
				case 1:
					if (__netris_test_collide(board, x+1, y) != 0 ||
					    __netris_test_collide(board, x+1, y+1) != 0 ||
					    __netris_test_collide(board, x+1, y+2) != 0 ||
					    __netris_test_collide(board, x+1, y+3) != 0) {
						return 1;
					}
					break;
				default: 
					return -1;
			}				
			break;
		case 4:
			switch (pos) {
				case 0:
					if (__netris_test_collide(board, x+2, y+1) != 0 ||
					    __netris_test_collide(board, x+1, y+1) != 0 ||
					    __netris_test_collide(board, x+1, y+2) != 0 ||
					    __netris_test_collide(board, x, y+2) != 0) {
						return 1;
					}
					break;
				case 1:
					if (__netris_test_collide(board, x+1, y) != 0 ||
					    __netris_test_collide(board, x+1, y+1) != 0 ||
					    __netris_test_collide(board, x+2, y+1) != 0 ||
					    __netris_test_collide(board, x+2, y+2) != 0) {
						return 1;
					}
					break;
				default: 
					return -1;
			}
			break;
		case 5:
			switch (pos) {
				case 0:
					if (__netris_test_collide(board, x, y+1) != 0 ||
					    __netris_test_collide(board, x+1, y+1) != 0 ||
					    __netris_test_collide(board, x+1, y+2) != 0 ||
					    __netris_test_collide(board, x+2, y+2) != 0) {
						return 1;
					}
					break;
				case 1:
					if (__netris_test_collide(board, x+1, y) != 0 ||
					    __netris_test_collide(board, x+1, y+1) != 0 ||
					    __netris_test_collide(board, x, y+1) != 0 ||
					    __netris_test_collide(board, x, y+2) != 0) {
						return 1;
					}
					break;
				default :
					return -1;
			}
			break;
		case 6: // T block
			switch (pos) {
				case 0:
					if (__netris_test_collide(board, x, y+1) != 0 ||
					    __netris_test_collide(board, x+1, y+1) != 0 ||
					    __netris_test_collide(board, x+1, y) != 0 ||
					    __netris_test_collide(board, x+1, y+2) != 0) {
						return 1;
					}
					break;
				case 1:
					if (__netris_test_collide(board, x+1, y) != 0 ||
					    __netris_test_collide(board, x+1, y+1) != 0 ||
					    __netris_test_collide(board, x+2, y+1) != 0 ||
					    __netris_test_collide(board, x, y+1) != 0) {
						return 1;
					}
					break;
				case 2:
					if (__netris_test_collide(board, x+2, y+1) != 0 ||
					    __netris_test_collide(board, x+1, y+1) != 0 ||
					    __netris_test_collide(board, x+1, y+2) != 0 ||
					    __netris_test_collide(board, x+1, y) != 0) {
						return 1;
					}
					break;
				case 3:
					if (__netris_test_collide(board, x+1, y+2) != 0 ||
					    __netris_test_collide(board, x+1, y+1) != 0 ||
					    __netris_test_collide(board, x, y+1) != 0 ||
					    __netris_test_collide(board, x+2, y+1) != 0) {
						return 1;
					}
					break;
				default: return -1;
			} 
			break;
			default: 
				return -1;
				break;
	}
	return 0;
}

int __n_shapes_destroy (struct __n_shapes* piece) {
	free(piece->piece);
	piece->x_offset = 0;
	piece->y_offset = 0;
	piece->type = -1;
	piece->color = -1;
	piece->position = 0;
	return 0;
}

int __n_shapes_set(struct __netris_board* board, struct __n_shapes* piece) {
	int i;
	fprintf(stderr, "Set at %d, %d, color %d\n", piece->piece[1].x+piece->x_offset, piece->piece[1].y+piece->y_offset, piece->piece[1].color);
	for (i = 0; i < 4; i++) {
		__n_board_set_block(board, piece->piece+i, piece->piece[i].x+piece->x_offset, piece->piece[i].y+piece->y_offset);
	}
	return 0;
}
