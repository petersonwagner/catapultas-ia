#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabuleiro.h"
#include <math.h>
#include <stdbool.h>

#define MAXSTR 512
#define MAXINT 16
#define OUTRO(l) ((l) == 'x' ? 'o' : 'x')
#define ABS(x) ((x < 0) ? (-(x)) : (x))
#define POS(l, c) ((l)*9 + (c))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

int minimax(char board[MAXSTR], int depth, float alpha, float beta, bool maximizing, char player, char move[MAXSTR]);

int main(int argc, char **argv)
{
  char buf[MAXSTR];
  char tabuleiro[MAXSTR];
  char lado_meu;
  char lado_adv;
  char tipo_mov_adv;
  int num_mov_adv;
  int mov_adv_l[MAXINT];
  int mov_adv_c[MAXINT];
  int i;

  // conecta com o controlador do campo
  tabuleiro_conecta(argc, argv);

  // recebe o campo inicial e o movimento do adversario
  while (1)
  {
    tabuleiro_recebe(buf);
    // separa os elementos do string recebido
    sscanf(strtok(buf, " \n"), "%c", &lado_meu);
    sscanf(strtok(NULL, " \n"), "%c", &lado_adv);
    sscanf(strtok(NULL, " \n"), "%c", &tipo_mov_adv);
    if (tipo_mov_adv == 'm')
    {
      num_mov_adv = 2;
      for (i = 0; i < num_mov_adv; i++)
      {
        sscanf(strtok(NULL, " \n"), "%d", &(mov_adv_l[i]));
        sscanf(strtok(NULL, " \n"), "%d", &(mov_adv_c[i]));
      }
    }
    else if (tipo_mov_adv == 's')
    {
      sscanf(strtok(NULL, " \n"), "%d", &num_mov_adv);
      for (i = 0; i < num_mov_adv; i++)
      {
        sscanf(strtok(NULL, " \n"), "%d", &(mov_adv_l[i]));
        sscanf(strtok(NULL, " \n"), "%d", &(mov_adv_c[i]));
      }
    }
    strncpy(tabuleiro, strtok(NULL, "."), MAXSTR);

    // mostra o que recebeu
    printf("%c\n", lado_meu);
    printf("%c %c", lado_adv, tipo_mov_adv);
    if (tipo_mov_adv != 'n')
    {
      if (tipo_mov_adv == 's')
        printf(" %d", num_mov_adv);
      for (i = 0; i < num_mov_adv; i++)
      {
        printf(" %d", mov_adv_l[i]);
        printf(" %d", mov_adv_c[i]);
      }
    }
    printf("\n");
    printf("%s", tabuleiro);

    buf[0] = '1';
    minimax(tabuleiro, 8, -INFINITY, +INFINITY, true, lado_meu, buf);

    printf("\nbuf:%s\nbuf[MAXSTR]:", buf);

    /*
      for (int i = 0; i < MAXSTR; i++)
      {
        char character = buf[i] ;
        if ((character >= 'a' && character <= 'z') ||
        (character >= 'A' && character <= 'Z') || 
        (character >= '1' && character <= '9'))
          printf("%c", buf[i]);
        else if (character == '\0')
          printf("_");
        else
          printf("%c", '*');
      }
      fflush(stdout);
      */

    // envia o movimento para o controlador do campo
    tabuleiro_envia(buf);
  }
}

int pos_valida(int l, int c)
{
  if (l < 1 || l > 6 || c < 1 || c > 6)
    return 0;
  return 1;
}

int aplica(char *buf, char *tabuleiro, char lado, char tipo,
           int num_mov, int *mov_l, int *mov_c)
{
  int i, l, c, p, ln, cn, pn, dist;

  strcpy(buf, tabuleiro);
  if (tipo == 'n')
    return 1;
  l = mov_l[0];
  c = mov_c[0];
  if (!pos_valida(l, c))
    return 0;
  p = POS(l, c);
  if (buf[p] != lado)
    return 0;
  if (tipo == 'm')
  {
    buf[p] = '-';
    ln = mov_l[1];
    cn = mov_c[1];
    if (!pos_valida(ln, cn))
      return 0;
    pn = POS(ln, cn);
    dist = ln + cn - l - c;
    dist = ABS(dist);
    if ((buf[pn] != '-') ||         // destino livre
        ((ln != l) && (cn != c)) || // movimento na linha ou coluna
        (dist != 1))                // movimento simples de 1 passo
      return 0;
    buf[pn] = lado;
  }
  else
  { // tipo s
    for (i = 1; i < num_mov; i++)
    {
      buf[p] = '-';
      ln = mov_l[i];
      cn = mov_c[i];
      if (!pos_valida(ln, cn))
        return 0;
      pn = POS(ln, cn);
      dist = ln + cn - l - c;
      dist = ABS(dist);
      if ((buf[pn] == lado) ||        // destino livre ou adversario
          ((ln != l) && (cn != c)) || // movimento na linha ou coluna
          (dist != 2))                // movimento de salto, 2 passos
        return 0;
      l = (l + ln) / 2; // posicao da peca que sera saltada
      c = (c + cn) / 2;
      p = POS(l, c);
      if (buf[p] != lado)
        return 0;
      buf[pn] = lado;
      l = ln;
      c = cn;
      p = pn;
    }
  }
  return 1;
}


int heuristic(char *board, char player)
{
  int l, c, p, adv;

  p = adv = 0;
  for (l = 1; l < 7; l++)
    for (c = 1; c < 7; c++)
      if (board[POS(l, c)] == player)
        p++;
      else if (board[POS(l, c)] == OUTRO(player))
        adv++;

  return p - adv;
}

int minimax(char board[MAXSTR], int depth, float alpha, float beta, bool maximizing, char player, char move[MAXSTR])
{
  int c, r, i, n_moves;
  int mov_r[2], mov_c[2];
  float eval, maxEval = 0, minEval;
  char buf[MAXSTR], move_type, aux[MAXSTR];
  int save_move = (move[0] == '1') ? 1 : 0;

  if (depth == 0)
    return heuristic(board, player);

  strcpy(buf, board);

  n_moves = 2;

  //#####################################################
  //                  MAX
  //#####################################################
  if (maximizing)
  {
    maxEval = -INFINITY;

    for (r = 1; r < 7; r++)
    {
      for (c = 1; c < 7; c++)
      {
        mov_r[0] = r;
        mov_c[0] = c;

        //SALTO
        move_type = 's';
        for (i = -2; i < 3; i += 4)
        {
          //HORIZONTAL (LINHA)
          mov_r[1] = r + i;
          mov_c[1] = c;
          if (aplica(buf, board, player, move_type, n_moves, mov_r, mov_c))
          {
            aux[0] = '0';
            eval = minimax(buf, depth - 1, alpha, beta, false, player, aux);
            if (maxEval <= eval)
            {
              maxEval = eval;
              if (save_move == 1)
              {
                sprintf(move, "%c %c %d %d %d %d %d\n", player, move_type, n_moves, mov_r[0], mov_c[0], mov_r[1], mov_c[1]);
              }
            }
            alpha = MAX(alpha, eval);
            if (beta <= alpha)
              return maxEval;
          }

          //VERTICAL (COLUNA)
          mov_r[1] = r;
          mov_c[1] = c + i;
          if (aplica(buf, board, player, move_type, n_moves, mov_r, mov_c))
          {
            aux[0] = '0';
            eval = minimax(buf, depth - 1, alpha, beta, false, player, aux);
            if (maxEval <= eval)
            {
              maxEval = eval;
              if (save_move == 1)
              {
                sprintf(move, "%c %c %d %d %d %d %d\n", player, move_type, n_moves, mov_r[0], mov_c[0], mov_r[1], mov_c[1]);
              }
            }
            alpha = MAX(alpha, eval);
            if (beta <= alpha)
              return maxEval;
          }
        }

        //MOVE
        move_type = 'm';
        for (i = -1; i < 2; i += 2)
        {
          //HORIZONTAL (LINHA)
          mov_r[1] = r + i;
          mov_c[1] = c;
          if (aplica(buf, board, player, move_type, n_moves, mov_r, mov_c))
          {
            aux[0] = '0';
            eval = minimax(buf, depth - 1, alpha, beta, false, player, aux);
            if (maxEval <= eval)
            {
              maxEval = eval;
              if (save_move == 1)
              {
                sprintf(move, "%c %c %d %d %d %d %d\n", player, move_type, n_moves, mov_r[0], mov_c[0], mov_r[1], mov_c[1]);
              }
            }
            alpha = MAX(alpha, eval);
            if (beta <= alpha)
              return maxEval;
          }
          //VERTICAL (COLUNA)

          mov_r[1] = r;
          mov_c[1] = c + i;
          if (aplica(buf, board, player, move_type, n_moves, mov_r, mov_c))
          {
            aux[0] = '0';
            eval = minimax(buf, depth - 1, alpha, beta, false, player, aux);
            if (maxEval <= eval)
            {
              maxEval = eval;
              if (save_move == 1)
              {
                sprintf(move, "%c %c %d %d %d %d %d\n", player, move_type, n_moves, mov_r[0], mov_c[0], mov_r[1], mov_c[1]);
              }
            }
            alpha = MAX(alpha, eval);
            if (beta <= alpha)
              return maxEval;
          }
        }
      }
    }
    return maxEval;
  }

  //#####################################################
  //                  MIN
  //#####################################################
  else
  {
    minEval = +INFINITY;

    for (r = 1; r < 7; r++)
    {
      for (c = 1; c < 7; c++)
      {
        mov_r[0] = r;
        mov_c[0] = c;

        //SALTO
        move_type = 's';
        for (i = -2; i < 3; i += 4)
        {
          //HORIZONTAL (LINHA)
          mov_r[1] = r + i;
          mov_c[1] = c;
          if (aplica(buf, board, OUTRO(player), move_type, n_moves, mov_r, mov_c))
          {
            aux[0] = '0';
            eval = minimax(buf, depth - 1, alpha, beta, true, player, aux);
            minEval = MIN(minEval, eval);
            beta = MIN(beta, eval);
            if (beta <= alpha)
              return minEval;
          }

          //VERTICAL (COLUNA)
          mov_r[1] = r;
          mov_c[1] = c + i;
          if (aplica(buf, board, OUTRO(player), move_type, n_moves, mov_r, mov_c))
          {
            aux[0] = '0';
            eval = minimax(buf, depth - 1, alpha, beta, true, player, aux);
            minEval = MIN(minEval, eval);
            beta = MIN(beta, eval);
            if (beta <= alpha)
              return minEval; //r=c=7; //break
          }
        }

        //MOVE
        move_type = 'm';
        for (i = -1; i < 2; i += 2)
        {
          //HORIZONTAL (LINHA)
          mov_r[1] = r + i;
          mov_c[1] = c;
          if (aplica(buf, board, OUTRO(player), move_type, n_moves, mov_r, mov_c))
          {
            aux[0] = '0';
            eval = minimax(buf, depth - 1, alpha, beta, true, player, aux);
            minEval = MIN(minEval, eval);
            beta = MIN(beta, eval);
            if (beta <= alpha)
              return minEval;
          }

          //VERTICAL (COLUNA)
          mov_r[1] = r;
          mov_c[1] = c + i;
          if (aplica(buf, board, OUTRO(player), move_type, n_moves, mov_r, mov_c))
          {
            aux[0] = '0';
            eval = minimax(buf, depth - 1, alpha, beta, true, player, aux);
            minEval = MIN(minEval, eval);
            beta = MIN(beta, eval);
            if (beta <= alpha)
              return minEval;
          }
        }
      }
    }
  }

  return minEval;
}
