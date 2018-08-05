#include <stdio.h>
#include <stdlib.h>
#include "file_manipulation.h"
#include "aux_main.h"

int save_to_file(char* filepath){
	int temp_buff[TEMP_BUFF_SIZE];
	FILE* fd;
	int rows, columns, res, val, i, j, curr_col, read_counter, num_of_rows;
	
	/* Open the file*/
	fd = fopen(filepath, "w");
	if (!fd) {
		printf("Error: File cannot be opened\n");
		return EXIT_FAILURE;
	}
	/* Read the row, column from the file */
	res = fprintf(fd, "%d", sudoku.block_row_length);
	if (res <= 0) {
		perror("writing block row size to the file has failed. exiting\n");
		return EXIT_FAILURE;
	}
	res = fprintf(fd, "%d\n", sudoku.block_col_length);
	if (res <= 0) {
		perror("writing block column size to the file has failed. exiting\n");
		return EXIT_FAILURE;
	}

	read_counter = 1;
	num_of_rows = sudoku.block_col_length * sudoku.block_row_length;
	for (i = 0; i < num_of_rows; i++) {
		for (j = 0; j < num_of_rows; j++) {
			if (game_mode == edit || sudoku.board[i][j].fixed) { /* Saving in "edit mode" all cells are saved as fixed values */
				/* After num_of_rows numbers, print a new line */
				if (read_counter == num_of_rows) { 
					res = fprintf(fd, "%d.\n", sudoku.board[i][j].value);
					if (res <= 0) {
						perror("writing block column size to the file has failed. exiting\n");
						return EXIT_FAILURE;
					}
					read_counter = 1;
				}
				else {
					res = fprintf(fd, "%d. ", sudoku.board[i][j].value);
					if (res <= 0) {
						perror("writing block column size to the file has failed. exiting\n");
						return EXIT_FAILURE;
					}
					read_counter++;
				}
			}
			else { /* Not fixed */
				   /* After num_of_rows numbers, print a new line */
				if (read_counter == num_of_rows) {
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


int read_from_file(char* filepath) {
	int vals[BUF_SIZE];
	FILE* fd;
	int rows, columns, res, val, i, j, board_len, curr_row, curr_col, ret_code;
	char posibble_fixed;

	/* Open the file*/
	fd = fopen(filepath, "r");
	if (!fd) {
		printf("Error: File cannot be opened\n");
		return EXIT_FAILURE;
	}

	/* Read the row, column from the file */
	res = fscanf(fd, "%d", &rows);
	if (res <= 0) {
		return EXIT_FAILURE;
	}
	res = fscanf(fd, "%d\n", &columns);
	if (res <= 0) {
		return EXIT_FAILURE;
	}

	board_len = rows * columns;
	ret_code = BUF_SIZE;
	curr_col = 0;
	curr_row = 0;
	/* Fill the game board with the relevant info from the file */
	while (ret_code >= 0) {
		ret_code = (int)fread(vals, sizeof(int), BUF_SIZE, fd);
		if (feof(fd)) {
			return EXIT_SUCCESS;
		}
		else if (ferror(fd)) {
			perror("Error reading From the file");
			return EXIT_FAILURE;
		}
		for (i = 0; i < ret_code; i++) {
			sudoku.board[curr_row][curr_col].value = vals[i];
			/* Iterate i,j by counting the number of numbers read */
			if (curr_col == board_len-1) {
				if (curr_row == board_len-1) {
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
		}
	}
	return EXIT_SUCCESS;
}