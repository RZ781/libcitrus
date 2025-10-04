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
	CITRUS_KEY_ANTICLOCKWISE
} CitrusKey;

typedef enum {
	CITRUS_CELL_EMPTY,
	CITRUS_CELL_SHADOW,
	CITRUS_CELL_FULL
} CitrusCellType;

typedef enum {
	CITRUS_COLOUR_I,
	CITRUS_COLOUR_J,
	CITRUS_COLOUR_L,
	CITRUS_COLOUR_O,
	CITRUS_COLOUR_S,
	CITRUS_COLOUR_T,
	CITRUS_COLOUR_Z
} CitrusColour;

typedef struct {
	CitrusColour colour;
	CitrusCellType type;
} CitrusCell;

typedef struct {
	const CitrusCell* piece_data;
	int n_rotation_states;
	int width;
	int height;
	int spawn_x;
	int spawn_y;
} CitrusPiece;

typedef struct {
	int width;
	int height;
	int full_height;
	double gravity;
	const CitrusPiece* (*randomizer)(void*);
} CitrusGameConfig;

typedef struct {
	CitrusGameConfig config;
	CitrusCell* board;
	void* randomizer_data;
	const CitrusPiece* current_piece;
	int current_x;
	int current_y;
	double fall_amount;
	int current_rotation;
	bool alive;
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
	void (*send)(void* send_data, int n, uint8_t* data);
	void* send_data;
} CitrusClientLobby;

typedef struct {
	CitrusLobby lobby;
	CitrusParser parsers[256];
	void (*send)(void* send_data, int n, uint8_t* data, int id);
	void* send_data;
} CitrusServerLobby;

typedef struct {
	int state;
	int chosen_pieces[7];
	int count;
} CitrusBagRandomizer;

extern const CitrusPiece citrus_pieces[7];

void CitrusGameConfig_init(CitrusGameConfig* config, const CitrusPiece* (*randomizer)(void*));

void CitrusPiece_init(CitrusPiece* piece, const CitrusCell* piece_data, int n_rotation_states, int width, int height, int spawn_x, int spawn_y);

void CitrusGame_init(CitrusGame* game, CitrusCell* board, CitrusGameConfig config, void* randomizer_data);
void CitrusGame_key_down(CitrusGame* game, CitrusKey key);
void CitrusGame_tick(CitrusGame* game);
bool CitrusGame_is_alive(CitrusGame* game);
CitrusCell CitrusGame_get_cell(CitrusGame* game, int x, int y);

void CitrusClientLobby_init(CitrusClientLobby* lobby, void (*send)(void* send_data, int n, uint8_t* data), void* send_data);
void CitrusClientLobby_recv(CitrusClientLobby* lobby, int n, uint8_t* data);

void CitrusServerLobby_init(CitrusServerLobby* lobby, void (*send)(void* send_data, int n, uint8_t* data, int id), void* send_data);
void CitrusServerLobby_client_connect(CitrusServerLobby* lobby, int id);
void CitrusServerLobby_client_disconnect(CitrusServerLobby* lobby, int id);
void CitrusServerLobby_recv(CitrusServerLobby* lobby, int n, uint8_t* data, int id);

void CitrusBagRandomizer_init(CitrusBagRandomizer* bag, int seed);
const CitrusPiece* CitrusBagRandomizer_randomizer(void* data);

#endif
