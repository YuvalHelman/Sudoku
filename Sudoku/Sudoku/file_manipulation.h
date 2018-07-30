
/*
* This Module is responsible for anything related to files within the program.
*/


#define BUF_SIZE 50
#define TEMP_BUFF_SIZE 2400


/*
*
*
*
*
*   row_arg: the row in which the change was made
*   column_arg: the column in which the change was made
*   prev_val_arg: the value of the cell before the change. (0 if empty)
*   updated_val_arg: the new value that is being put into that cell.
*
*   returns: EXIT_SUCCESS(0) on adding a new node.
*/
int read_from_file(char* filepath);
/*
*
*
*
*
*   row_arg: the row in which the change was made
*   column_arg: the column in which the change was made
*   prev_val_arg: the value of the cell before the change. (0 if empty)
*   updated_val_arg: the new value that is being put into that cell.
*
*   returns: EXIT_SUCCESS(0) on adding a new node.
*/
int save_to_file(char* filepath);
