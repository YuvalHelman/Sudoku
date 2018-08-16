#pragma once

#define false 0
#define true 1

struct node_stack
{
	int row_index;
	int col_index;
	int value;
	struct node_stack *link;
}*top = NULL;


// function prototypes
void push(int row_index, int col_index, int value);
void pop(int *row_index, int *col_index, int *value);
int empty();
void destroy();
