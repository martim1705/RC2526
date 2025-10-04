# Compilador e flags
CC = gcc                # compilador
CFLAGS = -Wall -O2      # flags de compilação
SRC = src/project.c src/read_noncanonical.c src/write_noncanonical.c src/serial.c
OBJ = $(SRC:.c=.o)      # gera lista de objetos substituindo .c -> .o
EXE = project           # nome do executável


# ——— Programa principal ———

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $(EXE)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -f $(EXE) $(OBJ)