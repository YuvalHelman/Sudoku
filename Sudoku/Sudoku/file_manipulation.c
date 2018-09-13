
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "file_manipulation.h"




int save_to_file(FILE* fd){
	int res, i, j, read_counter, board_len;
	
	/* Read the row, column from the file */
	res = fprintf(fd, "%d ", sudoku.block_row_length);
	if (res <= 0) {
		printf("writing block row size to the file has failed. exiting\n");
		exit(EXIT_FAILURE);
	}
	res = fprintf(fd, "%d \n", sudoku.block_col_length);
	if (res <= 0) {
		printf("writing block column size to the file has failed. exiting\n");
		exit(EXIT_FAILURE);
	}

	read_counter = 1; 
	board_len = sudoku.block_col_length * sudoku.block_row_length;
	for (i = 0; i < board_len; i++) {
		for (j = 0; j < board_len; j++) {
			/* Saving in "edit mode" all cells are saved as fixed values. (except 0's) */
			if (sudoku.board[i][j].value != 0 && (sudoku.game_mode == edit || sudoku.board[i][j].is_fixed) ) { 
				/* After num_of_rows numbers, print a new line */
				if (read_counter == board_len) { 
					res = fprintf(fd, "%d.\n", sudoku.board[i][j].value);
					if (res <= 0) {
						printf("writing to the file has failed. exiting\n");
						exit(EXIT_FAILURE);
					}
					read_counter = 1;
				}
				else {
					res = fprintf(fd, "%d. ", sudoku.board[i][j].value);
					if (res <= 0) {
						printf("writing to the file has failed. exiting\n");
						exit(EXIT_FAILURE);
					}
					read_counter++;
				}
			}
			else { /* Not fixed */
				   /* After num_of_rows numbers, print a new line */
				if (read_counter == board_len) {
					res = fprintf(fd, "%d\n", sudoku.board[i][j].value);
					if (res <= 0) {
						printf("writing to the file has failed. exiting\n");
						exit(EXIT_FAILURE);
					}
					read_counter = 1;
				}
				else {
					res = fprintf(fd, "%d ", sudoku.board[i][j].value);
					if (res <= 0) {
						printf("writing to the file has failed. exiting\n");
						exit(EXIT_FAILURE);
					}
					read_counter++;
				}
			}

		}
	}

	return EXIT_SUCCESS;
}


int read_from_file(FILE* fd, int* pRow, int* pCol, int *pNumOfCellsFilled) { 
	char vals[BUF_SIZE];
	int rows, columns, res, board_len, curr_row, curr_col, ret_code, value, num_of_filled_cells;
	char *token;

	num_of_filled_cells = 0;

	/* Read the row, column from the file */
	res = fscanf(fd, "%d", &rows);  
	if (res <= 0) {
		return EXIT_FAILURE;
	}
	res = fscanf(fd, "%d\n", &columns);
	if (res <= 0) {
		return EXIT_FAILURE;
	}

	if (initialize_new_board(columns, rows) == EXIT_FAILURE) {
		printf("Initializing a new board has failed. exiting\n");
		exit(EXIT_FAILURE);
	}

	board_len = rows * columns;
	(*pRow) = rows;
	(*pCol) = columns;
	ret_code = BUF_SIZE;
	curr_col = 0;
	curr_row = 0;
	/* Fill the game board with the relevant info from the file */
	while (ret_code >= 0) { /* Read as long as there is more input in the file. */
		ret_code = (int)fread(vals, sizeof(char), BUF_SIZE-1, fd);
		vals[ret_code - 1] = '\0';

		token = strtok(vals, " \t\r\n");
		while (token) {
			value = (int)strtol(token, NULL, 10);

			sudoku.board[curr_row][curr_col].value = value;
			
			if (value != 0) { /* Count how many numbers are filled */
				num_of_filled_cells++;
			}

			if ((strchr(token, '.')) != NULL && sudoku.game_mode != edit) { /* Check for a '.' in the token */
				sudoku.board[curr_row][curr_col].is_fixed = true;
			}
			else {
				sudoku.board[curr_row][curr_col].is_fixed = false;
			}

			/* Iterate the current column\row values */
			if (curr_col == board_len - 1) {
				if (curr_row == board_len - 1) {
					(*pNumOfCellsFilled) = num_of_filled_cells;
					return EXIT_SUCCESS;
				}
				else {
					curr_row++;
					curr_col = 0;
				}
			}
			else {
				curr_col++;
			}

			token = strtok(NULL, " \t\r\n");
		}
		if (feof(fd)) {
			(*pNumOfCellsFilled) = num_of_filled_cells;
			return EXIT_SUCCESS;
		}
		else if (ferror(fd)) {
			printf("Error reading From the file. Exiting\n");
			exit(EXIT_FAILURE);
			
		}
	}
	(*pNumOfCellsFilled) = num_of_filled_cells;

	return EXIT_SUCCESS;
}
