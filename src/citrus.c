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
#include <stddef.h>
#include "citrus.h"

const int CLEAR_SCORES[5] = {0, 100, 300, 500, 800};
const int MINI_T_SPIN_SCORES[4] = {100, 200, 400, 800};
const int T_SPIN_SCORES[4] = {400, 800, 1200, 1600};

void CitrusGameConfig_init(CitrusGameConfig *config,
			   const CitrusPiece *(*randomizer) (void *))
{
	config->width = 10;
	config->height = 20;
	config->full_height = 40;
	config->next_piece_queue_size = 3;
	config->gravity = 1.0 / 60.0;
	config->max_move_reset = 15;
	config->lock_delay = 30;
	config->randomizer = randomizer;
}

void CitrusPiece_init(CitrusPiece *piece, const CitrusCell *piece_data,
		      int n_rotation_states, int width, int height, int spawn_y)
{
	piece->piece_data = piece_data;
	piece->n_rotation_states = n_rotation_states;
	piece->width = width;
	piece->height = height;
	piece->spawn_y = spawn_y;
}

bool CitrusGame_collided(CitrusGame *game)
{
	int width = game->current_piece->width;
	int height = game->current_piece->height;
	int rotation = game->current_rotation;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (game->current_piece->piece_data[rotation * width *
							    height + y * width +
							    x].type !=
			    CITRUS_CELL_FULL)
				continue;
			if (x + game->current_x < 0 || y + game->current_y < 0
			    || x + game->current_x >= game->config.width
			    || y + game->current_y >= game->config.full_height
			    || game->board[(y + game->current_y) *
					   game->config.width + x +
					   game->current_x].type
			    == CITRUS_CELL_FULL) {
				return true;
			}
		}
	}
	return false;
}

void CitrusGame_draw_piece_inner(CitrusGame *game, CitrusCellType type)
{
	int width = game->current_piece->width;
	int height = game->current_piece->height;
	int rotation = game->current_rotation;
	for (int dy = 0; dy < height; dy++) {
		for (int dx = 0; dx < width; dx++) {
			CitrusCell cell =
			    game->current_piece->piece_data[rotation * width *
							    height +
							    dy * width + dx];
			if (cell.type != CITRUS_CELL_FULL)
				continue;
			int x = game->current_x + dx;
			int y = game->current_y + dy;
			if (x < 0 || x >= game->config.width || y < 0
			    || y >= game->config.full_height) {
				continue;
			}
			game->board[y * game->config.width + x] = (CitrusCell) {
			.type = type,.color = cell.color};
		}
	}
}

void CitrusGame_draw_piece(CitrusGame *game, bool clear)
{
	if (clear) {
		CitrusGame_draw_piece_inner(game, CITRUS_CELL_EMPTY);
	}
	int y = game->current_y;
	while (!CitrusGame_collided(game)) {
		game->current_y--;
	}
	game->current_y++;
	CitrusGame_draw_piece_inner(game,
				    clear ? CITRUS_CELL_EMPTY :
				    CITRUS_CELL_SHADOW);
	game->current_y = y;
	if (!clear) {
		CitrusGame_draw_piece_inner(game, CITRUS_CELL_FULL);
	}
}

void CitrusGame_reset_piece(CitrusGame *game)
{
	game->current_x = (game->config.width - game->current_piece->width) / 2;
	game->current_y = game->current_piece->spawn_y + game->config.height;
	game->fall_amount = 0;
	game->held = false;
	game->current_rotation = 0;
	game->lock_delay = game->config.lock_delay;
	game->move_reset_count = 0;
	game->lowest_y = game->current_y;
}

void CitrusGame_init(CitrusGame *game, CitrusCell *board,
		     const CitrusPiece **next_piece_queue,
		     CitrusGameConfig config, void *randomizer_data)
{
	game->config = config;
	game->randomizer_data = randomizer_data;
	game->board = board;
	game->next_piece_queue = next_piece_queue;
	game->current_piece = config.randomizer(randomizer_data);
	game->hold_piece = NULL;
	game->alive = true;
	game->score = 0;
	CitrusGame_reset_piece(game);
	for (int i = 0; i < config.width * config.full_height; i++) {
		board[i].type = CITRUS_CELL_EMPTY;
	}
	for (int i = 0; i < config.next_piece_queue_size; i++) {
		next_piece_queue[i] = config.randomizer(randomizer_data);
	}
	CitrusGame_draw_piece(game, false);
}

bool CitrusGame_move_piece(CitrusGame *game, int dx, int dy)
{
	CitrusGame_draw_piece(game, true);
	game->current_x += dx;
	game->current_y += dy;
	bool collided = CitrusGame_collided(game);
	if (collided) {
		game->current_x -= dx;
		game->current_y -= dy;
	}
	if (game->current_y < game->lowest_y) {
		game->lowest_y = game->current_y;
		game->move_reset_count = 0;
		game->lock_delay = game->config.lock_delay;
	}
	CitrusGame_draw_piece(game, false);
	return !collided;
}

const CitrusPiece *CitrusGame_next_piece(CitrusGame *game)
{
	if (game->config.next_piece_queue_size == 0) {
		return game->config.randomizer(game->randomizer_data);
	}
	const CitrusPiece *piece = game->next_piece_queue[0];
	for (int i = 0; i < game->config.next_piece_queue_size - 1; i++) {
		game->next_piece_queue[i] = game->next_piece_queue[i + 1];
	}
	game->next_piece_queue[game->config.next_piece_queue_size - 1] =
	    game->config.randomizer(game->randomizer_data);
	return piece;
}

void CitrusGame_lock_piece(CitrusGame *game)
{
	game->current_piece = CitrusGame_next_piece(game);
	CitrusGame_reset_piece(game);
	int cleared_lines = 0;
	for (int y = 0; y < game->config.full_height; y++) {
		int full = 1;
		for (int x = 0; x < game->config.width; x++) {
			if (game->board[y * game->config.width + x].type !=
			    CITRUS_CELL_FULL) {
				full = 0;
				break;
			}
		}
		if (full) {
			cleared_lines++;
			int n_cells =
			    game->config.width * (game->config.full_height - y -
						  1);
			for (int i = 0; i < n_cells; i++) {
				game->board[i + y * game->config.width] =
				    game->board[i +
						(y + 1) * game->config.width];
			}
			for (int x = 0; x < game->config.width; x++) {
				game->board[(game->config.full_height - 1) *
					    game->config.width + x].type =
				    CITRUS_CELL_EMPTY;
			}
			y--;
		}
	}
	if (cleared_lines > 4) {
		cleared_lines = 4;
	}
	game->score += CLEAR_SCORES[cleared_lines];
	if (CitrusGame_collided(game)) {
		game->alive = false;
		return;
	}
	CitrusGame_draw_piece(game, false);
}

bool CitrusGame_rotate_piece(CitrusGame *game, int n)
{
	bool success = true;
	CitrusGame_draw_piece(game, true);
	int prev_rotation = game->current_rotation;
	game->current_rotation += n + game->current_piece->n_rotation_states;
	game->current_rotation %= game->current_piece->n_rotation_states;
	if (CitrusGame_collided(game)) {
		game->current_rotation = prev_rotation;
		success = false;
	}
	CitrusGame_draw_piece(game, false);
	return success;
}

void CitrusGame_key_down(CitrusGame *game, CitrusKey key)
{
	if (!game->alive)
		return;
	bool moved = false;
	switch (key) {
	case CITRUS_KEY_LEFT:
		moved = CitrusGame_move_piece(game, -1, 0);
		break;
	case CITRUS_KEY_RIGHT:
		moved = CitrusGame_move_piece(game, 1, 0);
		break;
	case CITRUS_KEY_HARD_DROP:
		while (CitrusGame_move_piece(game, 0, -1))
			game->score += 2;
		CitrusGame_lock_piece(game);
		break;
	case CITRUS_KEY_SOFT_DROP:
		while (CitrusGame_move_piece(game, 0, -1))
			game->score++;
		break;
	case CITRUS_KEY_CLOCKWISE:
		moved = CitrusGame_rotate_piece(game, 1);
		break;
	case CITRUS_KEY_ANTICLOCKWISE:
		moved = CitrusGame_rotate_piece(game, -1);
		break;
	case CITRUS_KEY_180:
		moved = CitrusGame_rotate_piece(game, 2);
		break;
	case CITRUS_KEY_HOLD:
		if (game->held) {
			break;
		}
		CitrusGame_draw_piece(game, true);
		const CitrusPiece *piece = game->hold_piece;
		game->hold_piece = game->current_piece;
		if (piece == NULL) {
			game->current_piece = CitrusGame_next_piece(game);
		} else {
			game->current_piece = piece;
		}
		CitrusGame_reset_piece(game);
		CitrusGame_draw_piece(game, false);
		game->held = true;
		break;
	}
	if (moved && game->move_reset_count < game->config.max_move_reset) {
		game->lock_delay = game->config.lock_delay;
		game->move_reset_count++;
	}
}

void CitrusGame_tick(CitrusGame *game)
{
	CitrusGame_draw_piece(game, true);
	game->current_y--;
	bool on_ground = CitrusGame_collided(game);
	game->current_y++;
	CitrusGame_draw_piece(game, false);
	if (on_ground) {
		game->lock_delay--;
		if (game->lock_delay == 0) {
			CitrusGame_lock_piece(game);
		}
	} else {
		game->fall_amount += game->config.gravity;
		while (game->fall_amount >= 1) {
			game->fall_amount -= 1;
			CitrusGame_move_piece(game, 0, -1);
		}
	}
}

CitrusCell CitrusGame_get_cell(CitrusGame *game, int x, int y)
{
	if (x < 0 || x >= game->config.width || y < 0
	    || y >= game->config.full_height) {
		return (CitrusCell) {
		.type = CITRUS_CELL_EMPTY};
	}
	return game->board[y * game->config.width + x];
}

const CitrusPiece *CitrusGame_get_next_piece(CitrusGame *game, int i)
{
	return game->next_piece_queue[i];
}

bool CitrusGame_is_alive(CitrusGame *game)
{
	return game->alive;
}

void CitrusBagRandomizer_init(CitrusBagRandomizer *bag, int seed)
{
	bag->count = 0;
	for (int i = 0; i < 7; i++) {
		bag->chosen_pieces[i] = 0;
	}
	bag->state = seed;
}

const CitrusPiece *CitrusBagRandomizer_randomizer(void *data)
{
	CitrusBagRandomizer *bag = data;
	if (bag->count == 7) {
		for (int i = 0; i < 7; i++) {
			bag->chosen_pieces[i] = 0;
		}
		bag->count = 0;
	}
	bag->state *= 6364136223846793005ULL;
	bag->state++;
	while (bag->chosen_pieces[bag->state % 7] != 0) {
		bag->state++;
	}
	bag->chosen_pieces[bag->state % 7] = 1;
	bag->count++;
	return citrus_pieces + (bag->state % 7);
}
