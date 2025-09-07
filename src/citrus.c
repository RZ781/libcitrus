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

#include "citrus.h"

void CitrusGameConfig_init(CitrusGameConfig* config, const CitrusPiece* (*randomizer)(void)) {
	config->width = 10;
	config->height = 20;
	config->full_height = 40;
	config->randomizer = randomizer;
}

void CitrusPiece_init(CitrusPiece* piece, const char* piece_data, int n_rotation_states, int width, int height, int spawn_x, int spawn_y) {
	piece->piece_data = piece_data;
	piece->n_rotation_states = n_rotation_states;
	piece->width = width;
	piece->height = height;
	piece->spawn_x = spawn_x;
	piece->spawn_y = spawn_y;
}

void CitrusGame_init(CitrusGame* game, char* board, CitrusGameConfig config) {
	game->config = config;
	game->board = board;
	game->current_piece = config.randomizer();
	game->current_x = game->current_piece->spawn_x;
	game->current_y = game->current_piece->spawn_y;
	game->current_rotation = 0;
}

int CitrusGame_move_piece(CitrusGame* game, int dx, int dy) {
	game->current_x += dx;
	game->current_y += dy;
	int collided = 0;
	int width = game->current_piece->width;
	int height = game->current_piece->height;
	int rotation = game->current_rotation;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (game->current_piece->piece_data[rotation * width * height + y * width + x] == 0)
				continue;
			if (x + game->current_x < 0 || y + game->current_y < 0 || x + game->current_x >= game->config.width || y + game->current_y >= game->config.full_height || game->board[(y + game->current_y) * game->config.width + (x + game->current_x)] != 0) {
				collided = 1;
				break;
			}
		}
	}
	if (collided) {
		game->current_x -= dx;
		game->current_y -= dy;
		return 0;
	}
	return 1;
}

void CitrusGame_key_down(CitrusGame* game, CitrusKey key) {
	if (key == CITRUS_KEY_LEFT) {
		CitrusGame_move_piece(game, -1, 0);
	} else if (key == CITRUS_KEY_RIGHT) {
		CitrusGame_move_piece(game, 1, 0);
	}
}

int CitrusGame_get_cell(CitrusGame* game, int x, int y) {
	return game->board[y * game->config.width + x];
}
