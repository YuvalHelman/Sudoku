

#include <stdio.h>
#include <stdlib.h>
#include "file_manipulation.h"
#include "aux_main.h"

int read_from_file(char* filepath) {
	int vals[BUF_SIZE];
	FILE* fd;
	int rows, columns, res, val, i, j, row_len, curr_row, curr_col;
	size_t ret_code;

	/* Open the file*/
	fd = fopen(filepath, "r");
	if (!fd) {
		perror("File opening failed");
		return EXIT_FAILURE;
	}

	/* Read the row, column from the file */
	res = fscanf(fd, "%d", &rows);
	if (res != 1) {
		return EXIT_FAILURE;
	}
	res = fscanf(fd, "%d", &columns);
	if (res != 1) {
		return EXIT_FAILURE;
	}

	row_len = rows * columns;
	ret_code = BUF_SIZE;
	curr_col = 0;
	curr_row = 0;
	/* Fill the game board with the relevant info from the file */
	while (ret_code == BUF_SIZE) {
		ret_code = fread(vals, sizeof(int), BUF_SIZE, fd);
		for (i = 0; i<ret_code; i++) {
			sudoku.board[curr_row][curr_col].value = vals[i];

			if (curr_col == row_len) {
				if (curr_row == row_len) {
					perror("File is bigger then the current board");
					return EXIT_FAILURE;
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










}