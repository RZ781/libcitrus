# Copyright (C) 2026 RZ781
#
# This file is part of libcitrus.
#
# libcitrus is free software: you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published
# by the Free Software Foundation, either version 2.1 of the License, or
# (at your option) any later version.
#
# libcitrus is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this program.  If not, see
# <https://www.gnu.org/licenses/>.

CFLAGS = -Wall -Wextra -Wpedantic -Iinclude -fPIC
SOURCE := $(wildcard src/*.c)
OBJECT := $(SOURCE:.c=.o)
TEST_SOURCE := $(wildcard tests/*.c)
TEST_OBJECT := $(TEST_SOURCE:.c=.o)
INCLUDE := $(wildcard include/*.h)
TEST_INCLUDE := $(wildcard tests/*.h)

all: libcitrus.a libcitrus.so

src/%.o: src/%.c $(INCLUDE)
	gcc $(CFLAGS) -c $< -o $@

tests/%.o: tests/%.c $(INCLUDE) $(TEST_INCLUDE)
	gcc $(CFLAGS) -g -Itests -c $< -o $@

libcitrus.a: $(OBJECT)
	ar rcs libcitrus.a $(OBJECT)

libcitrus.so: $(OBJECT)
	gcc -shared -nostdlib $(OBJECT) -o libcitrus.so

test: libcitrus.so $(TEST_OBJECT)
	gcc $(TEST_OBJECT) -Wl,-rpath='$${ORIGIN}' -L. -lcitrus -o test

format: $(SOURCE) $(INCLUDE) $(TEST_SOURCE) $(TEST_INCLUDE)
	VERSION_CONTROL=none indent -linux $(SOURCE) $(INCLUDE) $(TEST_SOURCE) $(TEST_INCLUDE)

verify_no_libc: libcitrus.so
	undefined_symbols="$$(nm -u libcitrus.so | grep -v __stack_chk)"; \
	if [ -n "$$undefined_symbols" ] ; then \
		echo "undefined symbols:"; \
		echo "$$undefined_symbols"; \
	fi

run_tests: test
	./test

check: format all verify_no_libc run_tests
