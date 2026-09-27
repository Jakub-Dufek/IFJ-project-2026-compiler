##
# @file Makefile
# @internal Part of IFJ-project-2026-compiler, Team 274855
#
# @brief File for Make
# @author Jakub Dufek
# @date 27. 9. 2026
##


#	.PHONY
.PHONY: all pack clean test-err

#	FLAGS
CC = gcc
CFLAGS = -pedantic -Wall -Wextra -Isrc

all: tests/err_test.c src/error.c | build
	$(CC) $(CFLAGS) tests/err_test.c src/error.c -o build/err_test

test-err:
	-./build/err_test

build:
	mkdir -p build/