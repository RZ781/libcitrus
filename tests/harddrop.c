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

#include "citrus.h"
#include "tests.h"

void add_piece(int x, int y, CitrusCellType type)
{
	set_piece(x, y, type, CITRUS_COLOR_O);
	set_piece(x + 1, y, type, CITRUS_COLOR_O);
	set_piece(x, y + 1, type, CITRUS_COLOR_O);
	set_piece(x + 1, y + 1, type, CITRUS_COLOR_O);
}

void hard_drop_test()
{
	CitrusGame game;
	LoopRandomizer randomizer_data = {.length = 1,.position = 0,.pieces =
		    (const CitrusPiece *[]) {citrus_pieces + CITRUS_COLOR_O}
	};
	CitrusGame_init(&game, board, next_piece_queue, test_config,
			&randomizer_data);

	clear_board();
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
}
