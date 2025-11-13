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

#include <stdio.h>
#include <stdlib.h>
#include "tests.h"

CitrusCell board[10 * 40];
CitrusCell expected_board[10 * 40];
const CitrusPiece *next_piece_queue[3];

void assert_expected(void)
{
	for (int y = 0; y < 40; y++) {
		for (int x = 0; x < 10; x++) {
			bool cell = board[y * 10 + x].type == CITRUS_CELL_FULL;
			bool expected =
			    expected_board[y * 10 + x].type == CITRUS_CELL_FULL;
			if (cell != expected) {
				fprintf(stderr, "assert_expected(): "
					"expected %s cell at (%i, %i), got %s cell\n",
					expected ? "full" : "empty", x, y,
					cell ? "full" : "empty");
				abort();
			}
		}
	}
}

void clear_board(void)
{
	for (int i = 0; i < 40 * 10; i++) {
		expected_board[i].type = CITRUS_CELL_EMPTY;
	}
}

void set_piece(int x, int y, CitrusCellType type, CitrusColor color)
{
	expected_board[y * 10 + x].type = type;
	expected_board[y * 10 + x].color = color;
}

const CitrusPiece *loop_randomizer(void *data)
{
	LoopRandomizer *loop = data;
	const CitrusPiece *piece = loop->pieces[loop->position];
	loop->position++;
	loop->position %= loop->length;
	return piece;
}

int main()
{
	hard_drop_test();
	rotation_test();
	movement_test();
}
