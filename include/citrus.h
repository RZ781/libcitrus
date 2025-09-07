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
	CITRUS_KEY_RIGHT
} CitrusKey;

typedef struct {
	const char* piece_data;
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
	const CitrusPiece* (*randomizer)(void);
} CitrusGameConfig;

typedef struct {
	CitrusGameConfig config;
	char* board;
	const CitrusPiece* current_piece;
	int current_x;
	int current_y;
	int current_rotation;
} CitrusGame;

void CitrusGameConfig_init(CitrusGameConfig* config, const CitrusPiece* (*randomizer)(void));
void CitrusPiece_init(CitrusPiece* piece, const char* piece_data, int n_rotation_states, int width, int height, int spawn_x, int spawn_y);
void CitrusGame_init(CitrusGame* game, char* board, CitrusGameConfig config);
void CitrusGame_key_down(CitrusGame* game, CitrusKey key);
int CitrusGame_get_cell(CitrusGame* game, int x, int y);

#endif
