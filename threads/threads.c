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
  pthread_mutex_t product;
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

void mutexPrintf(const char *format, ...)
{
    // Bloqueia o mutex antes de imprimir
    pthread_mutex_lock(&print);
    
    // Usa a mesma sintaxe de argumentos variáveis como printf
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    
    // Libera o mutex após a impressão
    pthread_mutex_unlock(&print);
}
void *run_client(void *positionCliente)
{
  int *idCliente = (int *)positionCliente;

  int idxProduto;
  int qtdCompras = rand() % 10 + 10;

  while (qtdCompras)
  {
    sleep(TEMPO_MEDIO_DE_UMA_COMPRA);
    idxProduto = rand() % QTDPRODUTO;

    pthread_mutex_lock(&produto[idxProduto].product);

    if (produto[idxProduto].qtdProduto > 0)
    {
      produto[idxProduto].qtdProduto--;
      mutexPrintf("O cliente %02d, ", cliente[*idCliente].id);
      mutexPrintf("comprou o produto %02d, ", produto[idxProduto].idProduto);
      mutexPrintf("sobrou %02d. \n", produto[idxProduto].qtdProduto);
    }
#ifdef EXIBIR_COMPRAS_FALHAS
    else
    {
      mutexPrintf("O cliente %02d, ", cliente[*idCliente].id);
      mutexPrintf("comprou o produto %02d, ", produto[idxProduto].idProduto);
      mutexPrintf("sobrou %02d. (Falha em comprar, Nao possuia na platileira)\n", produto[idxProduto].qtdProduto);
    }
#endif

    pthread_mutex_unlock(&produto[idxProduto].product);
    qtdCompras--;
  }

  mutexPrintf("O cliente %2d, arrasta para cima!\n", cliente[*idCliente].id);

  free(idCliente);
  return NULL;
}

void *run_Repositore(void *positionRepostor)
{
  int *idRepositor = (int *)positionRepostor;
  int qtdReposicao = rand() % 1000000 + 10;

  int posicao;

  while (qtdReposicao)
  {
    posicao = rand() % QTDPRODUTO;
    pthread_mutex_lock(&produto[posicao].product);

    if (produto[posicao].qtdProduto < QUANTIDADE_POR_REPOSICAO)
    {
      sleep(TEMPO_MEDIO_DE_UMA_REPOSCAO);
      produto[posicao].qtdProduto += QUANTIDADE_POR_REPOSICAO;

      mutexPrintf("O Repistor nmr %02d, ", repositores[*idRepositor].id);
      mutexPrintf("repos %02d item ", QUANTIDADE_POR_REPOSICAO);
      mutexPrintf("do produto %02d,", produto[posicao].idProduto);
      mutexPrintf("agora tem %02d. \n", produto[posicao].qtdProduto);
    }

    pthread_mutex_unlock(&produto[posicao].product);
    --qtdReposicao;
  }

  mutexPrintf("O Repistor nmr %02d, arrasta para cima!\n", repositores[*idRepositor].id);

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
      mutexPrintf("ESTAMOS SEM PRODUTOS!!!\n");
    }
    else
    {
      mutexPrintf("Possuimos %d produtos nas pratileiras!\n", qtdProduto);
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

void aguardarThreads()
{
  for (int x = 0; x < QTDCLIENTE; x++)
    pthread_join(cliente[x].thread, NULL);

  for (int x = 0; x < QTDREPOSITOR; x++)
    pthread_join(repositores[x].thread, NULL);
}