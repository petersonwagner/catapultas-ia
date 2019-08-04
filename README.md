# catapultas-ia
Trabalho de Inteligencia Artificial - Busca adversaria usando Mini-Max

### Compilar:
    `make`

### Executar
    `./minimax o`
    `./minimax x`


### Especificação
O jogo "catapultas" é um jogo de tabuleiro onde dois jogadores se
enfretam em um tabuleiro quadriculado de 6x6 células, ocupadas por
peças de dois tipos: 'x' e 'o'. Cada jogador ocupa a metade do
tabuleiro com 18 peças idênticas. O jogador 'x' ocupa a parte superior
e o jogador 'o' a parte inferior do tabuleiro. O tabuleiro inicial é
dado por:
```
    123456
   ########
 1 #xxxxxx#
 2 #xxxxxx#
 3 #xxxxxx#
 4 #oooooo#
 5 #oooooo#
 6 #oooooo#
   ########
```

O objetivo é eliminar as peças do adversário. Vence o jogador que
eliminar 17 peças do seu adversário ou aquele que, depois de um
determinado número de jogadas, tiver o maior número de peças no
tabuleiro.

O jogador 'x' começa a partida e os jogadores se revesam movimentando
suas próprias peças.

As jogadas possíveis são:

  1) mover: move uma peça para uma célula vazia adjacente (adjacência
            vertical e horizontal). Note que este tipo de jogada não é
            possível no tabuleiro inicial do jogo.

  2) saltar: salta uma peça sobre outra peça adjacente do mesmo tipo.
             O salto é em linha reta na vertical ou na horizoltal,
             sobre uma única peça e a célula de destino é a célula
             seguinte à adjacente. Mais de um salto consecutivo pode
             ser realizado em uma mesma jogada. O destino de cada
             salto pode ser uma célula vazia ou uma célula ocupada por
             uma peça do adversário. Neste último caso a peça do
             adversário é eliminada.

Considerando o tabuleiro inicial, a jogada do jogador 'x' de saltar a
peça da posição (2,1) sobre a peça da posição (3,1) elimina a peça 'o'
da posição (4,1) e resulta no tabuleiro:
```
    123456
   ########
 1 #xxxxxx#
 2 #-xxxxx#
 3 #xxxxxx#
 4 #xooooo#
 5 #oooooo#
 6 #oooooo#
   ########
```
O jogador 'o' salta a peça da posição (6,1) sobre a peça da posição
(5,1) e elimina a peça 'x' da posição (4,1):
```
    123456
   ########
 1 #xxxxxx#
 2 #-xxxxx#
 3 #xxxxxx#
 4 #oooooo#
 5 #oooooo#
 6 #-ooooo#
   ########
```
O jogador 'x' faz um salto consecutivo de dois passos, saltando a peça
da posição (2,3) para (2,1) e depois para (4,1):
```
    123456
   ########
 1 #xxxxxx#
 2 #-x-xxx#
 3 #xxxxxx#
 4 #xooooo#
 5 #oooooo#
 6 #-ooooo#
   ########
```
O jogador 'o' move a peça da posição (6,2) para a posição (6,1):
```
    123456
   ########
 1 #xxxxxx#
 2 #-x-xxx#
 3 #xxxxxx#
 4 #xooooo#
 5 #oooooo#
 6 #o-oooo#
   ########
```
As jogadas são descritas por sequências de caracteres e podem ser de
dois tipos: mover (m) e saltar (s).

As jogadas do tipo mover são dadas pelo formato:

   `<jogador> m <l_ini> <c_ini> <l_fin> <c_fin>`

onde, jogador indica quem esta jogando, l_ini e c_ini indicam a linha
e a coluna da posição inicial do movimento e l_fin e c_fin a posição
final do movimento.

Por exemplo, a jogada que move a peça 'o' da posição (6,2) para a
posição (6,1) é dada por:

   `o m 6 2 6 1`

As jogadas de salto são dadas pelo formato:

   `<jogador> s <num_tuplas> <l_ini> <c_ini> <l_s_1> <c_s_1> ...`

onde, num_tuplas indica o número de pares de números que compõem o
restante da sequência de caracteres que representa a jogada, l_ini e
c_ini indicam a posição inicial e, l_s_i e c_s_i indicam a posição
destino do salto i. Nenhuma das posições de destino dos saltos pode
ser igual a posição inicial.

Note que o número de saltos é igual a num_tuplas menos 1, pois a
primeira tupla indica a posição inicial.

Por exemplo, a jogada:

   `x s 3 2 3 2 1 4 1`

salta a peça 'x' da posição (2,3) para (2,1) e depois para (4,1).

### Solução:
O programa recebe o tabuleiro e roda o algoritmo mini-max com podas alfa-beta.
O algoritmo faz o mesmo que o seguinte pseudo-algoritmo:

```python
def minimax(tabuleiro, depth=8, ...):
   se depth == 0:
      return heuristica(tabuleiro)

   para toda celula (i,j) do tabuleiro:
      para toda ação possivel para o player que esta jogando ('s' ou 'm'): //esses seriam os nos filhos do tabuleiro
         se (a ação é aplicavel no tabuleiro):

            valor = minimax(tabuleiro)
            se eh_ainda_melhor(melhor_valor, valor):   //melhor valor considerando tanto min quanto max para simplificar o pseudo codigo
               melhor_valor = valor
                
            se podemos podar:
               return melhor_valor
            return melhor_valor
```     
    
heuristica:
Retorna a diferença entre o número de peças do nosso jogador e o número de peças do jogador adversário.
