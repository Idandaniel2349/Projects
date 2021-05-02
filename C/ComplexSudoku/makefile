all:sudoku-console
CC = gcc
OBJS = Parser.o CommandsImplementation.o DoublyLinkedList.o Errors.o exhaustiveBacktracking.o main.o MainAux.o MainAuxDefinitions.o StackImplementation.o Gurobi.o
EXEC = sudoku-console
COMP_FLAG = -ansi -O3 -Wall -Wextra -Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56


$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
main.o: main.c CommandsImplementation.h DoublyLinkedList.h Errors.h exhaustiveBacktracking.h MainAux.h MainAuxDefinitions.h StackImplementation.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
Parser.o: Parser.c Parser.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
CommandsImplementation.o: CommandsImplementation.c CommandsImplementation.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
DoublyLinkedList.o: DoublyLinkedList.c DoublyLinkedList.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
Errors.o: Errors.c Errors.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
exhaustiveBacktracking.o: exhaustiveBacktracking.c exhaustiveBacktracking.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
MainAux.o: MainAux.c MainAux.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
MainAuxDefinitions.o: MainAuxDefinitions.c MainAuxDefinitions.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
StackImplementation.o: StackImplementation.c StackImplementation.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
Gurobi.o: Gurobi.c Gurobi.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
