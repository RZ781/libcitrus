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

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "citrus.h"

void CitrusGameConfig_init(CitrusGameConfig* config, const CitrusPiece* (*randomizer)(void*)) {
	config->width = 10;
	config->height = 20;
	config->full_height = 40;
	config->gravity = 1.0 / 60.0;
	config->randomizer = randomizer;
}

void CitrusPiece_init(CitrusPiece* piece, const CitrusCell* piece_data, int n_rotation_states, int width, int height, int spawn_x, int spawn_y) {
	piece->piece_data = piece_data;
	piece->n_rotation_states = n_rotation_states;
	piece->width = width;
	piece->height = height;
	piece->spawn_x = spawn_x;
	piece->spawn_y = spawn_y;
}

bool CitrusGame_collided(CitrusGame* game) {
	int width = game->current_piece->width;
	int height = game->current_piece->height;
	int rotation = game->current_rotation;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (game->current_piece->piece_data[rotation * width * height + y * width + x].type != CITRUS_CELL_FULL)
				continue;
			if (x + game->current_x < 0 || y + game->current_y < 0 || x + game->current_x >= game->config.width || y + game->current_y >= game->config.full_height || game->board[(y + game->current_y) * game->config.width + (x + game->current_x)].type == CITRUS_CELL_FULL) {
				return true;
			}
		}
	}
	return false;
}

void CitrusGame_draw_piece_inner(CitrusGame* game, CitrusCellType type) {
	int width = game->current_piece->width;
	int height = game->current_piece->height;
	int rotation = game->current_rotation;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			CitrusCell cell = game->current_piece->piece_data[rotation * width * height + y * width + x];
			if (cell.type != CITRUS_CELL_FULL)
				continue;
			game->board[(y + game->current_y) * game->config.width + (x + game->current_x)] = (CitrusCell) {.type = type, .color = cell.color};
		}
	}
}

void CitrusGame_draw_piece(CitrusGame* game, bool clear) {
	if (clear) {
		CitrusGame_draw_piece_inner(game, CITRUS_CELL_EMPTY);
	}
	double y = game->current_y;
	while (!CitrusGame_collided(game)) {
		game->current_y--;
	}
	game->current_y++;
	CitrusGame_draw_piece_inner(game, clear ? CITRUS_CELL_EMPTY : CITRUS_CELL_SHADOW);
	game->current_y = y;
	if (!clear) {
		CitrusGame_draw_piece_inner(game, CITRUS_CELL_FULL);
	}
}

void CitrusGame_init(CitrusGame* game, CitrusCell* board, CitrusGameConfig config, void* randomizer_data) {
	game->config = config;
	game->randomizer_data = randomizer_data;
	game->board = board;
	game->current_piece = config.randomizer(randomizer_data);
	game->current_x = game->current_piece->spawn_x;
	game->current_y = game->current_piece->spawn_y;
	game->current_rotation = 0;
	game->alive = true;
	game->fall_amount = 0;
	for (int i = 0; i < config.width * config.full_height; i++) {
		board[i].type = CITRUS_CELL_EMPTY;
	}
	CitrusGame_draw_piece(game, false);
}

bool CitrusGame_move_piece(CitrusGame* game, int dx, int dy) {
	CitrusGame_draw_piece(game, true);
	game->current_x += dx;
	game->current_y += dy;
	bool collided = CitrusGame_collided(game);
	if (collided) {
		game->current_x -= dx;
		game->current_y -= dy;
	}
	CitrusGame_draw_piece(game, false);
	return !collided;
}

void CitrusGame_lock_piece(CitrusGame* game) {
	game->current_piece = game->config.randomizer(game->randomizer_data);
	game->current_x = game->current_piece->spawn_x;
	game->current_y = game->current_piece->spawn_y;
	game->current_rotation = 0;
	for (int y = 0; y < game->config.full_height; y++) {
		int full = 1;
		for (int x = 0; x < game->config.width; x++) {
			if (game->board[y * game->config.width + x].type != CITRUS_CELL_FULL) {
				full = 0;
				break;
			}
		}
		if (full) {
			memmove(game->board + y * game->config.width, game->board + (y + 1) * game->config.width, sizeof(CitrusCell) * game->config.width * (game->config.full_height - y - 1));
			for (int x = 0; x < game->config.width; x++) {
				game->board[(game->config.full_height - 1) * game->config.width + x].type = CITRUS_CELL_EMPTY;
			}
			y--;
		}
	}
	if (CitrusGame_collided(game)) {
		game->alive = false;
		return;
	}
	CitrusGame_draw_piece(game, false);
}

void CitrusGame_rotate_piece(CitrusGame* game, int n) {
	CitrusGame_draw_piece(game, true);
	int prev_rotation = game->current_rotation;
	game->current_rotation += n + game->current_piece->n_rotation_states;
	game->current_rotation %= game->current_piece->n_rotation_states;
	if (CitrusGame_collided(game)) {
		game->current_rotation = prev_rotation;
	}
	CitrusGame_draw_piece(game, false);
}

void CitrusGame_key_down(CitrusGame* game, CitrusKey key) {
	if (!game->alive)
		return;
	if (key == CITRUS_KEY_LEFT) {
		CitrusGame_move_piece(game, -1, 0);
	} else if (key == CITRUS_KEY_RIGHT) {
		CitrusGame_move_piece(game, 1, 0);
	} else if (key == CITRUS_KEY_HARD_DROP) {
		while (CitrusGame_move_piece(game, 0, -1));
		CitrusGame_lock_piece(game);
	} else if (key == CITRUS_KEY_SOFT_DROP) {
		while (CitrusGame_move_piece(game, 0, -1));
	} else if (key == CITRUS_KEY_CLOCKWISE) {
		CitrusGame_rotate_piece(game, 1);
	} else if (key == CITRUS_KEY_ANTICLOCKWISE) {
		CitrusGame_rotate_piece(game, -1);
	}
}

void CitrusGame_tick(CitrusGame* game) {
	CitrusGame_draw_piece(game, true);
	game->fall_amount += game->config.gravity;
	while (game->fall_amount >= 1) {
		game->fall_amount -= 1;
		game->current_y--;
		if (CitrusGame_collided(game)) {
			game->current_y++;
			game->fall_amount = 0;
			break;
		}
	}
	CitrusGame_draw_piece(game, false);
}

CitrusCell CitrusGame_get_cell(CitrusGame* game, int x, int y) {
	return game->board[y * game->config.width + x];
}

bool CitrusGame_is_alive(CitrusGame* game) {
	return game->alive;
}

void CitrusBagRandomizer_init(CitrusBagRandomizer* bag, int seed) {
	memset(bag, 0, sizeof(*bag));
	bag->state = seed;
}

const CitrusPiece* CitrusBagRandomizer_randomizer(void* data) {
	CitrusBagRandomizer* bag = data;
	if (bag->count == 7) {
		memset(bag->chosen_pieces, 0, sizeof(bag->chosen_pieces));
		bag->count = 0;
	}
	srand(bag->state);
	bag->state = rand();
	while (bag->chosen_pieces[bag->state % 7] != 0) {
		bag->state++;
	}
	bag->chosen_pieces[bag->state % 7] = 1;
	bag->count++;
	return citrus_pieces + (bag->state % 7);
}
