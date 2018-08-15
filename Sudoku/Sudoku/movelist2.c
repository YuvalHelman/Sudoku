#include <stdio.h>
#include <stdlib.h>

#include "minunit.h"
#include "movelist2.h"
#include "aux_main.h"

/* Global Variables */
List *move_list = NULL;
int move_number;

char *test_list_module() { /* prints the message if (2nd arg) is false */
	int ret_val;
	Node* curr;

	curr = NULL;

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

// TODO: use set in order to update the errors. use the fanc updateerrors
Node* redo_list() {
	Node* curr_node;
	int prev_b_val, updated_b_val;

	curr_node = move_list->current_Node_move;

	if (sudoku.game_mode == init) {/* only available in edit/solve mode */
		printf("ERROR: invalid command\n");
		return NULL;
	}
	if (curr_node == move_list->tail) {
		printf("Error: no moves to redo\n");
		return NULL;
	}

	while (curr_node != move_list->tail && move_number == curr_node->movenumber) {
		redo_print(curr_node->row, curr_node->column, curr_node->prev_val, curr_node->updated_val);
		curr_node = curr_node->next;
	}

	move_number++;

	/* Set the current_node_move pointer to the next node in the list */
	move_list->current_Node_move = curr_node;

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


Node* undo_list() {
	Node* curr_node;
	int i, j, board_size, prev_b_val, updated_b_val;

	curr_node = move_list->current_Node_move;
	board_size = sudoku.block_col_length * sudoku.block_row_length;

	if (sudoku.game_mode == init) { /* only available in edit/solve mode */
		printf("ERROR: invalid command\n");
		return NULL;
	}
	if (curr_node == move_list->head) {
		printf("Error: no moves to undo\n");
		return NULL;
	}

	while (curr_node != move_list->head && move_number == curr_node->movenumber) {
		undo_print(curr_node->row, curr_node->column, curr_node->prev_val, curr_node->updated_val);
		curr_node = curr_node->prev;
	}

	move_number--;

	/* Set the node pointer to its previous node in the list */
	move_list->current_Node_move = curr_node;

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

	move_list->head->next = NULL;

	return EXIT_SUCCESS;
}


int delete_list_from_the_current_node() {
	Node *curr, *next;

	/* Check for atleast one Node in the list (besides the head) */
	if (move_list == NULL || move_list->head == NULL || move_list->head->next == NULL) {
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
	node_ptr->movenumber = move_number;
	node_ptr->next = NULL;
	node_ptr->prev = move_list->tail;

	/* Delete a part of the list if the current_node isn't the tail */
	if (move_list->current_Node_move != move_list->tail) {
		delete_list_from_the_current_node();
	}

	move_list->tail->next = node_ptr; /* set the tail's next pointer to be the new node */
	move_list->tail = node_ptr; /* Set as new Tail of the move list */
	move_list->current_Node_move = move_list->tail; /* set the current pointer to be the new tail */

	return EXIT_SUCCESS;
}




int initialize_list_parameters() {
	Node* head_ptr;
	move_number = 0;
	move_list = malloc(SIZE_OF_LIST);
	if (move_list == NULL) {
		perror("malloc failed in initialize_list_parameters() function");
		return EXIT_FAILURE;
	}
	head_ptr = calloc(NODE_NUM_OF_PTRS, sizeof(int));
	if (head_ptr == NULL) {
		perror("calloc failed in initialize_list_parameters() function");
		return EXIT_FAILURE;
	}
	move_list->head = head_ptr;
	move_list->current_Node_move = head_ptr;
	move_list->tail = head_ptr;

	return EXIT_SUCCESS;
}

void delete_list_on_exit() {
	delete_list_full();
	free(move_list->head);
	free(move_list);
}