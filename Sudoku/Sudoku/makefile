CC = gcc
OBJS = main.o aux_main.o file_manipulation.o game_logic.o move_list.o solver.o user_interface.o stack.o
EXEC = sudoku-console
COMP_FLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

all: $(OBJS) 
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
	
$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
main.o: main.c
	$(CC) $(COMP_FLAGS) -c $*.c
aux_main.o: aux_main.c aux_main.h
	$(CC) $(COMP_FLAGS) -c $*.c
file_manipulation.o: file_manipulation.c file_manipulation.h aux_main.h
	$(CC) $(COMP_FLAGS) -c $*.c
game_logic.o: game_logic.c game_logic.h aux_main.h
	$(CC) $(COMP_FLAGS) -c $*.c
move_list.o: move_list.c move_list.h aux_main.h
	$(CC) $(COMP_FLAGS) -c $*.c
solver.o: solver.c solver.h gurobi_c.h aux_main.h game_logic.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
user_interface.o: user_interface.c user_interface.h game_logic.h move_list.h file_manipulation.h solver.h aux_main.h stack.h
	$(CC) $(COMP_FLAGS) -c $*.c
clean:
	rm -f *.o $(EXEC)
