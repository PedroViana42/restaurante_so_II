CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -pthread
OBJ = main.o fila_clientes.o fila_pedidos.o gerador_clientes.o garcons.o

all: restaurante

restaurante: $(OBJ)
	$(CC) $(CFLAGS) -o restaurante $(OBJ)

main.o: main.c restaurante.h
	$(CC) $(CFLAGS) -c main.c

fila_clientes.o: fila_clientes.c restaurante.h
	$(CC) $(CFLAGS) -c fila_clientes.c

fila_pedidos.o: fila_pedidos.c restaurante.h
	$(CC) $(CFLAGS) -c fila_pedidos.c

gerador_clientes.o: gerador_clientes.c restaurante.h
	$(CC) $(CFLAGS) -c gerador_clientes.c

garcons.o: garcons.c restaurante.h
	$(CC) $(CFLAGS) -c garcons.c

clean:
	rm -f restaurante $(OBJ)

.PHONY: all clean
