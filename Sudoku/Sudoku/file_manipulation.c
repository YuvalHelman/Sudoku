#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "file_manipulation.h"
#include "aux_main.h"


int save_to_file(FILE* fd){
	int temp_buff[TEMP_BUFF_SIZE];
	int rows, columns, res, val, i, j, curr_col, read_counter, board_len;
	
	/* Read the row, column from the file */
	res = fprintf(fd, "%d", sudoku.block_row_length);
	if (res <= 0) {
		printf("writing block row size to the file has failed. exiting\n");
		return EXIT_FAILURE;
	}
	res = fprintf(fd, "%d\n", sudoku.block_col_length);
	if (res <= 0) {
		printf("writing block column size to the file has failed. exiting\n");
		return EXIT_FAILURE;
	}

	read_counter = 1; //???
	board_len = sudoku.block_col_length * sudoku.block_row_length;
	for (i = 0; i < board_len; i++) {
		for (j = 0; j < board_len; j++) {
			if (sudoku.game_mode == edit || sudoku.board[i][j].is_fixed) { /* Saving in "edit mode" all cells are saved as fixed values */
				/* After num_of_rows numbers, print a new line */
				if (read_counter == board_len) { 
					res = fprintf(fd, "%d.\n", sudoku.board[i][j].value);
					if (res <= 0) {
						printf("writing block column size to the file has failed. exiting\n");
						return EXIT_FAILURE;
					}
					read_counter = 1;
				}
				else {
					res = fprintf(fd, "%d. ", sudoku.board[i][j].value);
					if (res <= 0) {
						printf("writing block column size to the file has failed. exiting\n");
						return EXIT_FAILURE;
					}
					read_counter++;
				}
			}
			else { /* Not fixed */
				   /* After num_of_rows numbers, print a new line */
				if (read_counter == board_len) {
					res = fprintf(fd, "%d\n", sudoku.board[i][j].value);
					if (res <= 0) {
						perror("writing block column size to the file has failed. exiting\n");
						return EXIT_FAILURE;
					}
					read_counter = 1;
				}
				else {
					res = fprintf(fd, "%d ", sudoku.board[i][j].value);
					if (res <= 0) {
						perror("writing block column size to the file has failed. exiting\n");
						return EXIT_FAILURE;
					}
					read_counter++;
				}
			}
		}

	}
	return EXIT_SUCCESS;
}


int read_from_file(FILE* fd, int* pRow, int* pCol) { // why pointer to rows/cols
	char vals[BUF_SIZE];
	int rows, columns, res, val, board_len, curr_row, curr_col, ret_code, value;
	char token;

	/* Read the row, column from the file */
	res = fscanf(fd, "%d", &rows);  
	if (res <= 0) {
		return EXIT_FAILURE;
	}
	res = fscanf(fd, "%d\n", &columns);
	if (res <= 0) {
		return EXIT_FAILURE;
	}

	if (initialize_new_board(sudoku.board, columns, rows) == EXIT_FAILURE) {
		return EXIT_FAILURE;
	}

	board_len = rows * columns;
	(*pRow) = rows;
	(*pCol) = columns;
	ret_code = BUF_SIZE;
	curr_col = 0;
	curr_row = 0;
	/* Fill the game board with the relevant info from the file */
	while (ret_code >= 0) {
		ret_code = (int)fread(vals, sizeof(char), BUF_SIZE-1, fd);
		vals[ret_code - 1] = '\0';

		token = strtok(vals, " \t\r\n");
		while (token) {
			value = (int)strtol(token, NULL, 10);

			sudoku.board[curr_row][curr_col].value = value;
			sudoku.board[curr_row][curr_col].is_fixed = 0;

			if ((strchr(token, '.')) != NULL) { /* Check for a '.' in the token */
				sudoku.board[curr_row][curr_col].is_fixed = True;
			}
			/* Iterate the current column\row values */
			if (curr_col == board_len - 1) {
				if (curr_row == board_len - 1) {
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

			token = strtok(NULL, " ");
		}
		if (feof(fd)) {
			return EXIT_SUCCESS;
		}
		else if (ferror(fd)) {
			perror("Error reading From the file");
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}
