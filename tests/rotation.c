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

void rotation_test()
{
	clear_board();
	CitrusGame game;
	LoopRandomizer randomizer_data = {.length = 1,.position = 0,.pieces =
		    (const CitrusPiece *[]) {citrus_pieces + CITRUS_COLOR_T}
	};
	CitrusGame_init(&game, board, next_piece_queue, test_config,
			&randomizer_data);

	clear_board();
	set_piece(3, 21, CITRUS_CELL_FULL, CITRUS_COLOR_T);
	set_piece(4, 21, CITRUS_CELL_FULL, CITRUS_COLOR_T);
	set_piece(5, 21, CITRUS_CELL_FULL, CITRUS_COLOR_T);
	set_piece(4, 22, CITRUS_CELL_FULL, CITRUS_COLOR_T);
	assert_expected();

	CitrusGame_key_down(&game, CITRUS_KEY_CLOCKWISE);
	set_piece(4, 20, CITRUS_CELL_FULL, CITRUS_COLOR_T);
	set_piece(3, 21, CITRUS_CELL_EMPTY, 0);
	assert_expected();

	CitrusGame_key_down(&game, CITRUS_KEY_CLOCKWISE);
	set_piece(3, 21, CITRUS_CELL_FULL, CITRUS_COLOR_T);
	set_piece(4, 22, CITRUS_CELL_EMPTY, 0);
	assert_expected();

	CitrusGame_key_down(&game, CITRUS_KEY_CLOCKWISE);
	set_piece(4, 22, CITRUS_CELL_FULL, CITRUS_COLOR_T);
	set_piece(5, 21, CITRUS_CELL_EMPTY, 0);
	assert_expected();

	CitrusGame_key_down(&game, CITRUS_KEY_CLOCKWISE);
	set_piece(5, 21, CITRUS_CELL_FULL, CITRUS_COLOR_T);
	set_piece(4, 20, CITRUS_CELL_EMPTY, 0);
	assert_expected();

	CitrusGame_key_down(&game, CITRUS_KEY_ANTICLOCKWISE);
	set_piece(4, 20, CITRUS_CELL_FULL, CITRUS_COLOR_T);
	set_piece(5, 21, CITRUS_CELL_EMPTY, 0);
	assert_expected();

	CitrusGame_key_down(&game, CITRUS_KEY_ANTICLOCKWISE);
	set_piece(5, 21, CITRUS_CELL_FULL, CITRUS_COLOR_T);
	set_piece(4, 22, CITRUS_CELL_EMPTY, 0);
	assert_expected();

	CitrusGame_key_down(&game, CITRUS_KEY_ANTICLOCKWISE);
	set_piece(4, 22, CITRUS_CELL_FULL, CITRUS_COLOR_T);
	set_piece(3, 21, CITRUS_CELL_EMPTY, 0);
	assert_expected();

	CitrusGame_key_down(&game, CITRUS_KEY_ANTICLOCKWISE);
	set_piece(3, 21, CITRUS_CELL_FULL, CITRUS_COLOR_T);
	set_piece(4, 20, CITRUS_CELL_EMPTY, 0);
	assert_expected();

	CitrusGame_key_down(&game, CITRUS_KEY_CLOCKWISE);
	CitrusGame_key_down(&game, CITRUS_KEY_LEFT);
	CitrusGame_key_down(&game, CITRUS_KEY_LEFT);
	CitrusGame_key_down(&game, CITRUS_KEY_LEFT);
	CitrusGame_key_down(&game, CITRUS_KEY_LEFT);
	CitrusGame_key_down(&game, CITRUS_KEY_ANTICLOCKWISE);
	clear_board();
	set_piece(1, 21, CITRUS_CELL_FULL, CITRUS_COLOR_T);
	set_piece(1, 22, CITRUS_CELL_FULL, CITRUS_COLOR_T);
	set_piece(0, 21, CITRUS_CELL_FULL, CITRUS_COLOR_T);
	set_piece(2, 21, CITRUS_CELL_FULL, CITRUS_COLOR_T);
	assert_expected();
}
