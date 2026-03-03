/* Copyright (C) 2025-2026 RZ781
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

#include <stdio.h>
#include <stdlib.h>
#include "citrus.h"
#include "tests.h"

void hard_drop_test(void)
{
	CitrusGame game;
	// infinite o pieces
	LoopRandomizer randomizer_data = {.length = 1,.position = 0,.pieces =
		    (const CitrusPiece *[]) {citrus_pieces + CITRUS_COLOR_O}
	};
	CitrusGame_init(&game, board, next_piece_queue, test_config,
			&randomizer_data, NULL);

	// o piece is at spawn position
	clear_board();
	set_o_piece(4, 21, CITRUS_CELL_FULL);
	assert_expected();
	// o piece hard dropped at center
	CitrusGame_key_down(&game, CITRUS_KEY_HARD_DROP);
	set_o_piece(4, 0, CITRUS_CELL_FULL);
	assert_expected();
	// second o piece hard dropped at center
	CitrusGame_key_down(&game, CITRUS_KEY_HARD_DROP);
	set_o_piece(4, 2, CITRUS_CELL_FULL);
	assert_expected();
	// o piece hard dropped left of center
	CitrusGame_key_down(&game, CITRUS_KEY_LEFT);
	CitrusGame_key_down(&game, CITRUS_KEY_LEFT);
	CitrusGame_key_down(&game, CITRUS_KEY_HARD_DROP);
	set_o_piece(2, 0, CITRUS_CELL_FULL);
	assert_expected();
	// place o pieces to top
	for (int i = 0; i < 8; i++) {
		CitrusGame_key_down(&game, CITRUS_KEY_HARD_DROP);
		set_o_piece(4, 4 + i * 2, CITRUS_CELL_FULL);
	}
	assert_expected();
	if (!CitrusGame_is_alive(&game)) {
		fprintf(stderr, "hard_drop_test(): expected alive, is dead\n");
		abort();
	}
	// top out
	CitrusGame_key_down(&game, CITRUS_KEY_HARD_DROP);
	set_o_piece(4, 21, CITRUS_CELL_EMPTY);
	set_o_piece(4, 20, CITRUS_CELL_FULL);
	assert_expected();
	if (CitrusGame_is_alive(&game)) {
		fprintf(stderr, "hard_drop_test(): expected dead, is alive\n");
		abort();
	}
}
