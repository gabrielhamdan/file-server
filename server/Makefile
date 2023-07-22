CC=cc
FLAGS=-Wall -g
SRC=src
OBJ=obj
SRCS=$(wildcard $(SRC)/*.*)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
EXE=server

all: $(EXE)

$(EXE): $(OBJS)
	@clear
	@echo "Compiling $(EXE)."
	@$(CC) $(FLAGS) $(OBJS) -o $@
	@echo "$(EXE) was successfully compiled."

$(OBJ)/%.o: $(SRC)/%.*
	@if [ ! -d "obj" ]; then echo "Creating obj directory."; mkdir "obj"; fi
	@$(CC) $(FLAGS) -o $@ -c $<

.PHONY: clean
clean:
	@clear
	@-rm -f $(OBJ)/*.o
	@-rm -f ./$(EXE)

.PHONY: run
run:
	@clear
	@./$(EXE)