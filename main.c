/* main.c - contains main loop for net
 */
#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include "board.h"
#include "shapes.h"
#include "block.h"
#include "video.h"

int init();
int send_piece(struct __n_shapes* piece, int purpose, int socket);
int clear_lines(struct __netris_board* board);
int cycle_piece(struct __n_shapes* current, struct __n_shapes* next);

SDL_Surface* screen;
SDL_Event event;
int quit = 0;
	int* temp;

int main(int argc, char* argv[]) {
	srand(time(NULL));
	struct __netris_board next_window[2];
	struct __netris_board play_board[2];
	struct __n_shapes current[2];
	struct __n_shapes next[2];
	int width, height;
	int t;
	long ticker;
	int threshold = 1000; /* time in between ticks */
	SDL_Event timer;
	SDL_Event net_recieve;

	struct sockaddr_in other_addr;
	int other_addr_len = sizeof(struct sockaddr_in);
	int from_server[5];
	int net_socket,client;
	int isServer;

/* initialize the network socket */
	if ( (net_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Socket");
		exit(errno);
	}

/* Set up the socket address */
	bzero(&other_addr, sizeof(other_addr));
	other_addr.sin_family = AF_INET;
	if (inet_aton("127.0.0.1", &other_addr.sin_addr.s_addr) == 0) {
		perror("Socket");
		exit(errno);
	}
	other_addr.sin_port = htons(1337);

/* Bind the socket to a specific socket address */
	bind(net_socket, (struct sockaddr*)&other_addr, sizeof(struct sockaddr_in));

/* initialize the playing fields */
	if (strcmp(argv[2], "p1")  == 0) {
		__n_board_create(play_board,   14, 20,  50, 100);
		__n_board_create(play_board+1, 14, 20, 380, 100);
		__n_board_create(next_window,   4, 4,  180,  20);
		__n_board_create(next_window+1, 4, 4,  510,  10);

/* Set up a connection queue */	
		isServer = 1;
		other_addr.sin_addr.s_addr = INADDR_ANY;
		listen(net_socket,2);
		fprintf(stderr, "Setting up connection queue.\n");
		if ( (client = accept(net_socket,(struct sockaddr*)&other_addr , &other_addr_len)) == -1) {
				perror("accept");
				exit(errno);
		}

	} else {
		isServer = 0;
		__n_board_create(play_board,   14, 20, 380, 100);
		__n_board_create(play_board+1, 14, 20,  50, 100);
		__n_board_create(next_window,   4,  4, 510,  20);
		__n_board_create(next_window+1, 4,  4, 180,  10);
		fprintf(stderr, "Attempting connection to server.\n");
		if (connect(net_socket,(struct sockaddr*)&other_addr, sizeof(struct sockaddr)) == -1) {
			fprintf(stderr, "Connection to server failed\n");
//			perror("127.0.0.1");
			exit(errno);
		} else { fprintf(stderr,"Connection success."); }
	}

/* Initialize the "Next Piece" windows */
	__n_shapes_create(current, rand()%7);
	__n_shapes_create(next,  rand()%7);

	__n_shapes_move(play_board,current, (play_board->width / 2) -2, 0);
	__n_shapes_move(next_window, next, 0,0);

	if (init() == 1 || loadsurfaces() == 1 ) {
		fprintf(stderr, "Failure on init.\n");
		return 1;
	}

	ticker = SDL_GetTicks();
	while (quit == 0) {
		if (ticker+threshold < SDL_GetTicks()) {
			ticker = SDL_GetTicks();
			timer.type=SDL_USEREVENT;
			timer.user.code=1;
			SDL_PushEvent(&timer);
		}
		fprintf(stderr, "checking for data from other player.\n");
			net_recieve.type=SDL_USEREVENT;
			net_recieve.user.code=5;
			net_recieve.user.data1 = from_server;
		if (isServer) {
			sleep(1);
			if (recv(client, net_recieve.user.data1, sizeof(int)*5,MSG_DONTWAIT) > 0) {
				fprintf(stderr,"Network: Data read, raising event.\n");
				fprintf(stderr,"Network: Recieved %d, %d, %d, %d\n", from_server[0], from_server[1], from_server[2],from_server[3]);
				SDL_PushEvent(&net_recieve);
			} else { 
				fprintf(stderr, "No data found. Moving on.\n");
			}
		} else {
			if (recv(net_socket, net_recieve.user.data1, sizeof(int)*5,MSG_DONTWAIT) > 0) {
				fprintf(stderr,"Network: Data read, raising event.\n");
				fprintf(stderr,"Network: Recieved %d, %d, %d, %d\n", from_server[0], from_server[1], from_server[2],from_server[3]);
				SDL_PushEvent(&net_recieve);
			} else { 
				fprintf(stderr, "No data found. Moving on.\n");
			}
		}
		// event poll
		while(SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					quit = 1;
					break;
				case SDL_KEYDOWN:
						switch (event.key.keysym.sym) {
							case SDLK_LEFT:
								__n_shapes_move(play_board, current, current->x_offset-1, current->y_offset);
								if (isServer) {
									send_piece(current, 0, client);
								} else {
									send_piece(current, 0, net_socket);
								} 
								break;
							case SDLK_RIGHT:
								__n_shapes_move(play_board, current, current->x_offset+1, current->y_offset);
								if (isServer) {
									send_piece(current, 0, client);
								} else {
									send_piece(current, 0, net_socket);
								} 
								break;
							case SDLK_DOWN:
									threshold = 0; /* turbospeed!*/
									break;
							case SDLK_SPACE:
								__n_shapes_rotate(play_board, current);
								fprintf(stderr, "Piece rotated to %d\n", current->position);
								break;
							default:
								break;
						}
					break;
				case SDL_USEREVENT:
						switch(event.user.code) {
							case 5:
								temp = (int*)event.user.data1;
								fprintf(stderr,"Network: Event Recieved %d, %d, %d, %d\n", temp[0], temp[1], temp[2],temp[3]);
								switch(temp[3]) {
										case 0:
											fprintf(stderr,"Network Action: Normal Piece\n");
											if ((current+1)->piece != NULL) { __n_shapes_destroy(current+1); }
											__n_shapes_create(current+1,temp[0]);
											for (t = 0; t < temp[4]; t++) {
												__n_shapes_rotate(play_board+1, current+1);
											}
											__n_shapes_move(play_board+1,current+1, temp[1],temp[2]);
											break;
										case 1:
											fprintf(stderr,"Network Action: Set Piece\n");
											if ((current+1)->piece != NULL) { __n_shapes_destroy(current+1); }
											__n_shapes_create(current+1,temp[0]);

											for (t = 0; t < temp[4]; t++) {
												__n_shapes_rotate(play_board+1, current+1);
											}
											__n_shapes_move(play_board+1,current+1, temp[1],temp[2]);
											__n_shapes_set(play_board+1,current+1);

											clear_lines(play_board+1);
										break;
									case 2:
										fprintf(stderr,"Network Action: New Next Piece\n");
										if ((next+1)->piece != NULL) { __n_shapes_destroy(next+1); }
										__n_shapes_create(next+1,temp[0]);
										__n_shapes_move(next_window+1,next+1, 0,0);
										__n_shapes_set(next_window+1,next+1);
										break;
									}
								break;
							 case 1:
							 	fprintf(stderr,"Timer Action: Collsion Check");
								if (__n_shapes_move(play_board, current, current->x_offset, current->y_offset+1) == 1) {
									// Stop timer here
									threshold = 1000;
									__n_shapes_set(play_board, current);
								if (isServer) {
									send_piece(current, 1, client);
								} else {
									send_piece(current, 1, net_socket);
								} 

									clear_lines(play_board);
									cycle_piece(current, next);

									__n_shapes_move(next_window, next,0,0);
									fprintf(stderr,"Home Board\n");
									__netris_board_grid_print(play_board); /* Debug code. */

								if (isServer) {
									send_piece(current, 0, client);
								} else {
									send_piece(current, 0, net_socket);
								} 

									if (__n_shapes_move(play_board, current, (play_board->width / 2) - 2, 0) != 0) {
										quit = 1;
									}
								}
								else {	
									if (isServer) {
										send_piece(current, 0, client);
									} else {
										send_piece(current, 0, net_socket);
									} 
								}
								break;
							default:
								break;
						}
					break;
				default:
					break;
			}
		}
	fprintf(stderr,"Drawing background.\n");
/* The Great Drawing */
		drawbg();
		drawbin(play_board);
		drawshape(current, play_board);
		drawshape(next, next_window);
	fprintf(stderr,"Drawing (other board).\n");

		if ((current+1)->piece == NULL) {
			fprintf(stderr,"Block not created. Making temp.\n");
			__n_shapes_create(current+1, 0);
			__n_shapes_move(play_board+1,current+1,0,0);
		}
//	fprintf(stderr,"We get drawing(other board).\n");

		if ((next+1)->piece == NULL) {
			fprintf(stderr,"Block not created. Making temp.\n");
			__n_shapes_create(next+1, 0);
			__n_shapes_move(next_window+1,next+1,0,0);
		}

	drawbin(play_board+1);
	if ((current+1)->piece == NULL) { drawshape(current+1, play_board+1); }
	if ((next+1)->piece == NULL) { 
			__n_shapes_move(next_window+1,next+1,0,0);
			drawshape(next+1,next_window+1); 
	}
		flipscreen();
		fprintf(stderr,"Away Board\n");
		__netris_board_grid_print(play_board+1); /* Debug code. */
		
	}

/* Clean up structures */
	__n_board_destroy(play_board);
	__n_board_destroy(play_board+1);
	__n_shapes_destroy(current);
	close(net_socket);
	if (isServer) {
		close(client);
	}
	__n_shapes_destroy(next);
	SDL_Quit();
	return 0;
}

int init() {
/*Initialize all SDL subsystems */
		if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_EVENTTHREAD | SDL_INIT_VIDEO) == -1) {
		return 1;
	}
	return 0;
}

/* purpose -- what to do with the data on the recieving end.
 * 0 - just display as a falling piece.
 * 1 - set the piece in the other bin.
 * 2 - set as the 'next piece' for the other bin. */
int send_piece(struct __n_shapes* piece, int purpose, int socket) {
	int* temp = (int*)malloc(sizeof(int)*4);
	if (temp == NULL) { return 1; /* malloc failed. */ }
	temp[0] = piece->type;
	temp[1] = piece->x_offset;
	temp[2] = piece->y_offset;
	temp[3] = purpose;
	fprintf(stderr, "Network: Writing Data to Socket\n");
	write(socket, temp, sizeof(int)*4);
	free(temp);

	return 0;
}

int clear_lines(struct __netris_board* board) {
	int row;
	while (__n_board_check_row(board) != -1) {
		fprintf(stderr,"Checking rows...\n");
		row = __n_board_check_row(board);
		if (row != -1) {
			fprintf(stderr,"%d row clear.\n",row);
			while (row > 0) {
				__n_board_shift_row(board, row);
				row--;
			}
		}
	}
	return 0;
}

int cycle_piece(struct __n_shapes* current, struct __n_shapes* next) {
	__n_shapes_destroy(current);
	__n_shapes_create(current, next->type);
	__n_shapes_destroy(next);
	__n_shapes_create(next,rand()%7);

	return 0;
}
