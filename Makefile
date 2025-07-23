CC = gcc
CXX = g++

CFLAGS = -Wall -Werror -Wextra -Wpedantic -Wshadow -std=c11
CXXFLAGS = -Wall -Werror -Wextra -Wpedantic -Wshadow -std=c++20
TESTFLAGS = -lgtest

SHELLNAME:=$(shell uname)
ifeq ($(SHELLNAME), Linux)
	LDFLAGS = -lcheck -lsubunit -lm
else ifeq ($(SHELLNAME), Darwin)
	LDFLAGS = -lcheck
endif

BIN = brick_game
OBJ_DIR = obj
LOGS_DIR = ./logs
LIB_DIR = ./lib
BUILD_DIR = ./build
RECORDS_DIR = ./records

MAIN = $(BIN).cc
MAIN_DIR_OBJ = $(OBJ_DIR)/$(BIN).o

CLI_DIR = ./gui/cli
CLI_DIR_OBJ = $(OBJ_DIR)/cli

TETRIS_DIR = ./brick_game/tetris
TETRIS_DIR_OBJ = $(OBJ_DIR)/tetris
TETRIS_TEST = ./tests/tetris_tests.c

SNAKE_DIR = ./brick_game/snake
SNAKE_DIR_OBJ = $(OBJ_DIR)/snake
SNAKE_TEST = ./tests/snake_tests.cc

TETRIS_LIB = $(LIB_DIR)/tetris_backend.a
SNAKE_LIB = $(LIB_DIR)/snake_backend.a

TETRIS_SRC = $(wildcard $(TETRIS_DIR)/*.c)
TETRIS_OBJ = $(patsubst $(TETRIS_DIR)/%.c, $(TETRIS_DIR_OBJ)/%.o, $(TETRIS_SRC))

SNAKE_SRC = $(wildcard $(SNAKE_DIR)/*.cc)
SNAKE_OBJ = $(patsubst $(SNAKE_DIR)/%.cc, $(SNAKE_DIR_OBJ)/%.o, $(SNAKE_SRC))

CLI_SRC = $(wildcard $(CLI_DIR)/*.c)
CLI_OBJ = $(patsubst $(CLI_DIR)/%.c, $(CLI_DIR_OBJ)/%.o, $(CLI_SRC))

DIST_DIR = dist
DIST_NAME = brick_game_v2.0.tar.gz
DIST_FILES = brick_game gui headers tests Makefile CMakeLists.txt Doxyfile *.cc

all: uninstall install

install: uninstall
		@mkdir -p $(BUILD_DIR)
		@mkdir -p $(RECORDS_DIR)
		@cd $(BUILD_DIR) && cmake ..
		@$(MAKE) -C build

uninstall: clean
		@rm -rf $(BUILD_DIR) $(RECORDS_DIR)

test: clean tetris_test snake_test
		@./tetris_tests
		@./snake_tests

snake_test: $(SNAKE_LIB)
		@mkdir -p $(RECORDS_DIR)
		@rm -rf $(RECORDS_DIR)/.snake_record.txt
		@$(CXX) $(CXXFLAGS) $(SNAKE_TEST) $(SNAKE_LIB) -o snake_tests $(TESTFLAGS)

tetris_test: $(TETRIS_LIB)
		@mkdir -p $(RECORDS_DIR)
		@rm -rf $(RECORDS_DIR)/.tetris_record.txt
		@$(CC) $(CFLAGS) $(TETRIS_TEST) $(TETRIS_LIB) -o tetris_tests $(LDFLAGS)
		

gcov_report: CXXFLAGS += --coverage
gcov_report: CFLAGS += --coverage
gcov_report: test
		@mkdir -p $(LOGS_DIR)
		@echo "Generating coverage report via gcovr..."
		@gcovr -r . \
		--exclude-unreachable-branches \
		--exclude 'tests/' \
		--html-details \
		--html-theme green \
		-o $(LOGS_DIR)/coverage_report.html
		@echo "Report generated: coverage_report.html"
		@open $(LOGS_DIR)/coverage_report.html

dvi: clean
		@doxygen && open doxygen/html/index.html

dist: clean
		@echo "Creating archive..."
		@mkdir -p $(DIST_DIR)
		@tar -czf $(DIST_DIR)/$(DIST_NAME) $(DIST_FILES)
		@echo "Archive created!"

play:	
		mkdir -p records
		@$(BUILD_DIR)/$(BIN)

valgrind: clean tetris_test snake_test
		@clear
		@echo "Valgrind checking..."
		@rm -rf $(RECORDS_DIR)/.tetris_record.txt
		@rm -rf $(RECORDS_DIR)/.snake_record.txt
		@valgrind --tool=memcheck --leak-check=yes ./tetris_tests > tetris_valgrind.txt 2>&1
		@valgrind --tool=memcheck --leak-check=yes ./snake_tests > snake_valgrind.txt 2>&1
		@echo "Valgrind checking completed! Output files: \ntetris_valgrind.txt \nsnake_valgrind.txt"

leaks: clean tetris_test snake_test
		@clear
		@echo "Leaks checking..."
		@rm -rf $(RECORDS_DIR)/.tetris_record.txt
		@rm -rf $(RECORDS_DIR)/.snake_record.txt
		@leaks -atExit -- ./tetris_tests > tetris_leaks.txt 2>&1
		@leaks -atExit -- ./snake_tests > snake_leaks.txt 2>&1
		@echo "Leaks checking completed! Output files: \ntetris_leaks.txt \nsnake_leaks.txt"

clang:
		@echo "Checking clang-format..."
		@cp ../materials/linters/.clang-format .
		@find . -type f -name "*.cc" -exec clang-format -n {} \;
		@find . -type f -name "*.c" -exec clang-format -n {} \;
		@rm -rf .clang-format
		@echo "Checking completed!"

clean:	
		@rm -rf $(OBJ_DIR) $(LOGS_DIR) $(LIB_DIR) $(DIST_DIR) .clang-format *.gcda *.gcno snake_tests tetris_tests doxygen
		@rm -rf tetris_valgrind.txt snake_valgrind.txt tetris_leaks.txt snake_leaks.txt

$(CLI_DIR_OBJ) $(TETRIS_DIR_OBJ) $(SNAKE_DIR_OBJ):
		@mkdir -p $(OBJ_DIR)
		@mkdir -p $@

$(TETRIS_LIB) : $(TETRIS_OBJ)
		@mkdir -p $(LIB_DIR)
		@ar rcs $@ $^

$(SNAKE_LIB): $(SNAKE_OBJ)
		@mkdir -p $(LIB_DIR)
		@ar rcs $@ $^


$(TETRIS_DIR_OBJ)/%.o: $(TETRIS_DIR)/%.c | $(TETRIS_DIR_OBJ)

		@$(CC) $(CFLAGS) -c $< -o $@

$(SNAKE_DIR_OBJ)/%.o: $(SNAKE_DIR)/%.cc | $(SNAKE_DIR_OBJ)
		@$(CXX) $(CXXFLAGS) -c $< -o $@

$(CLI_DIR_OBJ)/%.o: $(CLI_DIR)/%.c | $(CLI_DIR_OBJ)
		@$(CC) $(CFLAGS) -c $< -o $@
