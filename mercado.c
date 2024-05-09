#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#ifndef TEMPO_MEDIO_DE_UMA_COMPRA
#define TEMPO_MEDIO_DE_UMA_COMPRA 1
#endif

#ifndef TEMPO_MEDIO_DE_UMA_REPOSCAO
#define TEMPO_MEDIO_DE_UMA_REPOSCAO 3
#endif

#ifndef QUANTIDADE_POR_REPOSICAO
#define QUANTIDADE_POR_REPOSICAO 20
#endif

#ifndef QUANTIDADE_INICIAL_REPOSICAO
#define QUANTIDADE_INICIAL_REPOSICAO 0
#endif

#ifndef QTDPRODUTO
#define QTDPRODUTO 50
#endif

#ifndef QTDCLIENTE
#define QTDCLIENTE 10
#endif

#ifndef QTDREPOSITOR
#define QTDREPOSITOR 10
#endif

#include "defincoes/definicoes.c"
#include "threads/threads.c"


int main()
{
  defProdutos();
  defCliente();
  defRepositores();

  pthread_mutex_init(&product, NULL);

  runTreadCliente();
  runTreadRepositor();
  runThreadVerificarMercado();

  exit_client();
  exit_repositore();

  pthread_mutex_destroy(&product);

  freeProdutos();
  freeCliente();
  freeRepositores();
  return 0;
}