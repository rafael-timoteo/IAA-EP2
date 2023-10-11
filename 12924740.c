/*********************************************************************/
/**   ACH2002 - Introducao à Análise de Algoritmos                  **/
/**   EACH-USP - Segundo Semestre de 2022                           **/
/**   Turma extra - Prof. Marcos Lordello Chaim                     **/
/**                                                                 **/
/**   Segundo Exercicio-Programa                                    **/
/**                                                                 **/
/**   <Rafael Francisco de Freitas Timoteo>     <12924740>          **/
/**                                                                 **/
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define true 1
#define false 0

typedef int boolean;
typedef struct {
  boolean exists;
  boolean belongs;
} celula_mochila;

/*
dijkstra() é uma função que implementa o algoritmo de Dijstra para encontrar
a menor distância entre dois pontos em um grafo.

Parâmetros:
grf : o ponteiro para o arranjo que representa o grafo. O elemento grf[i][j]
contém a distância entre os vértices i e j do grafo, caso exista uma aresta do nó
i para o nó j. Caso não exista uma aresta de i para j, então o valor de grf[i][j]
é INT_MAX (o maior valor possível de um inteiro)
n : tamanho do arranjo grf, isto é, grf é uma matriz nxn;
s : vértice inicial;
v_f : vértice final

Valor de retorno: menor distância entre s e v_f.
Observação: 0 =< s e v_f < n. Retorne a distância 0 (zero) caso os valores de
s e v_f não sejam válidos.

*/

int pertence(int * vec, int n, int x) {

for(int i = 0; i < n; i++)
   if(vec[i] == x)
     {
       return 1;
     }

 return 0;
}

int remove_vertice(int * vec, int n, int x) {
 int i = 0;
 
 for(int j = 0; j < n; j++)
   if(vec[j] != x)
     {
       vec[i] = vec[j];
       i++;
     }

 return i;
}

int min_vertice_dist(int * d, int * q, int n){
  int min = INT_MAX;
  int vmin = 0;

  for (int i = 0; i < n; ++i)
    if(d[q[i]] < min)
        {
          min = d[q[i]];
          vmin = q[i];
        }

  return vmin;
}

int dijkstra(int ** grf, int n, int s, int v_f) {
int * dist = (int *) malloc(sizeof(int)*n);
int * q = (int *) malloc(sizeof(int)*n);
int alt,v,u, qsize = n;

if(q == NULL || dist == NULL)
  return 0;

if(n <= 0)
  return 0;

if((s < 0 || s > n-1) || (v_f < 0 || v_f > n-1))
  return 0;

dist[s] = 0;

for(v=0; v < n; ++v)
  {
    if(s != v)
      dist[v] = INT_MAX;
    q[v] = v;
  }

while(qsize > 0) {
  v = min_vertice_dist(dist, q, qsize);
  qsize = remove_vertice(q,qsize,v);

  for(u = 0; u < n; ++u) {
    if(grf[v][u] != INT_MAX) {
    alt = dist[v]+grf[v][u];
    if (alt < dist[u])
        dist[u] = alt;
    }
 }
}

return dist[v_f];
}

/*
mochila() retorna uma matriz P[n+1,K+1] com as possíveis soluções para o problema da
mochila para um arranjo S de tamanho n e uma mochila de tamanho K.
P é uma matriz tal que P[i,j].exists = true, se existe uma solução para o
problema da mochila com os primeiros i
(0 <= i <= n) elementos e uma mochila de tamanho k (0 <= k <= K);
e P[i,j].belongs = true, se o i-ésimo elemento pertence à solução.

Parâmetros:
S: um arranjo de tamanho n que armazena os tamanhos dos itens.
n: tamanho do arranjo S
K: tamanho da mochila

Valor de retorno: ponteiro para a matriz P.

Observação: Os valores válidos para S, n e K são, respectivamente,
S != NULL, n > 0, K > 0. A função deve retornar NULL se os valores
de algum dos parâmetros não for válido.

Dica: não se esqueça de fazer uma cópia de S, chamada,  por exemplo,
S2, que inclui um item de tamanho 0 (zero). Por exemplo, se
S[] = {2,3,5,6}, S2 será S2 [] = {0,2,3,5,6}.
*/

celula_mochila ** mochila(int * S, int n, int K)
{
  if(S == NULL || n <= 0 || K <= 0)
    return NULL;

  celula_mochila ** p = (celula_mochila**) malloc(sizeof(celula_mochila*) * (n + 1));
  
  for(int i = 0; i < n + 1; i++)
      p[i] = (celula_mochila*) malloc(sizeof(celula_mochila) * (K + 1));
  
  int * S2 = (int*) malloc(sizeof(int) * n + 1);

  S2[0] = 0;
  p[0][0].exists = true;

  for(int z = 1; z < n + 1; z++)
    S2[z] = S[z - 1];

  for(int w = 1; w < K + 1; w++)
    p[0][w].exists = false;

  for(int j = 1 ; j <= n ; j++){
    for(int l = 0 ; l <= K ; l++){
      p[j][l].exists = false;
      if (p[j - 1][l].exists){
        p[j][l].exists = true;
        p[j][l].belongs = false;
      }
      else
        if(l - S2[j] >= 0){
          if(p[j - 1][l - S2[j]].exists){
            p[j][l].exists = true;
            p[j][l].belongs = true;
          }
        }
     }
  }
  return p;
}

/* selecionaItensMochila() percorre uma matriz P[n+1,K+1] com as
possíveis soluções para o problema da
mochila para um arranjo S de tamanho n e uma mochila de tamanho K e
adiciona a um arranjo recebido como parâmetro os itens que preenchem
completamente a mochila.

Parâmetros:
P: P[n+1,K+1] com as possíveis soluções para o problema da
mochila para um arranjo S de tamanho n e uma mochila de tamanho K.
itens: ponteiro para o arranjo que conterá os itens que preenchem
a mochila.
S: um arranjo de tamanho n que armazena os tamanhos dos itens.
n: tamanho do arranjo S
K: tamanho da mochila

Valor de retorno: nenhum, pois os valores desejados deverão ser incluídos
no arranjo itens passado como parâmetro.

*/

void selecionaItensMochila(int * itens, celula_mochila ** P, int *S, int n, int K) {
  int i, k;
  int w = 0;
  i = n;
  k = K;

  while(i > 0 && k > 0){
    if(P[i][k].exists == true){
        if(P[i][k].belongs == true){
            itens[w] = S[i - 1];
            k = k - S[i - 1];
            w++;
        }
    }
    i--;
  }
}

/* imprimeMatrizP() foi criada para imprimir de forma "bonita"
a matriz P retornada pela função mochila().
*/

void imprimeMatrizP(celula_mochila ** p, int * s, int n, int K){
 int i,j;

  printf("\n   P   ");
  for(i=0; i < K+1; i++)
    printf("  %d  ",i);
  printf("\n");

  for(i=0; i < n+1; i++){
    if(i == 0)
      printf("k_0 = 0 ");
    else
      printf("k_%d =%2d ", i,s[i-1]);
    for(j=0; j < K+1; j++)
      {
      printf("%c/",(p[i][j].exists)?'T':'F');
      printf("%c  ",(p[i][j].belongs)?'T':'F');
      }
    printf("\n");
  }

}
/*
	função main criada apenas para seus testes.
  Os testes abaixo são apenas ilustrativos.
  A correção será realizada com testes diferentes.
	Voce deve adicionar novos testes que testem
  o mais completamente possível seu código.
	ESTA função SERA IGNORADA NA CORRECAO
*/

int main() {
int ** grf_dist;
int i, j, ini, fim, n = 7;
int noErros = 0, noTestes = 0;

// Prepara dados para testar a função dijkstra()
grf_dist = (int **) malloc(n*sizeof(int *));
for(i = 0; i < n; ++i) {
  grf_dist[i] = (int *) malloc(n*sizeof(int));
}

for(i = 0; i < n; ++i)
  for(j = 0; j < n; ++j)
    grf_dist[i][j] = INT_MAX;

grf_dist[0][1] = 6;
grf_dist[0][2] = 8;
grf_dist[0][3] = 7;
grf_dist[1][3] = 100;
grf_dist[1][4] = 99;
grf_dist[2][5] = 1;
grf_dist[3][2] = 200;
grf_dist[3][4] = 400;
grf_dist[3][5] = 500;
grf_dist[4][6] = 200;
grf_dist[5][4] = 600;
grf_dist[5][6] = 1;

// for(i = 0; i < n; ++i)
//   {
//     for(j = 0; j < n; ++j)
//       printf("grf_dist[%d][%d] = %d ", i, j, grf_dist[i][j]);
//     printf("\n");
//    }

noTestes++;
ini = 0; fim = 6;
printf("Distância menor de %d a %d: %d\n", ini, fim, dijkstra(grf_dist,n,ini,fim));
if(dijkstra(grf_dist,n,ini,fim) == 10)
  {
  printf("Teste %d: OK\n", noTestes);
  }
else {
  printf("Teste %d: NOK: ", noTestes);
  printf("Distância menor de %d a %d: %d\n", ini, fim, 10);
  noErros++;
  }

noTestes++;
ini = 0; fim = 5;
printf("Distância menor de %d a %d: %d\n", ini, fim, dijkstra(grf_dist,n,ini,fim));

if(dijkstra(grf_dist,n,ini,fim) == 9)
  {
  printf("Teste %d: OK\n", noTestes);
  }
else {
  printf("Teste %d: NOK: ", noTestes);
  printf("Distância menor de %d a %d: %d\n", ini, fim, 9);
  noErros++;
  }

noTestes++;
ini = 0; fim = 4;
printf("Distância menor de %d a %d: %d\n", ini, fim, dijkstra(grf_dist,n,ini,fim));

if(dijkstra(grf_dist,n,ini,fim) == 105)
  {
  printf("Teste %d: OK\n", noTestes);
  }
else {
  printf("Teste %d: NOK:", noTestes);
  printf("Distância menor de %d a %d: %d\n", ini, fim,105);
  noErros++;
  }

noTestes++;
ini = -1; fim = 4;
printf("Distância menor de %d a %d: %d\n", ini, fim, dijkstra(grf_dist,n,ini,fim));

if(dijkstra(grf_dist,n,ini,fim) == 0)
  {
  printf("Teste %d: OK\n", noTestes);
  }
else {
  printf("Teste %d: NOK:", noTestes);
  printf("Distância menor de %d a %d: %d\n", ini, fim,0);
  noErros++;
  }

noTestes++;
ini = 0; fim = 14;
printf("Distância menor de %d a %d: %d\n", ini, fim, dijkstra(grf_dist,n,ini,fim));

if(dijkstra(grf_dist,n,ini,fim) == 0)
  {
  printf("Teste %d: OK\n", noTestes);
  }
else {
  printf("Teste %d: NOK:", noTestes);
  printf("Distância menor de %d a %d: %d\n", ini, fim,0);
  noErros++;
  }

// Prepara dados para testar a função mochila() e selecionaItensMochila()
noTestes++;
int r[] = {2,3,5,6}; // itens a serem possivelmente salvos na celula_mochila
int K = 9;
n = 4;
int itensmochila[] = {-1,-1,-1,-1};
celula_mochila ** p = mochila(r,n,K);

if(p != NULL)
  {
  imprimeMatrizP(p,r,n,K);
  selecionaItensMochila(itensmochila, p, r,n,K);

  printf("Itens mochila de tamanho 9: ");
  for(i=0; i < 4; ++i)
    if(itensmochila[i]!=-1)
      printf("%d ",itensmochila[i]);
  printf("\n");

  if(itensmochila[0] == 6 && itensmochila[1] == 3)
   {
     printf("Teste %d: OK\n", noTestes);
   }
   else {
     printf("Teste %d: NOK:", noTestes);
     printf("Itens mochila de tamanho 9: 6 3\n");
     noErros++;
   }
}
else {
  printf("Teste %d: NOK:", noTestes);
  printf("Itens mochila de tamanho 9: 6 3\n");
  noErros++;
}

noTestes++;
int s[] = {1,2,3,5,6}; // itens a serem possivelmente salvos na celula_mochila
K = 9;
n = 5;
int itensmochila2[] = {-1,-1,-1,-1,-1};
p = mochila(s,n,K);

if(p != NULL)
  {
  imprimeMatrizP(p,s,n,K);
  selecionaItensMochila(itensmochila2, p,s,n,K);
  printf("Itens mochila de tamanho 9: ");
  for(i=0; i < 5; ++i)
    if(itensmochila2[i]!=-1)
      printf("%d ",itensmochila2[i]);
  printf("\n");
  if(itensmochila2[0] == 5 && itensmochila2[1] == 3 && itensmochila2[2] == 1)
   {
     printf("Teste %d: OK\n", noTestes);
   }
   else {
     printf("Teste %d: NOK:", noTestes);
     printf("Itens mochila de tamanho 9: 5 3 1\n");
     noErros++;
   }
}
else {
  printf("Teste %d: NOK:", noTestes);
  printf("Itens mochila de tamanho 9: 5 3 1\n");
  noErros++;
}

noTestes++;
int itensmochila3[] = {-1,-1,-1,-1,-1};
if(p != NULL)
  {
  selecionaItensMochila(itensmochila3, p,s,n,7);
  printf("Itens mochila de tamanho 7: ");
  for(i=0; i < 5; ++i)
    if(itensmochila3[i]!=-1)
      printf("%d ",itensmochila3[i]);
  printf("\n");
  if(itensmochila3[0] == 5 && itensmochila3[1] == 2)
   {
     printf("Teste %d: OK\n", noTestes);
   }
   else {
     printf("Teste %d: NOK:", noTestes);
     printf("Itens mochila de tamanho 7: 5 2\n");
     noErros++;
   }
}
else {
  printf("Teste %d: NOK:", noTestes);
  printf("Itens mochila de tamanho 7: 5 2\n");
  noErros++;
}

// Testa verificação dos parâmetros
 noTestes++;
 p = mochila(s,0,K);
 if(p==NULL)
   {
   printf("Teste %d: OK\n", noTestes);
   }
 else{
   printf("Teste %d: NOK:", noTestes);
   printf("p == NULL\n");
   noErros++;
   }

   noTestes++;
   p = mochila(NULL,5,K);
   if(p==NULL)
     {
     printf("Teste %d: OK\n", noTestes);
     }
   else{
     printf("Teste %d: NOK:", noTestes);
     printf("p == NULL\n");
     noErros++;
     }

     noTestes++;
     p = mochila(s,5,-1);
     if(p==NULL)
       {
       printf("Teste %d: OK\n", noTestes);
       }
     else{
       printf("Teste %d: NOK:", noTestes);
       printf("p == NULL\n");
       noErros++;
       }
}
