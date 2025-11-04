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
#include <stdbool.h>
#include <stdlib.h>
#include "citrus.h"
#include "tests.h"

CitrusGame game;
CitrusGameConfig config;

void assert_position(int piece_x, int piece_y) {
	for (int y = 0; y < 40; y++) {
		for (int x = 0; x < 10; x++) {
			bool cell = CitrusGame_get_cell(&game, x, y).type == CITRUS_CELL_FULL;
			bool expected = x >= piece_x && y >= piece_y && x <= piece_x + 1 && y <= piece_y + 1;
			if (cell != expected) {
				fprintf(stderr, "assert_position(%i, %i): expected %s cell at (%i, %i), got %s cell\n", piece_x, piece_y, expected ? "full" : "empty", x, y, cell ? "full" : "empty");
				exit(-1);
			}
		}
	}
}

void movement_test() {
	clear_board();
	CitrusGameConfig_init(&config, single_piece_randomizer);
	CitrusGame_init(&game, board, next_piece_queue, config, citrus_pieces + CITRUS_COLOR_O);
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
}
