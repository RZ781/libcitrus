/* Copyright (C) 2025 RZ781
 *
 * This file is part of libcitrus.
 *
 * libcitrus is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * libcitrus is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "citrus.h"
#include "tests.h"

void movement_test(void)
{
	clear_board();
	CitrusGame game;
	LoopRandomizer randomizer_data = {.length = 1,.position = 0,.pieces =
		    (const CitrusPiece *[]) {citrus_pieces + CITRUS_COLOR_O}
	};
	CitrusGame_init(&game, board, next_piece_queue, test_config,
			&randomizer_data, NULL);
	int x = 4;
	for (int i = 0; i < 4; i++) {
		CitrusGame_key_down(&game, CITRUS_KEY_LEFT);
		x -= 1;
		clear_board();
		set_o_piece(x, 21, CITRUS_CELL_FULL);
		assert_expected();
	}
	CitrusGame_key_down(&game, CITRUS_KEY_LEFT);
	clear_board();
	set_o_piece(x, 21, CITRUS_CELL_FULL);
	assert_expected();
	for (int i = 0; i < 8; i++) {
		CitrusGame_key_down(&game, CITRUS_KEY_RIGHT);
		x += 1;
		clear_board();
		set_o_piece(x, 21, CITRUS_CELL_FULL);
		assert_expected();
	}
	CitrusGame_key_down(&game, CITRUS_KEY_RIGHT);
	clear_board();
	set_o_piece(x, 21, CITRUS_CELL_FULL);
	assert_expected();
}
