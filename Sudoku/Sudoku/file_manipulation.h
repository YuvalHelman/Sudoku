
#ifndef FILE_HEADER
#define FILE_HEADER

#include "aux_main.h"


/*
* This Module is responsible for anything related to files within the program.
*/


#define BUF_SIZE 2048
#define TEMP_BUFF_SIZE 2400


/*
 *	reads from the file that is given as an argument and initializes the board and sets the
 *	pointer argument for the sudoku.block_col_length / sudoku.block_row_length values.
 *
 *
 *  @fd: a file struct to read from
 *	@pRow: a pointer to a number to return for the sudoku.block_row_length
 *	@pCol: a pointer to a number to return for the sudoku.block_col_length
 *
 *   @return: the FILE* struct for this file on Success, or NULL on Failure.
 */
int read_from_file(FILE* fd, int* pRow, int* pCol, int *pNumOfCellsFilled);


/*
 *	The functions initiates a loading of the file given by its argument, and interprets
 *	the values in the file to load a new gaming board. 
 *
 *	
 *	@fd: a file descriptor which has been already opened by the "open_file" function
 *
 *   @returns: EXIT_SUCCESS(0) on adding a new node.
 */
int save_to_file(FILE* fd);


#endif /* FILE_HEADER */