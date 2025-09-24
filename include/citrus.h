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
	const CitrusPiece* (*randomizer)(void*);
} CitrusGameConfig;

typedef struct {
	CitrusGameConfig config;
	CitrusCell* board;
	void* randomizer_data;
	const CitrusPiece* current_piece;
	int current_x;
	int current_y;
	int current_rotation;
} CitrusGame;

extern const CitrusPiece pieces[7];

void CitrusGameConfig_init(CitrusGameConfig* config, const CitrusPiece* (*randomizer)(void*));
void CitrusPiece_init(CitrusPiece* piece, const CitrusCell* piece_data, int n_rotation_states, int width, int height, int spawn_x, int spawn_y);
void CitrusGame_init(CitrusGame* game, CitrusCell* board, CitrusGameConfig config, void* randomizer_data);
void CitrusGame_key_down(CitrusGame* game, CitrusKey key);
CitrusCell CitrusGame_get_cell(CitrusGame* game, int x, int y);

#endif
