##
# @file Makefile
# @internal Part of IFJ-project-2026-compiler, Team 274855
#
# @brief File for Make
# @author Jakub Dufek
# @date 27. 9. 2026
##


#		.PHONY
.PHONY: all pack clean test-err help

#		FLAGS
CC = gcc
CFLAGS = -Isrc -MMD -MP #-pedantic -Wall -Wextra


#		MAIN COMMANDS
all: build/compiler

clean:
	rm -rf build/

help:
	@echo ==================== HELP ====================
	@echo make all 		- Compiles entire project
	@echo make clean 	- Removes build/
	@echo make help		- Shows help feed
	@echo 
	@echo make test-err	- Tests error handling
	@echo ==================== HELP ====================

#		TEST COMMANDS
test-err: build/err_test.o build/error.o
	$(CC) $(CFLAGS) $^ -o build/err_test
	-./build/err_test


#		RULES
build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/%.o: tests/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@


#		DEPENDENCIES
build:
	mkdir -p build/

build/compiler: build/ast.o build/error.o build/expr_parser.o build/parser.o build/scanner.o build/symtable.o
	$(CC) $(CFLAGS) $^ -o $@


#		HEADER FILE UPDATE HANDLING
-include build/*.d
