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

const CitrusCell cell = {.colour = CITRUS_COLOUR_T, .type = CITRUS_CELL_FULL};
const CitrusCell empty = {.type = CITRUS_CELL_EMPTY};
const CitrusCell piece_data[4 * 3 * 3] = {
	empty, empty, empty,
	cell,  cell,  cell,
	empty, cell,  empty,

	empty, cell,  empty,
	empty, cell,  cell,
	empty, cell,  empty,

	empty, cell,  empty,
	cell,  cell,  cell,
	empty, empty, empty,

	empty, cell,  empty,
	cell,  cell,  empty,
	empty, cell,  empty
};
CitrusPiece piece;
CitrusCell board[10 * 40];
CitrusCell expected_board[10 * 40];

void assert_expected(void) {
	for (int y = 0; y < 40; y++) {
		for (int x = 0; x < 10; x++) {
			CitrusCell cell = board[y * 10 + x];
			CitrusCell expected_cell = expected_board[y * 10 + x];
			if ((cell.type == CITRUS_CELL_FULL) != (expected_cell.type == CITRUS_CELL_FULL)) {
				fprintf(stderr, "assert_expected(): expected cell type %i at (%i, %i), got %i\n", expected_cell.type, x, y, cell.type);
				abort();
			}
		}
	}
}


void clear_board(void) {
	for (int i=0; i<40*10; i++)
		expected_board[i].type = CITRUS_CELL_EMPTY;
}
void add_piece(int x, int y) {
	expected_board[y * 10 + x] = cell;
}

void remove_piece(int x, int y) {
	expected_board[y * 10 + x] = empty;
}

const CitrusPiece* randomizer(void* data) {
	(void) data;
	return &piece;
}

int main() {
	CitrusGame game;
	CitrusGameConfig config;
	CitrusGameConfig_init(&config, randomizer);
	CitrusPiece_init(&piece, piece_data, 4, 3, 3, 3, 20);
	CitrusGame_init(&game, board, config, NULL);

	clear_board();
	add_piece(3, 21);
	add_piece(4, 21);
	add_piece(5, 21);
	add_piece(4, 22);
	assert_expected();

	CitrusGame_key_down(&game, CITRUS_KEY_CLOCKWISE);
	add_piece(4, 20);
	remove_piece(3, 21);
	assert_expected();

	CitrusGame_key_down(&game, CITRUS_KEY_CLOCKWISE);
	add_piece(3, 21);
	remove_piece(4, 22);
	assert_expected();

	CitrusGame_key_down(&game, CITRUS_KEY_CLOCKWISE);
	add_piece(4, 22);
	remove_piece(5, 21);
	assert_expected();

	CitrusGame_key_down(&game, CITRUS_KEY_CLOCKWISE);
	add_piece(5, 21);
	remove_piece(4, 20);
	assert_expected();

	CitrusGame_key_down(&game, CITRUS_KEY_ANTICLOCKWISE);
	add_piece(4, 20);
	remove_piece(5, 21);
	assert_expected();

	CitrusGame_key_down(&game, CITRUS_KEY_ANTICLOCKWISE);
	add_piece(5, 21);
	remove_piece(4, 22);
	assert_expected();

	CitrusGame_key_down(&game, CITRUS_KEY_ANTICLOCKWISE);
	add_piece(4, 22);
	remove_piece(3, 21);
	assert_expected();

	CitrusGame_key_down(&game, CITRUS_KEY_ANTICLOCKWISE);
	add_piece(3, 21);
	remove_piece(4, 20);
	assert_expected();

	CitrusGame_key_down(&game, CITRUS_KEY_CLOCKWISE);
	CitrusGame_key_down(&game, CITRUS_KEY_LEFT);
	CitrusGame_key_down(&game, CITRUS_KEY_LEFT);
	CitrusGame_key_down(&game, CITRUS_KEY_LEFT);
	CitrusGame_key_down(&game, CITRUS_KEY_LEFT);
	CitrusGame_key_down(&game, CITRUS_KEY_ANTICLOCKWISE);
	clear_board();
	add_piece(0, 21);
	add_piece(0, 22);
	add_piece(0, 20);
	add_piece(1, 21);
	assert_expected();

	return 0;
}
