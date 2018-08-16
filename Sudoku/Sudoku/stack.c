/*
* C Program to Implement Stack Operations using Dynamic Memory
* Allocation
*/
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"


// to insert elements in stack
void push(int row_index, int col_index, int value)
{
	struct node_stack *temp;
	temp = (struct node_stack*)malloc(sizeof(struct node_stack));

	temp->col_index = col_index;
	temp->row_index = row_index;
	temp->value = value;
	temp->link = top;
	top = temp;

}

// to delete elements from stack
void pop(int *row_index, int *col_index, int *value)
{
	struct node_stack *temp;
	if (top == NULL)
		printf("**Stack is empty**\n");
	else
	{
		*row_index = top->row_index;
		*col_index = top->col_index;
		*value = top->value;
		temp = top;
		top = top->link;
		free(temp);
	}
}

// to check if stack is empty
int empty()
{
	if (top == NULL)
		return true;
	else
		return false;
}




// to empty and destroy the stack
void destroy()
{
	struct node_stack *temp;
	int *row_index, *col_index, *value;
	temp = top;
	while (temp!= NULL)
	{
		pop(row_index, col_index, value);
		temp = temp->link;
	}
	printf("stack destroyed\n");
}
