##
# @file Makefile
# @internal Part of IFJ-project-2026-compiler, Team 274855
#
# @brief File for Make
# @author Jakub Dufek
# @date 27. 9. 2026
##


#	.PHONY
.PHONY: all pack clean test-err help

#	FLAGS
CC = gcc
CFLAGS = -pedantic -Wall -Wextra -Isrc


#	RULES
build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: tests/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

#	DEPENDENCIES
build:
	mkdir -p build/


#	MAIN COMMANDS
all:


#	TEST COMMANDS
test-err: build/err_test.o build/error.o
	$(CC) $(CFLAGS) $^ -o build/err_test
	-./build/err_test
