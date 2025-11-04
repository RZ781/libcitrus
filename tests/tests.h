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

#ifndef TESTS_H
#define TESTS_H

#include "citrus.h"

extern CitrusCell board[10 * 40];
extern CitrusCell expected_board[10 * 40];
extern const CitrusPiece* next_piece_queue[3];

void clear_board(void);
void set_piece(int x, int y, CitrusCellType type, CitrusColor color);
void assert_expected(void);

const CitrusPiece* single_piece_randomizer(void* data);
void hard_drop_test();
void movement_test();
void rotation_test();

#endif
