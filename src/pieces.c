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

#include "citrus.h"

static const CitrusCell _ = {.type = CITRUS_CELL_EMPTY};
static const CitrusCell T = {.color = CITRUS_COLOR_T, .type = CITRUS_CELL_FULL};
static const CitrusCell S = {.color = CITRUS_COLOR_S, .type = CITRUS_CELL_FULL};
static const CitrusCell Z = {.color = CITRUS_COLOR_Z, .type = CITRUS_CELL_FULL};
static const CitrusCell J = {.color = CITRUS_COLOR_J, .type = CITRUS_CELL_FULL};
static const CitrusCell L = {.color = CITRUS_COLOR_L, .type = CITRUS_CELL_FULL};
static const CitrusCell I = {.color = CITRUS_COLOR_I, .type = CITRUS_CELL_FULL};
static const CitrusCell O = {.color = CITRUS_COLOR_O, .type = CITRUS_CELL_FULL};

// these appear upside down since the beginning of the array is the bottom of the piece

const CitrusCell citrus_t_piece_data[4 * 3 * 3] = {
	_, _, _,
	T, T, T,
	_, T, _,

	_, T, _,
	_, T, T,
	_, T, _,

	_, T, _,
	T, T, T,
	_, _, _,

	_, T, _,
	T, T, _,
	_, T, _
};

const CitrusCell citrus_s_piece_data[4 * 3 * 3] = {
	_, _, _,
	S, S, _,
	_, S, S,

	_, _, S,
	_, S, S,
	_, S, _,

	S, S, _,
	_, S, S,
	_, _, _,

	_, S, _,
	S, S, _,
	S, _, _
};

const CitrusCell citrus_z_piece_data[4 * 3 * 3] = {
	_, _, _,
	_, Z, Z,
	Z, Z, _,

	_, Z, _,
	_, Z, Z,
	_, _, Z,

	_, Z, Z,
	Z, Z, _,
	_, _, _,

	Z, _, _,
	Z, Z, _,
	_, Z, _
};

const CitrusCell citrus_l_piece_data[4 * 3 * 3] = {
	_, _, _,
	L, L, L,
	_, _, L,

	_, L, L,
	_, L, _,
	_, L, _,

	L, _, _,
	L, L, L,
	_, _, _,

	_, L, _,
	_, L, _,
	L, L, _,
};

const CitrusCell citrus_j_piece_data[4 * 3 * 3] = {
	_, _, _,
	J, J, J,
	J, _, _,

	_, J, _,
	_, J, _,
	_, J, J,

	_, _, J,
	J, J, J,
	_, _, _,

	J, J, _,
	_, J, _,
	_, J, _
};

const CitrusCell citrus_i_piece_data[4 * 4 * 4] = {
	_, _, _, _,
	_, _, _, _,
	I, I, I, I,
	_, _, _, _,

	_, _, I, _,
	_, _, I, _,
	_, _, I, _,
	_, _, I, _,

	_, _, _, _,
	I, I, I, I,
	_, _, _, _,
	_, _, _, _,

	_, I, _, _,
	_, I, _, _,
	_, I, _, _,
	_, I, _, _
};

const CitrusCell citrus_o_piece_data[1 * 2 * 2] = {O, O, O, O};

const CitrusPiece citrus_pieces[7] = {
	{citrus_i_piece_data, 4, 4, 4, -1},
	{citrus_j_piece_data, 4, 3, 3, 0},
	{citrus_l_piece_data, 4, 3, 3, 0},
	{citrus_o_piece_data, 1, 2, 2, 1},
	{citrus_s_piece_data, 4, 3, 3, 0},
	{citrus_t_piece_data, 4, 3, 3, 0},
	{citrus_z_piece_data, 4, 3, 3, 0},
};
