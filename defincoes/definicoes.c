#include "definicoes.h"
#include <pthread.h>

#ifndef QTDPRODUTO
#define QTDPRODUTO 30
#endif

#ifndef QTDCLIENTE
#define QTDCLIENTE 10
#endif

#ifndef QTDREPOSITORIO
#define QTDREPOSITORIO 10
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

int idProduto = 0;
int idCliente = 0;
int idRepositores = 0;

void defProdutos()
{
  produto = (Produto *)realloc(produto, sizeof(Produto) * QTDPRODUTO);
  for (int x = 0; x < QTDPRODUTO; x++)
  {
    produto[x].idProduto = ++idProduto;
    produto[x].qtdProduto = rand() % QTDPRODUTO + 1;
  }
}

void defCliente()
{
  cliente = (Cliente *)realloc(cliente, sizeof(Cliente) * QTDCLIENTE);
  for (int x = 0; x < QTDCLIENTE; x++)
    cliente[x].id = ++idCliente;
}

void defRepositores()
{
repositores = (Repositores *)realloc(repositores, sizeof(Repositores) * QTDREPOSITORIO);
  for (int x = 0; x < QTDREPOSITORIO; x++)
    repositores[x].id = ++idRepositores;
}

void freeProdutos()
{
  free(produto);
}

void freeCliente()
{
  free(cliente);
}

void freeRepositores()
{
  free(repositores);
}