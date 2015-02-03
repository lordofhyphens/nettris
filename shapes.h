/* shapes.h - Shape definitons for the board pieces  */

#ifndef __N_SHAPES_H
#define __N_SHAPES_H

struct __netris_board;
struct __netris_block;

struct __n_shapes {
	struct __netris_block* piece; // array of 4 blocks 
	int x_offset;  // upper left corner x coord of piece
	int y_offset;  // upper left corner y ccood of piece
	int type;      // what kind of piece it is
	int color;     // color of piece, should be type +1
	int position;  // Current position (rotate) of piece

};

int __n_shapes_create (struct __n_shapes* piece, int type);
int __n_shapes_rotate (struct __netris_board* board, struct __n_shapes* piece);
int __n_shapes_move (struct __netris_board* board, struct __n_shapes* piece, int x, int y);
int __n_shapes_will_collide(struct __netris_board* board, struct __n_shapes* piece, int x, int y);
int __n_shapes_test_collide(struct __netris_board* board, struct __n_shapes* piece, int x, int y, int pos);
int __n_shapes_destroy (struct __n_shapes* piece);

int __n_shapes_set(struct __netris_board* board, struct __n_shapes* piece);
#endif
