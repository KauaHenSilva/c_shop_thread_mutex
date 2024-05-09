#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#ifndef QTDPRODUTO
#define QTDPRODUTO 30
#endif

#ifndef QTDCLIENTE
#define QTDCLIENTE 10
#endif

#ifndef QTDREPOSITORIO
#define QTDREPOSITORIO 10
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