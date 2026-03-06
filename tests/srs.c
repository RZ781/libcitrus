/* Copyright (C) 2026 RZ781
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

typedef struct {
	int x;
	int y;
	int rotation;
} Location;

typedef struct {
	int width;
	int height;
	CitrusColor piece;
	Location spawn;
	Location final;
	CitrusKey key;
	int *board;
} SRSTest;

SRSTest test_cases[] = {
	// T north to west kick 1
	{
	 .width = 3,
	 .height = 3,
	 .piece = CITRUS_COLOR_T,
	 .spawn = {0, 0, 0},
	 .final = {1, 0, 3},
	 .key = CITRUS_KEY_ANTICLOCKWISE,
	 .board = (int[]) {
			   0, 0, 0,
			   0, 0, 0,
			   1, 1, 0,
			   }
	 },
	// T north to west kick 2
	{
	 .width = 3,
	 .height = 4,
	 .piece = CITRUS_COLOR_T,
	 .spawn = {0, 0, 0},
	 .final = {1, 1, 3},
	 .key = CITRUS_KEY_ANTICLOCKWISE,
	 .board = (int[]) {
			   0, 0, 0,
			   0, 0, 0,
			   0, 0, 0,
			   1, 1, 1,
			   }
	 },
	// T north to west kick 4 (TST)
	{
	 .width = 3,
	 .height = 5,
	 .piece = CITRUS_COLOR_T,
	 .spawn = {0, 2, 0},
	 .final = {1, 0, 3},
	 .key = CITRUS_KEY_ANTICLOCKWISE,
	 .board = (int[]) {
			   0, 0, 1,
			   0, 0, 0,
			   1, 1, 0,
			   1, 0, 0,
			   1, 1, 0,
			   }
	 },
	// T west to south kick 1
	{
	 .width = 3,
	 .height = 3,
	 .piece = CITRUS_COLOR_T,
	 .spawn = {1, 0, 3},
	 .final = {0, 0, 2},
	 .key = CITRUS_KEY_ANTICLOCKWISE,
	 .board = (int[9]) {}
	 },
	// T west to south kick 2
	{
	 .width = 3,
	 .height = 4,
	 .piece = CITRUS_COLOR_T,
	 .spawn = {1, 1, 3},
	 .final = {0, 0, 2},
	 .key = CITRUS_KEY_ANTICLOCKWISE,
	 .board = (int[]) {
			   0, 0, 0,
			   1, 0, 0,
			   0, 0, 0,
			   1, 0, 1,
			   }
	 },
	// T west to south kick 3
	{
	 .width = 3,
	 .height = 4,
	 .piece = CITRUS_COLOR_T,
	 .spawn = {0, 0, 3},
	 .final = {0, 2, 2},
	 .key = CITRUS_KEY_ANTICLOCKWISE,
	 .board = (int[]) {
			   0, 0, 0,
			   0, 0, 1,
			   0, 0, 1,
			   0, 0, 0,
			   }
	 },
	// T west to south kick 4
	{
	 .width = 3,
	 .height = 4,
	 .piece = CITRUS_COLOR_T,
	 .spawn = {1, 0, 3},
	 .final = {0, 2, 2},
	 .key = CITRUS_KEY_ANTICLOCKWISE,
	 .board = (int[]) {
			   0, 0, 0,
			   0, 0, 0,
			   0, 0, 0,
			   0, 1, 0,
			   }
	 },
	// T south to east kick 1
	{
	 .width = 4,
	 .height = 3,
	 .piece = CITRUS_COLOR_T,
	 .spawn = {1, 0, 2},
	 .final = {0, 0, 1},
	 .key = CITRUS_KEY_ANTICLOCKWISE,
	 .board = (int[]) {
			   0, 0, 1, 0,
			   0, 0, 0, 0,
			   0, 0, 0, 0,
			   }
	 },
	// T south to east kick 3 (iso TSD)
	{
	 .width = 3,
	 .height = 5,
	 .piece = CITRUS_COLOR_T,
	 .spawn = {0, 2, 2},
	 .final = {0, 0, 1},
	 .key = CITRUS_KEY_ANTICLOCKWISE,
	 .board = (int[]) {
			   1, 1, 1,
			   0, 0, 0,
			   0, 0, 1,
			   1, 0, 0,
			   1, 0, 1,
			   }
	 },
	// T south to east kick 3 (neo TSD)
	{
	 .width = 3,
	 .height = 5,
	 .piece = CITRUS_COLOR_T,
	 .spawn = {0, 2, 2},
	 .final = {0, 0, 1},
	 .key = CITRUS_KEY_ANTICLOCKWISE,
	 .board = (int[]) {
			   1, 1, 0,
			   0, 0, 0,
			   1, 0, 0,
			   1, 0, 0,
			   1, 0, 1,
			   }
	 },
	// T south to east kick 4 (fin TSD)
	{
	 .width = 4,
	 .height = 5,
	 .piece = CITRUS_COLOR_T,
	 .spawn = {1, 2, 2},
	 .final = {0, 0, 1},
	 .key = CITRUS_KEY_ANTICLOCKWISE,
	 .board = (int[]) {
			   1, 1, 1, 0,
			   1, 0, 0, 0,
			   1, 0, 0, 0,
			   1, 0, 0, 1,
			   1, 0, 1, 1,
			   }
	 },
	// T east to north kick 1
	{
	 .width = 4,
	 .height = 3,
	 .piece = CITRUS_COLOR_T,
	 .spawn = {0, 0, 1},
	 .final = {1, 0, 0},
	 .key = CITRUS_KEY_ANTICLOCKWISE,
	 .board = (int[]) {
			   0, 0, 0, 0,
			   1, 0, 0, 0,
			   0, 0, 1, 0,
			   }
	 },
	// T east to north kick 2
	{
	 .width = 4,
	 .height = 4,
	 .piece = CITRUS_COLOR_T,
	 .spawn = {0, 1, 1},
	 .final = {1, 0, 0},
	 .key = CITRUS_KEY_ANTICLOCKWISE,
	 .board = (int[]) {
			   0, 0, 0, 0,
			   1, 0, 0, 1,
			   0, 0, 0, 0,
			   0, 0, 0, 0,
			   }
	 },
	// T east to north kick 3
	{
	 .width = 3,
	 .height = 5,
	 .piece = CITRUS_COLOR_T,
	 .spawn = {0, 0, 1},
	 .final = {0, 2, 0},
	 .key = CITRUS_KEY_ANTICLOCKWISE,
	 .board = (int[]) {
			   0, 0, 0,
			   0, 0, 0,
			   0, 0, 0,
			   1, 0, 0,
			   0, 0, 0,
			   }
	 },
	// T east to north kick 4
	{
	 .width = 4,
	 .height = 5,
	 .piece = CITRUS_COLOR_T,
	 .spawn = {0, 0, 1},
	 .final = {1, 2, 0},
	 .key = CITRUS_KEY_ANTICLOCKWISE,
	 .board = (int[]) {
			   0, 1, 0, 0,
			   0, 0, 0, 0,
			   0, 0, 0, 0,
			   1, 0, 0, 1,
			   0, 0, 1, 0,
			   }
	 },
};

void srs_test(void)
{
	int n_test_cases = sizeof(test_cases) / sizeof(*test_cases);
	for (int i = 0; i < n_test_cases; i++) {
		// initialise game with blank board
		SRSTest test_case = test_cases[i];
		CitrusGameConfig config = test_config;
		config.width = test_case.width;
		config.height = test_case.height;
		CitrusGame game;
		LoopRandomizer randomizer_data = {.length = 1,.position =
			    0,.pieces =
			    (const CitrusPiece *[]) {citrus_pieces +
						     test_case.piece}
		};
		CitrusGame_init(&game, board, next_piece_queue, config,
				&randomizer_data, NULL);
		// initialise board
		for (int x = 0; x < test_case.width; x++) {
			for (int y = 0; y < test_case.height; y++) {
				CitrusCell cell;
				if (test_case.board[y * test_case.width + x]) {
					cell.color = CITRUS_COLOR_O;
					cell.type = CITRUS_CELL_FULL;
				} else {
					cell.type = CITRUS_CELL_EMPTY;
				}
				board[(test_case.height - y -
				       1) * test_case.width + x] = cell;
			}
		}
		// run test
		game.position.x = test_case.spawn.x;
		game.position.y = test_case.spawn.y;
		game.rotation = test_case.spawn.rotation;
		CitrusGame_key_down(&game, test_case.key);
		if (game.position.x != test_case.final.x
		    || game.position.y != test_case.final.y
		    || game.rotation != test_case.final.rotation) {
			fprintf(stderr, "srs_test(): test_cases[%i]: expected "
				"position (%i, %i, %i), got (%i, %i, %i)\n",
				i, test_case.final.x, test_case.final.y,
				test_case.final.rotation, game.position.x,
				game.position.y, game.rotation);
			abort();
		}
	}
}
