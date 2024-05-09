#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#ifndef TEMPO_MEDIO_DE_UMA_COMPRA
#define TEMPO_MEDIO_DE_UMA_COMPRA 1
#endif

#if !defined(TEMPO_MEDIO_DE_UMA_REPOSCAO)
#define TEMPO_MEDIO_DE_UMA_REPOSCAO 1
#endif

#ifndef QTDPRODUTO
#define QTDPRODUTO 5
#endif

#ifndef QTDCLIENTE
#define QTDCLIENTE 3
#endif

#ifndef QTDREPOSITOR
#define QTDREPOSITOR 1
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

  exit_client();
  exit_repositore();

  pthread_mutex_destroy(&product);

  freeProdutos();
  freeCliente();
  freeRepositores();
  return 0;
}