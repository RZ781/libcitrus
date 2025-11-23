/* Copyright (C) 2025 RZ781
 *
 * This file is part of libcitrus.
 *
 * libcitrus is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libcitrus is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program.  If not, see
 * <https://www.gnu.org/licenses/>.
 */

#ifndef CITRUS_H
#define CITRUS_H

#include <stdbool.h>
#include <stdint.h>

#define CITRUS_PARSER_BUFFER_SIZE 16

typedef enum {
	CITRUS_KEY_LEFT,
	CITRUS_KEY_RIGHT,
	CITRUS_KEY_HARD_DROP,
	CITRUS_KEY_SOFT_DROP,
	CITRUS_KEY_CLOCKWISE,
	CITRUS_KEY_ANTICLOCKWISE,
	CITRUS_KEY_180,
	CITRUS_KEY_HOLD
} CitrusKey;

typedef enum {
	CITRUS_CELL_EMPTY,
	CITRUS_CELL_SHADOW,
	CITRUS_CELL_FULL
} CitrusCellType;

typedef enum {
	CITRUS_COLOR_I,
	CITRUS_COLOR_J,
	CITRUS_COLOR_L,
	CITRUS_COLOR_O,
	CITRUS_COLOR_S,
	CITRUS_COLOR_T,
	CITRUS_COLOR_Z
} CitrusColor;

typedef struct {
	CitrusColor color;
	CitrusCellType type;
} CitrusCell;

typedef struct {
	const CitrusCell *piece_data;
	int n_rotation_states;
	int width;
	int height;
	int spawn_y;
} CitrusPiece;

typedef struct {
	int width;
	int height;
	int full_height;
	int next_piece_queue_size;
	double gravity;
	int lock_delay;
	int max_move_reset;
	const CitrusPiece *(*randomizer)(void *);
} CitrusGameConfig;

typedef struct {
	CitrusGameConfig config;
	CitrusCell *board;
	void *randomizer_data;
	const CitrusPiece *current_piece;
	const CitrusPiece *hold_piece;
	const CitrusPiece **next_piece_queue;
	bool held;
	int current_x;
	int current_y;
	double fall_amount;
	int current_rotation;
	bool alive;
	int lock_delay;
	int move_reset_count;
	int lowest_y;
	int score;
} CitrusGame;

typedef struct {
	uint8_t buffer[CITRUS_PARSER_BUFFER_SIZE];
	int write_pointer;
	int read_pointer;
} CitrusParser;

typedef struct {
	bool in_game_clients[256];
	bool connected_clients[256];
	CitrusGame games[256];
} CitrusLobby;

typedef struct {
	CitrusLobby lobby;
	CitrusParser parser;
	void (*send)(void *send_data, int n, uint8_t * data);
	void *send_data;
} CitrusClientLobby;

typedef struct {
	CitrusLobby lobby;
	CitrusParser parsers[256];
	void (*send)(void *send_data, int n, uint8_t * data, int id);
	void *send_data;
} CitrusServerLobby;

typedef struct {
	uint64_t state;
	int chosen_pieces[7];
	int count;
} CitrusBagRandomizer;

extern const CitrusPiece citrus_pieces[7];

/**
 * @brief Initializes a CitrusGameConfig struct.
 *
 * @param config Struct to be initialized
 * @param randomizer Randomizer function that accepts a void pointer passed in CitrusGame_init
 */
void CitrusGameConfig_init(CitrusGameConfig * config,
			   const CitrusPiece * (*randomizer) (void *));

/**
 * @brief Initializes a CitrusPiece struct.
 *
 * @param piece Struct to be initialized
 * @param piece_data Array of n_rotation_states*width*height cells representing the piece in each rotation state
 * @param n_rotation_states Number of rotation states the piece has
 * @param width Width of the piece
 * @param height Height of the piece
 * @param spawn_y Y coordinate relative to board height for when the piece enters
 */
void CitrusPiece_init(CitrusPiece * piece, const CitrusCell * piece_data,
		      int n_rotation_states, int width, int height,
		      int spawn_y);

/**
 * @brief Initializes a CitrusGame struct.
 *
 * @param game Struct to be initialized
 * @param board Array of config.width*config.full_height cells that will be used to store the board
 * @param config Configuration options
 * @param randomizer_data Private internal state for randomizer function passed in config.randomizer
 */
void CitrusGame_init(CitrusGame * game, CitrusCell * board,
		     const CitrusPiece ** next_piece_queue,
		     CitrusGameConfig config, void *randomizer_data);

/**
 * @brief Indicates a key has been pressed.
 * This should be called by the client whenever a key is pressed.
 *
 * @param game Game where key was pressed
 * @param key Key that has been pressed
 */
void CitrusGame_key_down(CitrusGame * game, CitrusKey key);

/**
 * @brief Indicates a tick has passed.
 * This should called by the client 60 times per second.
 *
 * @param game Game to update
 */
void CitrusGame_tick(CitrusGame * game);

/**
 * @brief Returns whether or not the player is alive.
 * Functions including CitrusGame_key_down and CitrusGame_tick will not do anything if the player is dead.
 *
 * @param game Game to check
 * @retval true The player is still alive
 * @retval false The player has died
 */
bool CitrusGame_is_alive(CitrusGame * game);

/**
 * @brief Gets the cell at a location.
 *
 * @param game Game to check
 * @param x X coordinate of the cell
 * @param y Y coordinate of the cell
 * @return Cell at the specified location
 */
CitrusCell CitrusGame_get_cell(CitrusGame * game, int x, int y);

/**
 * @brief Gets a piece in the next piece queue
 *
 * @param game Game to get piece from
 * @param i Zero-indexed position in the queue
 * @return Piece at the specified position
 */
const CitrusPiece *CitrusGame_get_next_piece(CitrusGame * game, int i);

/**
 * @brief Initializes a CitrusBagRandomizer struct
 *
 * @param bag Struct to be initialized
 * @param seedd Seed for the random number generator
 */
void CitrusBagRandomizer_init(CitrusBagRandomizer * bag, int seed);

/**
 * @brief Returns the next piece in the bag.
 * This can be passed to CitrusGameConfig_init as the randomizer callback function.
 *
 * @param data Pointer to CitrusBagRandomizer
 */
const CitrusPiece *CitrusBagRandomizer_randomizer(void *data);

void CitrusClientLobby_init(CitrusClientLobby * lobby,
			    void (*send)(void *send_data, int n,
					 uint8_t * data), void *send_data);
void CitrusClientLobby_recv(CitrusClientLobby * lobby, int n, uint8_t * data);
void CitrusServerLobby_init(CitrusServerLobby * lobby,
			    void (*send)(void *send_data, int n, uint8_t * data,
					 int id), void *send_data);
void CitrusServerLobby_client_connect(CitrusServerLobby * lobby, int id);
void CitrusServerLobby_client_disconnect(CitrusServerLobby * lobby, int id);
void CitrusServerLobby_recv(CitrusServerLobby * lobby, int n, uint8_t * data,
			    int id);

#endif
