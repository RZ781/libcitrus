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
static const CitrusCell T = {.colour = CITRUS_COLOUR_T, .type = CITRUS_CELL_FULL};
static const CitrusCell S = {.colour = CITRUS_COLOUR_S, .type = CITRUS_CELL_FULL};
static const CitrusCell Z = {.colour = CITRUS_COLOUR_Z, .type = CITRUS_CELL_FULL};
static const CitrusCell J = {.colour = CITRUS_COLOUR_J, .type = CITRUS_CELL_FULL};
static const CitrusCell L = {.colour = CITRUS_COLOUR_L, .type = CITRUS_CELL_FULL};
static const CitrusCell I = {.colour = CITRUS_COLOUR_I, .type = CITRUS_CELL_FULL};
static const CitrusCell O = {.colour = CITRUS_COLOUR_O, .type = CITRUS_CELL_FULL};

// these appear upside down since the beginning of the array is the bottom of the piece

const CitrusCell t_piece_data[4 * 3 * 3] = {
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

const CitrusCell s_piece_data[4 * 3 * 3] = {
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

const CitrusCell z_piece_data[4 * 3 * 3] = {
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

const CitrusCell l_piece_data[4 * 3 * 3] = {
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

const CitrusCell j_piece_data[4 * 3 * 3] = {
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

const CitrusCell i_piece_data[4 * 4 * 4] = {
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

const CitrusCell o_piece_data[1 * 2 * 2] = {O, O, O, O};

const CitrusPiece pieces[7] = {
	{i_piece_data, 4, 4, 4, 3, 19},
	{j_piece_data, 4, 3, 3, 3, 20},
	{l_piece_data, 4, 3, 3, 3, 20},
	{o_piece_data, 1, 2, 2, 4, 21},
	{s_piece_data, 4, 3, 3, 3, 20},
	{t_piece_data, 4, 3, 3, 3, 20},
	{z_piece_data, 4, 3, 3, 3, 20},
};
