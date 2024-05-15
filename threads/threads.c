#include "threads.h"
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

pthread_mutex_t print = PTHREAD_MUTEX_INITIALIZER;

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
  char nomeProduto[50];
  float valor;
  int qtdComprados;
  pthread_mutex_t product;
};

struct cliente
{
  pthread_t thread;
  char nome[50];
  int id;
};

struct repositores
{
  pthread_t thread;
  char nome[50];
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
  int qtdCompras = rand() % 10 + 10;

  while (qtdCompras)
  {
    idxProduto = rand() % QTDPRODUTO;

    pthread_mutex_lock(&produto[idxProduto].product);

    if (produto[idxProduto].qtdProduto > 0)
    {
      produto[idxProduto].qtdProduto--;

      printf("%s, comprou o produto %s, sobrou %02d itens. \n",
             cliente[*idCliente].nome,
             produto[idxProduto].nomeProduto,
             produto[idxProduto].qtdProduto);

      produto[idxProduto].qtdComprados++;
    }
#ifdef EXIBIR_COMPRAS_FALHAS
    else
    {
      printf("%s, comprou o produto %s, sobrou %02d. (Falha em comprar, Nao possuia na platileira)\n",
             cliente[*idCliente].nome,
             produto[idxProduto].nomeProduto,
             produto[idxProduto].qtdProduto);
    }
#endif

    pthread_mutex_unlock(&produto[idxProduto].product);
    qtdCompras--;
    sleep(TEMPO_MEDIO_DE_UMA_COMPRA);
  }

  printf("%s foi de arrasta para cima!\n", cliente[*idCliente].nome);

  free(idCliente);
  return NULL;
}

void *run_Repositore(void *positionRepostor)
{
  int *idRepositor = (int *)positionRepostor;
  int posicao;

  while (1)
  {
    posicao = rand() % QTDPRODUTO;
    pthread_mutex_lock(&produto[posicao].product);

    if (produto[posicao].qtdProduto < QUANTIDADE_POR_REPOSICAO)
    {
      produto[posicao].qtdProduto += QUANTIDADE_POR_REPOSICAO;

      printf("O Repistor %s, repos %02d itens do produto %s, agora tem %02d. \n",
             repositores[*idRepositor].nome,
             QUANTIDADE_POR_REPOSICAO,
             produto[posicao].nomeProduto,
             produto[posicao].qtdProduto);
    }

    pthread_mutex_unlock(&produto[posicao].product);
    sleep(TEMPO_MEDIO_DE_UMA_REPOSCAO);
  }

  return NULL;
}

void *run_verificarMercado(void *arg)
{
  (void)arg;
  while (1)
  {
    int qtdProduto = 0;

    for (int x = 0; x < QTDPRODUTO; x++)
    {
      pthread_mutex_lock(&produto[x].product);
      if (produto[x].qtdProduto > 0)
        qtdProduto++;
      pthread_mutex_unlock(&produto[x].product);
    }

    if (qtdProduto == 0)
    {
      printf("ESTAMOS SEM PRODUTOS!!!\n");
    }
    else
    {
      printf("Possuimos %d produtos nas pratileiras!\n", qtdProduto);
    }

    sleep(20);
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
  pthread_t thread;
  if (pthread_create(&thread, NULL, &run_verificarMercado, NULL) != 0)
    perror("Erro a criar");
}

void finalizamentoThreads()
{
  for (int x = 0; x < QTDCLIENTE; x++)
    pthread_join(cliente[x].thread, NULL);

  for (int x = 0; x < QTDREPOSITOR; x++)
    pthread_cancel(repositores[x].thread);
}
