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

CitrusGame game;
CitrusGameConfig config;
CitrusPiece piece;
const CitrusCell cell = {.colour = CITRUS_COLOUR_O, .type = CITRUS_CELL_FULL};
const CitrusCell piece_data[1 * 2 * 2] = {cell, cell, cell, cell};
CitrusCell board[10 * 40];

void assert_position(int piece_x, int piece_y) {
	for (int y = 0; y < 40; y++) {
		for (int x = 0; x < 10; x++) {
			CitrusCell current_cell = CitrusGame_get_cell(&game, x, y);
			CitrusCell expected = (x >= piece_x && y >= piece_y && x <= piece_x + 1 && y <= piece_y + 1) ? cell : (CitrusCell) {.type = CITRUS_CELL_EMPTY};
			if ((current_cell.type == CITRUS_CELL_FULL) != (expected.type == CITRUS_CELL_FULL)) {
				fprintf(stderr, "assert_position(%i, %i): expected cell type %i at (%i, %i), got %i\n", piece_x, piece_y, expected.type, x, y, current_cell.type);
				exit(-1);
			}
		}
	}
}

const CitrusPiece* randomizer(void* data) {
	(void) data;
	return &piece;
}

int main() {
	CitrusGameConfig_init(&config, randomizer);
	CitrusPiece_init(&piece, piece_data, 1, 2, 2, 4, 21);
	CitrusGame_init(&game, board, config, NULL);
	int x = 4;
	for (int i = 0; i < 4; i++) {
		CitrusGame_key_down(&game, CITRUS_KEY_LEFT);
		x -= 1;
		assert_position(x, 21);
	}
	CitrusGame_key_down(&game, CITRUS_KEY_LEFT);
	assert_position(x, 21);
	for (int i = 0; i < 8; i++) {
		CitrusGame_key_down(&game, CITRUS_KEY_RIGHT);
		x += 1;
		assert_position(x, 21);
	}
	CitrusGame_key_down(&game, CITRUS_KEY_RIGHT);
	assert_position(x, 21);
	return 0;
}
