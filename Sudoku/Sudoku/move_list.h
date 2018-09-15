/*

		This module implements the sudoku's double-linked List and any functions regarding manipulating data on the list.
		Adding new nodes, removing nodes, removing all of the list, traversing the list etc.
*/

#ifndef NOVE_LIST_HEADER
#define NOVE_LIST_HEADER

#include "aux_main.h"

#define SIZE_OF_NODE (4*sizeof(int*))
#define SIZE_OF_NODE_VALS (4*sizeof(int))
#define NODE_NUM_OF_PTRS 8
#define SIZE_OF_LIST (3*sizeof(void*))
#define NOT_INIT -1

/* the struct defines a single cell change in the board for undo\redo list */
typedef struct node_values_t {
	unsigned int row;
	unsigned int column;
	int prev_val; /* specifies the value that was at the cell before the turn. */
	int updated_val; /* updated_val specifies the value that was at the cell after the turn. */
} node_vals;

/* A Node struct that holds relavent info for a user move in the linked list */
typedef struct node_t {
	struct node_t *next;
	struct node_t *prev;
	node_vals *values;
	unsigned int num_of_values;
} Node;


/*	A linked list of user moves within the game.
*	The list implements a remembering idea of the user's last moves that were not yet deleted traversing the list and changing modes.
*/
typedef struct list_t {
	struct node_t* head;
	struct node_t* tail;
	struct node_t* current_Node_move; /* Will mainly be equal to the tail, unless redo/undo is being operated */
} List;


/* Global Variables: */
extern List *move_list;


/*
 *  This function is called first thing in "main" in order to initialize
 *	the objects that are needed to start working with the move_list module.
 *	It should be called only once before playing.
 *
 *   returns: EXIT_SUCCESS(0) on success.
 *			  on any error returns EXIT_FAILURE(1) and prints the error.
 */
int initialize_list_parameters();

/*
 *	 This function goes forward one move from the current move and changes the list position and board values accordingly.
 *   If there arent any futuristic moves, the function does nothing.
 *
 *	 num_of_values: a pointer to an integer which will be returned as the number of values in the board that changed due to the function.
 *
 *   returns: a pointer to a node_vals array with all of the values that were changed.
 *			  on any error returns NULL and prints the error.
 */
node_vals* redo_list(int* num_of_values);

/*
*	This function goes back one move from the current move and changes the list position and board values accordingly.
*	If there arent any previous moves, the function does nothing.
*	On the "reset" function, the function avoids printing "no more moves to undo" when we reach the head of the list.
*
*	num_of_values: a pointer to an integer which will be returned as the number of values in the board that changed due to the function.
*	reset_flag: a flag that indicates if the function is done in the "reset" function.
*
*   returns: a pointer to a node_vals array with all of the values that were changed.
*			  on any error returns NULL and prints the error.
*/
node_vals* undo_list(int* num_of_values, int reset_flag);

/*
 *  This function is called to free all of the list's dynamicaly allocated resources before exiting.
 *
 *	Free'ing the list and the head. 
 *	The list isn't useable after calling this function, and therefore use it only when game is over and terminating.
 *
 */
void delete_list_on_exit();

/*
 *   This function is used for deleting the entire list (back to the initialized phase).
 *   should be used for "edit" \ "solve" \ "generate" \ "reset" commands.
 *
 *   returns: EXIT_SUCCESS(0) on success.
 *			  on any error returns EXIT_FAILURE(1) and prints the error.
 */
int delete_list_full();

/*
 *   This function is used for deleting the part of the list which is positioned
 * 	 right from the current Node (the current move which is pointed when the function is called).
 *   should be used for adding new nodes when done after undo's/redo's when the current_node != tail_node .
 *
 *   This function sets the current Node to be the new tail.
 *
 *   returns: EXIT_SUCCESS(0) on success.
 *			  on any error returns EXIT_FAILURE(1) and prints the error.
 */
int delete_list_from_the_current_node();

/*
 *   adds a new Node to the tail of the move_list. should be used in the "set" command.
 *   Each new node should be added with the location and current+updated cell values that
 *	 the user uses.
 *	 If the current_Node_move isn't the tail, all of the list from it's right is erased, and the new 
 *	 Node added is the new tail which is connected after the current_Node_move.
 *
 *   row_arg: the row in which the change was made
 *   column_arg: the column in which the change was made
 *   prev_val_arg: the value of the cell before the change. (0 if empty)
 *   updated_val_arg: the new value that is being put into that cell.
 *
 *   returns: EXIT_SUCCESS(0) on adding a new node.
			  on any error returns EXIT_FAILURE(1) and prints the error.
 */
int add_new_node(int row_arg, int column_arg, int prev_val_arg, int updated_val_arg);

/*
*   adds a new value to the move_list->current_node->values array.
*	should be used in the "autofill"/"generate" command.
*
*   row_arg: the row in which the change was made
*   column_arg: the column in which the change was made
*   prev_val_arg: the value of the cell before the change. (0 if empty)
*   updated_val_arg: the new value that is being put into that cell.
*
*   returns: EXIT_SUCCESS(0) on success.
			 on any error returns EXIT_FAILURE(1) and prints the error.
*/
int add_val_to_current_node(int row_arg, int column_arg, int prev_val_arg, int updated_val_arg);




#endif /* MOVE_LIST_HEADER */