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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "citrus.h"

const CitrusCell cell = {.colour = CITRUS_COLOUR_O, .type = CITRUS_CELL_FULL};
const CitrusCell piece_data[1 * 2 * 2] = {cell, cell, cell, cell};
CitrusPiece piece;
CitrusCell board[10 * 40];
CitrusCell expected_board[10 * 40];

void assert_expected(void) {
	for (int y = 0; y < 40; y++) {
		for (int x = 0; x < 10; x++) {
			CitrusCell cell = board[y * 10 + x];
			CitrusCell expected_cell = expected_board[y * 10 + x];
			if (cell.type != expected_cell.type) {
				fprintf(stderr, "assert_expected(): expected cell type %i at (%i, %i), got %i\n", expected_cell.type, x, y, cell.type);
				abort();
			}
		}
	}
}

void add_piece(int x, int y, int type) {
	CitrusCell cell;
	cell.type = type;
	cell.colour = CITRUS_COLOUR_O;
	expected_board[y * 10 + x] = cell;
	expected_board[y * 10 + (x + 1)] = cell;
	expected_board[(y + 1) * 10 + x] = cell;
	expected_board[(y + 1) * 10 + (x + 1)] = cell;
}

const CitrusPiece* randomizer(void* data) {
	(void) data;
	return &piece;
}

int main() {
	CitrusGame game;
	CitrusGameConfig config;
	CitrusGameConfig_init(&config, randomizer);
	CitrusPiece_init(&piece, piece_data, 1, 2, 2, 4, 21);
	CitrusGame_init(&game, board, config, NULL);
	for (int i = 0; i < config.width * config.full_height; i++) {
		expected_board[i].type = CITRUS_CELL_EMPTY;
	}
	add_piece(4, 21, CITRUS_CELL_FULL);
	add_piece(4, 0, CITRUS_CELL_SHADOW);
	assert_expected();
	CitrusGame_key_down(&game, CITRUS_KEY_HARD_DROP);
	add_piece(4, 0, CITRUS_CELL_FULL);
	add_piece(4, 2, CITRUS_CELL_SHADOW);
	assert_expected();
	CitrusGame_key_down(&game, CITRUS_KEY_HARD_DROP);
	add_piece(4, 2, CITRUS_CELL_FULL);
	add_piece(4, 4, CITRUS_CELL_SHADOW);
	assert_expected();
	CitrusGame_key_down(&game, CITRUS_KEY_LEFT);
	CitrusGame_key_down(&game, CITRUS_KEY_LEFT);
	CitrusGame_key_down(&game, CITRUS_KEY_HARD_DROP);
	add_piece(2, 0, CITRUS_CELL_FULL);
	assert_expected();
	return 0;
}
