#include <stdio.h>
#include <stdlib.h>

#include "move_list.h"
#include "mainAux.h"

List *move_list = NULL;

char *test_list_module() {

	mu_assert("message", move_list == NULL); /* prints the message if move_list != NULL */
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

	/* Change game board cell to the updated_val*/
	game_board[curr_node->row][curr_node->column] = curr_node->updated_val;

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

	/* Change game board cell to the prev_val*/
	game_board[curr_node->row][curr_node->column] = curr_node->prev_val;

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
	/* Check for atleast one Node in the list */
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

/* Will be used for doing "set" after redo's. */
int delete_list_partly() {
	Node *curr, *next;
	/* Check for atleast one Node in the list (besides the head) */
	if (move_list == NULL || move_list->head == NULL || move_list->head->next == NULL) {
		return EXIT_SUCCESS;
	}
	/* delete all list when the */
	if (move_list->current_Node_move == move_list->head) {
		delete_list_full();
		return EXIT_SUCCESS;
	}

	curr = move_list->current_Node_move->next;
	while (curr != NULL) { /* free all of the nodes in the list */
		next = curr->next;
		free(curr);
		curr = next;
	}
	return EXIT_SUCCESS;
}


/* Used for "set" for adding a new move */
int add_new_node(int row_arg, int column_arg, int prev_val_arg, int updated_val_arg) {
	Node* node_ptr;

	/* Build a new node and attach to list */
	node_ptr = malloc(SIZE_OF_NODE);
	node_ptr->row = row_arg;
	node_ptr->column = column_arg;
	node_ptr->prev_val = prev_val_arg;
	node_ptr->updated_val = updated_val_arg;
	node_ptr->next = NULL;
	node_ptr->prev = move_list->tail;

	move_list->tail->next = node_ptr; /* set the tail's next pointer to be the new node */
	move_list->tail = node_ptr; /* Set as new Tail of the move list */

}

void initialize_list_parameters() {
	Node* head_ptr;

	move_list = malloc(SIZE_OF_LIST);
	head_ptr = calloc(6, sizeof(int));
	move_list->head = head_ptr;
	move_list->current_Node_move = head_ptr;
	move_list->tail = head_ptr;
}

