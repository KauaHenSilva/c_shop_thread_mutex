#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// #define EXIBIR_COMPRAS_FALHAS

#ifndef TEMPO_MEDIO_DE_UMA_COMPRA
#define TEMPO_MEDIO_DE_UMA_COMPRA 1
#endif

#ifndef TEMPO_MEDIO_DE_UMA_REPOSCAO
#define TEMPO_MEDIO_DE_UMA_REPOSCAO 3
#endif

#ifndef QUANTIDADE_POR_REPOSICAO
#define QUANTIDADE_POR_REPOSICAO 5
#endif

#ifndef QUANTIDADE_INICIAL_REPOSICAO
#define QUANTIDADE_INICIAL_REPOSICAO 10
#endif

#ifndef QTDPRODUTO
#define QTDPRODUTO 12
#endif

#ifndef QTDCLIENTE
#define QTDCLIENTE 7
#endif

#ifndef QTDREPOSITOR
#define QTDREPOSITOR 4
#endif

#include "defincoes/definicoes.c"
#include "threads/threads.c"

int main()
{
  defProdutos();
  defCliente();
  defRepositores();

  runThreadCliente();
  runThreadRepositor();
  runThreadVerificarMercado();

  finalizamentoThreads();

  freeProdutos();
  freeCliente();
  freeRepositores();
  return 0;
}