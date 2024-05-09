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

#ifndef QUANTIDADE_POR_REPOSICAO
#define QUANTIDADE_POR_REPOSICAO 10
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
    else
    {
      printf("O cliente %02d, ", cliente[*idCliente].id);
      printf("comprou o produto %02d, ", produto[idxProduto].idProduto);
      printf("sobrou %02d. (FALHA)\n", produto[idxProduto].qtdProduto);
    }

    pthread_mutex_unlock(&product);
    qtdCompras--;
    sleep(TEMPO_MEDIO_DE_UMA_COMPRA);
  }

  printf("O cliente %2d, arrasta para cima!\n", cliente[*idCliente].id);

  free(idCliente);
  return NULL;
}

void *run_Repositore(void *positionRepostor)
{
  int *idRepositor = (int *)positionRepostor;
  int qtdReposicao = 1000000;

  int idxProduto;
  while (qtdReposicao)
  {
    idxProduto = rand() % QTDPRODUTO;

    pthread_mutex_lock(&product);

    if (produto[idxProduto].qtdProduto < QUANTIDADE_POR_REPOSICAO)
    {
      produto[idxProduto].qtdProduto += QUANTIDADE_POR_REPOSICAO;
      printf("O Repistor nmr %02d, ", repositores[*idRepositor].id);
      printf("repos %02d item ", QUANTIDADE_POR_REPOSICAO);
      printf("do produto %02d. \n", produto[idxProduto].idProduto);

      pthread_mutex_unlock(&product);
      sleep(TEMPO_MEDIO_DE_UMA_REPOSCAO);
      qtdReposicao--;

      continue;
    }

    pthread_mutex_unlock(&product);
  }

  return NULL;
}

void *run_verificarMercado(void *arq)
{
  (void)arq;
  while (1)
  {
    int vazio = 0;
    pthread_mutex_lock(&product);
    for (int x = 0; x < QTDPRODUTO; x++)
    {
      if (produto[x].qtdProduto == 0)
        vazio++;
    }
    pthread_mutex_unlock(&product);
    if (vazio == QTDPRODUTO)
    {
      printf("ESTAMOS SEM PRODUTOS!!!\n");
      sleep(20);
    }
  }
  return NULL;
}


void runThreadCliente()
{
  for (int x = 0; x < QTDCLIENTE; x++)
  {
    int *aux = (int *)malloc(sizeof(int));
    *aux = x;

    if (pthread_create(&cliente[x].thread, NULL, &run_client, (void *)aux) != 0)
      perror("Erro a criar");
  }
}

void runThreadRepositor()
{
  for (int x = 0; x < QTDREPOSITOR; x++)
  {
    int *aux = (int *)malloc(sizeof(int));
    *aux = x;

    if (pthread_create(&repositores[x].thread, NULL, &run_Repositore, (void *)aux) != 0)
      perror("Erro a criar");
  }
}

void runThreadVerificarMercado()
{
  for (int x = 0; x < QTDCLIENTE; x++)
  {
    int *aux = (int *)malloc(sizeof(int));
    *aux = x;

    if (pthread_create(&cliente[x].thread, NULL, &run_client, (void *)aux) != 0)
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