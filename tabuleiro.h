#ifndef __TABULEIRO_H__
#define __TABULEIRO_H__

void tabuleiro_conecta(int, char**);
void tabuleiro_envia(char*);
void tabuleiro_recebe(char*);
int aplicavel(char *buf, char *tabuleiro, char lado, char tipo,
	   int num_mov, int *mov_l, int *mov_c);

#endif
