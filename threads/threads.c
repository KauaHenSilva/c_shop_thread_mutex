#include "threads.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef TEMPO_MEDIO_DE_UMA_COMPRA
#define TEMPO_MEDIO_DE_UMA_COMPRA 0
#endif

#if !defined(TEMPO_MEDIO_DE_UMA_REPOSCAO)
#define TEMPO_MEDIO_DE_UMA_REPOSCAO 0
#endif

#ifndef QTDPRODUTO
#define QTDPRODUTO 30
#endif

#ifndef QTDCLIENTE
#define QTDCLIENTE 10
#endif

#ifndef QTDREPOSITOR
#define QTDREPOSITOR 10
#endif

#if !defined(STRUCTPRODUTOCLIENTEREPOSITORIO)
#define STRUCTPRODUTOCLIENTEREPOSITORIO

struct produto
{
  int idProduto;
  int qtdProduto;
};

struct cliente
{
  pthread_t thread;
  int id;
};

struct repositores
{
  pthread_t thread;
  int id;
};

#endif // STRUCTPRODUTOCLIENTEREPOSITORIO

#if !defined(VARGLOBALPRODUTOCLIENTEREPOSITORIO)
#define VARGLOBALPRODUTOCLIENTEREPOSITORIO

pthread_mutex_t product;
Produto *produto;
Cliente *cliente;
Repositores *repositores;

#endif // VARGLOBALPRODUTOCLIENTEREPOSITORIO

void *run_client(void *positionCliente)
{
  int *idCliente = (int *)positionCliente;

  int idxProduto;
  int qtdCompras = 10000;

  while (qtdCompras)
  {
    idxProduto = rand() % QTDPRODUTO;

    pthread_mutex_lock(&product);

    if (produto[idxProduto].qtdProduto > 0)
    {
      produto[idxProduto].qtdProduto--;
      printf("O cliente %02d, ", cliente[*idCliente].id);
      printf("comprou o produto %02d, ", produto[idxProduto].idProduto);
      printf("sobrou %02d. \n", produto[idxProduto].qtdProduto);
    }

    pthread_mutex_unlock(&product);
    qtdCompras--;
    sleep(rand() % (TEMPO_MEDIO_DE_UMA_COMPRA + 1));
  }

  printf("O cliente %2d, arrasta para cima!\n", cliente[*idCliente].id);

  free(idCliente);
  return NULL;
}

void *run_Repositore(void *positionRepostor)
{
  int *idRepositor = (int *)positionRepostor;
  int qtdRepor;
  int idxProduto;

  int qtdReposicao = 10000;

  while (qtdReposicao)
  {
    idxProduto = rand() % QTDPRODUTO;
    qtdRepor = rand() % QTDPRODUTO + 1;

    pthread_mutex_lock(&product);

    if (produto[idxProduto].qtdProduto == 0)
    {
      produto[idxProduto].qtdProduto += qtdRepor;
      printf("O Repistor nmr %02d, ", repositores[*idRepositor].id);
      printf("repos %02d item ", qtdRepor);
      printf("do produto %02d. \n", produto[idxProduto].idProduto);
    }

    pthread_mutex_unlock(&product);

    qtdReposicao--;
    sleep(rand() % (TEMPO_MEDIO_DE_UMA_COMPRA + 1));
  }

  return NULL;
}

void runTreadCliente()
{
  for (int x = 0; x < QTDCLIENTE; x++)
  {
    int *aux = (int *)malloc(sizeof(int));
    *aux = x;

    if (pthread_create(&cliente[x].thread, NULL, &run_client, (void *)aux) != 0)
      perror("Erro a criar");
  }
}

void runTreadRepositor()
{
  for (int x = 0; x < QTDREPOSITOR; x++)
  {
    int *aux = (int *)malloc(sizeof(int));
    *aux = x;

    if (pthread_create(&repositores[x].thread, NULL, &run_Repositore, (void *)aux) != 0)
      perror("Erro a criar");
  }
}

void exit_client()
{
  for (int x = 0; x < QTDCLIENTE; x++)
    if (pthread_join(cliente[x].thread, NULL) != 0)
      perror("Erro ao esperar");
}

void exit_repositore()
{
  for (int x = 0; x < QTDREPOSITOR; x++)
    if (pthread_join(repositores[x].thread, NULL) != 0)
      perror("Erro ao esperar");
}