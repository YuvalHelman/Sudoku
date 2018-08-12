#include <stdio.h>
#include <stdlib.h>

#include "minunit.h"
#include "move_list.h"
#include "aux_main.h"

/* Global Variables */
List *move_list = NULL;


char *test_list_module() { /* prints the message if (2nd arg) is false */
	int ret_val;
	Node* curr;

	initialize_list_parameters();
	mu_assert("1", move_list != NULL); 
	mu_assert("2", move_list->current_Node_move != NULL);
	mu_assert("3", move_list->head != NULL);
	mu_assert("4", move_list->tail != NULL);
	
	/* Testing add_new_node */
	ret_val = add_new_node(0, 0, 0, 4);
	ret_val = add_new_node(11, 100, 4, 6);
	ret_val = add_new_node(0, 0, 4, 7);
	ret_val = add_new_node(0, 0, 3, 1);

	curr = move_list->current_Node_move;
	mu_assert("5", curr->row == 0);
	mu_assert("6", curr->column == 0);
	mu_assert("7", curr->prev_val == 3);
	mu_assert("8", curr->updated_val == 1);
	mu_assert("9", curr->next == NULL);
	mu_assert("10", curr->prev != NULL);
	mu_assert("11", curr == move_list->tail);
	mu_assert("12", curr != move_list->head);

	curr = curr->prev;
	mu_assert("13", curr->row == 0);
	mu_assert("14", curr->column == 0);
	mu_assert("15", curr->prev_val == 4);
	mu_assert("16", curr->updated_val == 7);
	mu_assert("17", curr->next != NULL);
	mu_assert("18", curr->prev != NULL);
	mu_assert("19", curr != move_list->tail);
	mu_assert("20", curr != move_list->head);

	curr = curr->prev;
	mu_assert("21", curr->row == 11);
	mu_assert("22", curr->column == 100);
	mu_assert("23", curr->prev_val == 4);
	mu_assert("24", curr->updated_val == 6);
	mu_assert("25", curr->next != NULL);
	mu_assert("26", curr->prev != NULL);
	mu_assert("27", curr != move_list->tail);
	mu_assert("28", curr != move_list->head);

	curr = curr->prev;
	mu_assert("29", curr->row == 0);
	mu_assert("30", curr->column == 0);
	mu_assert("31", curr->prev_val == 0);
	mu_assert("32", curr->updated_val == 4);
	mu_assert("33", curr->next != NULL);
	mu_assert("34", curr->prev != NULL);
	mu_assert("35", curr->prev == move_list->head);
	mu_assert("36", curr != move_list->tail);
	mu_assert("37", curr != move_list->head);

	curr = curr->prev;
	mu_assert("38", curr->row == 0);
	mu_assert("39", curr->column == 0);
	mu_assert("40", curr->prev_val == 0);
	mu_assert("41", curr->updated_val == 0);
	mu_assert("42", curr->next != NULL);
	mu_assert("43", curr->prev == NULL);
	mu_assert("44", curr != move_list->tail);
	mu_assert("45", curr == move_list->head);

	/* testing delete_list_partly() */
	// TODO: continue testing the methods

	return 0;
}


Node* redo() {
	Node* curr_node;
	int i, j, board_size, prev_b_val, updated_b_val;
	cell **prev_b, **updated_b;

	curr_node = move_list->current_Node_move;
	board_size = sudoku.block_col_length * sudoku.block_row_length;

	if (game_mode == init) {/* only available in edit/solve mode */
		printf("ERROR: invalid command\n");
		return NULL;
	}
	if (curr_node == move_list->tail) {
		printf("Error: no moves to redo\n");
		return NULL;
	}

	/* Set the current_node_move pointer to the next node in the list */
	move_list->current_Node_move = curr_node->next;
	curr_node = curr_node->next;

	/* Change game board cell to the updated_val. somewhere, but not in this module */
	/* TODO: game_board[curr_node->row][curr_node->column] = curr_node->updated_val; */

	prev_b = curr_node->prev_board;
	updated_b = curr_node->updated_board;

	/* print the appropriate message to the change */
	/* print the appropriate message to the change */
	if (prev_b || updated_b) {
		for (i = 0; i < board_size; i++) {
			for (j = 0; j < board_size; j++) {
				prev_b_val = prev_b[i][j].value;
				updated_b_val = updated_b[i][j].value;

				if (prev_b_val != updated_b_val) {
					redo_print(i, j, prev_b_val, updated_b_val);
				}
			}
		}
	}
	else {
		redo_print(curr_node->row, curr_node->column, curr_node->prev_val, curr_node->updated_val);
	}

	return curr_node;
}


void redo_print(int row, int column, int prev_val, int updated_val) {
	if (updated_val == 0) {
		if (prev_val == 0) {
			printf("Redo %d,%d from %c to %c\n",
				row, column, '_', '_');
		}
		else {
			printf("Redo %d,%d from %d to %c\n",
				row, column, prev_val, '_');
		}
	}
	else {
		if (prev_val == 0) { /* updated not 0 , prev = 0 */
			printf("Redo %d,%d from %c to %d\n",
				row, column, '_', updated_val);
		}
		else {
			printf("Redo %d,%d from %d to %d\n",
				row, column, prev_val, updated_val);
		}
	}
}


Node* undo() {
	Node* curr_node;
	int i, j, board_size, prev_b_val, updated_b_val;
	cell **prev_b, **updated_b;

	curr_node = move_list->current_Node_move;
	board_size = sudoku.block_col_length * sudoku.block_row_length;

	if (game_mode == init) { /* only available in edit/solve mode */
		printf("ERROR: invalid command\n");
		return NULL;
	}
	if (curr_node == move_list->head) {
		printf("Error: no moves to undo\n");
		return NULL;
	}

	/* Change game board cell to the prev_val. somewhere, but not in this module */
	// TODO: game_board[curr_node->row][curr_node->column] = curr_node->prev_val;

	prev_b = curr_node->prev_board;
	updated_b = curr_node->updated_board;

	/* print the appropriate message to the change */
	if (prev_b || updated_b) {
		for (i = 0; i < board_size; i++) {
			for (j = 0; j < board_size; j++) {
				prev_b_val = prev_b[i][j].value;
				updated_b_val = updated_b[i][j].value;
				
				if (prev_b_val != updated_b_val) {
					undo_print(i, j, prev_b_val, updated_b_val);
				}
			}
		}
	}
	else {
		undo_print(curr_node->row, curr_node->column, curr_node->prev_val, curr_node->updated_val);
	}
	

	/* Set the node pointer to its previous node in the list */
	move_list->current_Node_move = curr_node->prev;

	return curr_node;
}

void undo_print(int row, int column, int prev_val, int updated_val) {
	if (updated_val == 0) {
		if (prev_val == 0) {
			printf("Undo %d,%d from %c to %c\n",
				row, column, '_', '_');
		}
		else {
			printf("Undo %d,%d from %c to %d\n",
				row, column, '_', prev_val);
		}
	}
	else {
		if (prev_val == 0) {
			printf("Undo %d,%d from %d to %c\n",
				row, column, updated_val, '_');
		}
		else {
			printf("Undo %d,%d from %d to %d\n",
				row, column, updated_val, prev_val);
		}
	}
}


int node_delete(Node *node) {
	if (!node) {
		return EXIT_SUCCESS;
	}

	if (node->prev_board || node->updated_board) {
		free_board(node->prev_board);
		free_board(node->updated_board);
	}
	
	free(node);

	return EXIT_SUCCESS;
}


int delete_list_full()
{
	Node *curr, *next;
	/* Check for atleast one Node in the list (other then the head) */
	if (move_list == NULL || move_list->head == NULL || move_list->head->next == NULL) {
		return EXIT_SUCCESS;
	}

	curr = move_list->head->next;
	while (curr != NULL) { /* free all of the nodes in the list */
		next = curr->next;
		node_delete(curr);
		curr = next;
	}

	return EXIT_SUCCESS;
}


int delete_list_from_the_current_node() {
	Node *curr, *next;

	/* Check for atleast one Node in the list (besides the head) */
	if (move_list == NULL || move_list->head == NULL || move_list->head->next == NULL) {
		return EXIT_SUCCESS;
	}

	/* delete all list when the current pointer is the head, which is the step before doing any set's */
	if (move_list->current_Node_move == move_list->head) {
		delete_list_full();
		return EXIT_SUCCESS;
	}

	/* free all of the nodes in the list positioned right from the current_Node_move */
	curr = move_list->current_Node_move->next;
	while (curr != NULL) { 
		next = curr->next;
		node_delete(curr);
		curr = next;
	}

	/* Change the current_Node_move to be the new tail of the list */
	move_list->current_Node_move->next = NULL;
	move_list->tail = move_list->current_Node_move;

	return EXIT_SUCCESS;
}


/* Used for "set" for adding a new move */
int add_new_node(int row_arg, int column_arg, int prev_val_arg, int updated_val_arg) {
	Node* node_ptr;

	/* Build a new node and attach to list */
	node_ptr = malloc(SIZE_OF_NODE);
	if (node_ptr == NULL) {
		perror("malloc failed in add_new_node() function");
		return EXIT_FAILURE;
	}
	node_ptr->row = row_arg;
	node_ptr->column = column_arg;
	node_ptr->prev_val = prev_val_arg;
	node_ptr->updated_val = updated_val_arg;
	node_ptr->next = NULL;
	node_ptr->prev = move_list->tail;
	node_ptr->prev_board = NULL;
	node_ptr->updated_board = NULL;

	move_list->tail->next = node_ptr; /* set the tail's next pointer to be the new node */
	move_list->tail = node_ptr; /* Set as new Tail of the move list */

	return EXIT_SUCCESS;
}


/* Used for the Autofill command */
int add_new_node_autofill(cell **prev_board, cell **updated_board) {
	Node* node_ptr;

	/* Build a new node and attach to list */
	node_ptr = malloc(SIZE_OF_NODE);
	if (node_ptr == NULL) {
		perror("malloc failed in add_new_node() in autofill function");
		return EXIT_FAILURE;
	}

	node_ptr->row = NOT_INIT;
	node_ptr->column = NOT_INIT;
	node_ptr->prev_val = NOT_INIT;
	node_ptr->updated_val = NOT_INIT;
	node_ptr->next = NULL;
	node_ptr->prev = move_list->tail;
	node_ptr->prev_board = prev_board;
	node_ptr->updated_board = updated_board;

	move_list->tail->next = node_ptr; /* set the tail's next pointer to be the new node */
	move_list->tail = node_ptr; /* Set as new Tail of the move list */

	return EXIT_SUCCESS;
}


int initialize_list_parameters() {
	Node* head_ptr;

	move_list = malloc(SIZE_OF_LIST);
	if (move_list == NULL) {
		perror("malloc failed in initialize_list_parameters() function");
		return EXIT_FAILURE;
	}
	head_ptr = calloc(6, sizeof(int));
	if (head_ptr == NULL) {
		perror("calloc failed in initialize_list_parameters() function");
		return EXIT_FAILURE;
	}
	move_list->head = head_ptr;
	move_list->current_Node_move = head_ptr;
	move_list->tail = head_ptr;

	return EXIT_SUCCESS;
}
