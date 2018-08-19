
#ifndef NOVE_LIST_HEADER
#define NOVE_LIST_HEADER

#include "aux_main.h"

/*
	This Module defines anything related to the list of moves used by the player.
*/

#define SIZE_OF_NODE (4*sizeof(int*))
#define SIZE_OF_NODE_VALS (4*sizeof(int))
#define NODE_NUM_OF_PTRS 8
#define SIZE_OF_LIST (3*sizeof(void*))
#define NOT_INIT -1

#ifndef NODE_STRUCT
#define NODE_STRUCT


/* Variables that defines a "move" within the board: */
typedef struct node_values_t {
	unsigned int row;
	unsigned int column;
	int prev_val;
	int updated_val;
} node_vals;
/* A Node struct that will hold relavent info for a user move in the linked list
- row,column specify which cell was modified in this move.
- prev_val specifies the value that was at the cell before the turn.
- updated_val specifies the value that was at the cell after the turn.
*/
typedef struct node_t {
	struct node_t *next;
	struct node_t *prev;
	struct node_vals_t *values;
	unsigned int num_of_values;
} Node;


#endif


/* A linked list of user moves within the game.
The list implements a remembering idea of the user's last moves that were not yet deleted by him.
*/
typedef struct list_t {
	struct node_t* head;
	struct node_t* tail;
	struct node_t* current_Node_move; /* Will mainly be equal to the tail, unless redo/undo is being operated */
} List;


/* Global Variables related to the module: */
extern List *move_list;



/* 
 * a main function that is called for testing all of the other functions in this module 
 */
char *test_list_module();
/*
 *  This function is called first thing in "main" in order to initialize
 *	the objects that are needed to start working with the move_list module.
 *	It should be called only once before playing.
 *
 *   returns: EXIT_SUCCESS(0) on adding a new node.
 *			  on any error returns EXIT_FAILURE(1) and prints the error.
 */
int initialize_list_parameters();
/*
 *   This function goes back one move from the current move and changes the list position and board.
 *    If there arent any previous moves, the function does nothing and returns SUCCESS.
 *
 *   returns: EXIT_SUCCESS(0) on adding a new node.
 *			  on any error returns EXIT_FAILURE(1) and prints the error.
 */
node_vals* redo_list(int* num_of_values);

/*
 *   This function goes forward one move from the current move and changes the list position and board.
 *   If there isnt a move forward, the function does nothing and returns SUCCESS.
 *
 *   returns: EXIT_SUCCESS(0) on adding a new node.
 *			  on any error returns EXIT_FAILURE(1) and prints the error.
 */
node_vals* undo_list(int* num_of_values);

/*
 *   This function is used for deleting a node while free'ing all its content.
 *   should be used in any function that deletes a part of the list.
 *
 *   returns: EXIT_SUCCESS(0) on adding a new node.
 *			 on any error returns EXIT_FAILURE(1) and prints the error.
 */
int node_delete(Node *node);
/*
 *   This function is called to free leftover stuff when exiting cleanly.
 *	Free'ing the list as a whole and the head.
 *
 */
void delete_list_on_exit();
/*
 *   This function is used for deleting the entire list (back to the initialized phase).
 *   should be used for the "RESTART", "EXIT" or any other program terminating step.
 *
 *   returns: EXIT_SUCCESS(0) on adding a new node.
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
 *   returns: EXIT_SUCCESS(0) on adding a new node.
 *			  on any error returns EXIT_FAILURE(1) and prints the error.
 */
int delete_list_from_the_current_node();
/*
 *   adds a new Node to the tail of the move_list. should be used in the "set" command.
 *   Each new node should be added with the location and current+updated cell values that
 *	 the user uses.
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
*   returns: EXIT_SUCCESS(0) on adding a new node.
on any error returns EXIT_FAILURE(1) and prints the error.
*/
int add_val_to_current_node(int row_arg, int column_arg, int prev_val_arg, int updated_val_arg);


/* List explained for "SET" implemantation: */
/*    HEAD --> 1st move --> 2nd move --> ........
The head is meant to signal the "first view" of the sudoku before any moves were made.
I could've said that it is empty in the start... but this way it seems easier to implement the
methods. "curr_node == HEAD" means no more UNDO's are possible

the node is implemented in a way that lets us traverse the list and change values accordingly
so we can also change the game_board each time.
a Node contains a cell (row,column) where some Value was changed to a different value. (the
value 0 would indicate the cell is empty)

NODE =   { next_node_ptr, prev_node_ptr, (row,column), previous_value_in_cell, new_value_in_cell }
when the move_list->current_Node_move points to a certain node, the "new_value_in_cell" is the one that is
currently updated in the game_board.

*/

#endif /* MOVE_LIST_HEADER */