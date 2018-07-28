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


int redo() {
	Node* curr_node;

	curr_node = move_list->current_Node_move;

	if (game_mode == init) {/* only available in edit/solve mode */
		printf("ERROR: invalid command\n");
		return EXIT_SUCCESS;
	}
	if (curr_node == move_list->tail) {
		printf("Error: no moves to redo\n");
		return EXIT_SUCCESS;
	}

	/* Set the current_node_move pointer to the next node in the list */
	move_list->current_Node_move = curr_node->next;
	curr_node = curr_node->next;

	/* Change game board cell to the updated_val. somewhere, but not in this module */
	//  TODO: game_board[curr_node->row][curr_node->column] = curr_node->updated_val;

	/* print the appropriate message to the change */
	if (curr_node->updated_val == 0) {
		if (curr_node->prev_val == 0) {
			printf("Undo %d,%d from %c to %c\n",
				curr_node->row, curr_node->column, '_', '_');
		}
		else {
			printf("Undo %d,%d from %d to %c\n",
				curr_node->row, curr_node->column, curr_node->prev_val, '_');
		}
	}
	else {
		if (curr_node->prev_val == 0) {
			printf("Undo %d,%d from %c to %d\n",
				curr_node->row, curr_node->column, '_', curr_node->updated_val);
		}
		else {
			printf("Undo %d,%d from %d to %d\n",
				curr_node->row, curr_node->column, curr_node->prev_val, curr_node->updated_val);
		}
	}

	return EXIT_SUCCESS;
}


int undo() {
	Node* curr_node;

	curr_node = move_list->current_Node_move;

	if (game_mode == init) {/* only available in edit/solve mode */
		printf("ERROR: invalid command\n");
		return EXIT_SUCCESS;
	}
	if (curr_node == move_list->head) {
		printf("Error: no moves to undo\n");
		return EXIT_SUCCESS;
	}

	/* Change game board cell to the prev_val. somewhere, but not in this module */
	// TODO: game_board[curr_node->row][curr_node->column] = curr_node->prev_val;

	/* print the appropriate message to the change */
	if (curr_node->updated_val == 0) {
		if (curr_node->prev_val == 0) {
			printf("Undo %d,%d from %c to %c\n",
				curr_node->row, curr_node->column, '_', '_');
		}
		else {
			printf("Undo %d,%d from %c to %d\n",
				curr_node->row, curr_node->column, '_', curr_node->prev_val);
		}
	}
	else {
		if (curr_node->prev_val == 0) {
			printf("Undo %d,%d from %d to %c\n",
				curr_node->row, curr_node->column, curr_node->updated_val, '_');
		}
		else {
			printf("Undo %d,%d from %d to %d\n",
				curr_node->row, curr_node->column, curr_node->updated_val, curr_node->prev_val);
		}
	}

	/* Set the node pointer to its previous node in the list */
	move_list->current_Node_move = curr_node->prev;

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
		free(curr);
		curr = next;
	}

	return EXIT_SUCCESS;
}


int delete_list_partly() {
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
		free(curr);
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
		perror("malloc failed in add_new_node() function\n");
		return EXIT_FAILURE;
	}
	node_ptr->row = row_arg;
	node_ptr->column = column_arg;
	node_ptr->prev_val = prev_val_arg;
	node_ptr->updated_val = updated_val_arg;
	node_ptr->next = NULL;
	node_ptr->prev = move_list->tail;

	move_list->tail->next = node_ptr; /* set the tail's next pointer to be the new node */
	move_list->tail = node_ptr; /* Set as new Tail of the move list */

	return EXIT_SUCCESS;
}

int initialize_list_parameters() {
	Node* head_ptr;

	move_list = malloc(SIZE_OF_LIST);
	if (move_list == NULL) {
		perror("malloc failed in initialize_list_parameters() function\n");
		return EXIT_FAILURE;
	}
	head_ptr = calloc(6, sizeof(int));
	if (head_ptr == NULL) {
		perror("calloc failed in initialize_list_parameters() function\n");
		return EXIT_FAILURE;
	}
	move_list->head = head_ptr;
	move_list->current_Node_move = head_ptr;
	move_list->tail = head_ptr;

	return EXIT_SUCCESS;
}
