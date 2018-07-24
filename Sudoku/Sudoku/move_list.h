#pragma once
/*
This Module defines anything related to the list of moves by the player.
*/

#define SIZE_OF_NODE (6*sizeof(int))
#define SIZE_OF_LIST (3*sizeof(void*))


/* A Node struct that will hold relavent info for a user move in the linked list
- row,column specify which cell was modified in this move.
- prev_val specifies the value that was at the cell before the turn.
- updated_val specifies the value that was at the cell after the turn.
*/
typedef struct node_t {
	struct node_t *next;
	struct node_t *prev;
	/* Variables that defines a "move" within the board: */
	unsigned int row;
	unsigned int column;
	int prev_val;
	int updated_val;
} Node;

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




/*				functions that needs to be defined in here for now:

1. redo (also updates the board with the current Node's move)
2. undo (also updates the board with the current Node's move)
3. delete and free the list in a whole.
4. delete and free the list from a certain Node pointer. (which is used in "set")
5. Add a new node to the tail of the list (or the head if its new)
*/


/* Need to document in HERE only. not in C files.*/
char *test_list_module();
void initialize_list_parameters();
int redo();
int undo();
int delete_list_full();
int delete_list_partly();

int add_new_node(int row_arg, int column_arg, int prev_val_arg, int updated_val_arg);




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