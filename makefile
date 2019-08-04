CFLAGS = -g -Wall -Wno-unused-result
LDLIBS = -l hiredis -l readline

all: controlador minimax

controlador: controlador.c

minimax: tabuleiro.h tabuleiro.c minimax.c

clean:
	rm -f controlador exemplo1 exemplo2 teste minimax
