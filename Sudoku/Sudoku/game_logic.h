#include "aux_main.h"

int valid_value(sudoku *sudoku_game, int row_index, int col_index, int value);
int check_row(sudoku *sudoku_game, int row_index, int col_index, int value);
int check_col(sudoku *sudoku_game, int row_index, int col_index, int value);
int check_block(sudoku *sudoku_game, int row_index, int col_index, int value);
