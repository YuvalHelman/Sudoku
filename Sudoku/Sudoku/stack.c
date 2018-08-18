/*
* C Program to Implement Stack Operations using Dynamic Memory
* Allocation
*/
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"


/* to insert elements in stack*/
void push(int row_index, int col_index, int value)
{
	struct node_stack *temp;
	temp = (struct node_stack*)malloc(sizeof(struct node_stack));

	temp->col_index = col_index;
	temp->row_index = row_index;
	temp->value = value;
	temp->link = top_node;
	top_node = temp;

}

/* to delete elements from stack */
void pop(int *row_index, int *col_index, int *value)
{
	struct node_stack *temp;
	if (top_node == NULL)
		printf("**Stack is empty**\n");
	else
	{
		*row_index = top_node->row_index;
		*col_index = top_node->col_index;
		*value = top_node->value;
		temp = top_node;
		top_node = top_node->link;
		free(temp);
	}
}

/* to check if stack is empty*/
int empty()
{
	if (top_node == NULL)
		return true;
	else
		return false;
}




/* to empty and destroy the stack*/
void destroy()
{
	struct node_stack *temp;
	int *row_index, *col_index, *value;
	temp = top_node;
	while (temp!= NULL)
	{
		pop(row_index, col_index, value);
		temp = temp->link;
	}
	printf("stack destroyed\n");
}
