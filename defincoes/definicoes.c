#include "definicoes.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#ifndef QTDPRODUTO
#define QTDPRODUTO 30
#endif

#ifndef QTDCLIENTE
#define QTDCLIENTE 10
#endif

#ifndef QTDREPOSITOR
#define QTDREPOSITOR 10
#endif

#ifndef QUANTIDADE_INICIAL_REPOSICAO
#define QUANTIDADE_INICIAL_REPOSICAO 5
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

Produto *produto;
Cliente *cliente;
Repositores *repositores;

#endif // VARGLOBALPRODUTOCLIENTEREPOSITORIO

int idProduto = 0;
int idCliente = 0;
int idRepositores = 0;

FILE *arqCLientes;
FILE *arqProdutos;
FILE *arqRepositores;

void subtituiCaracter(char *str, char velho, char novo)
{
  for (int x = 0; str[x] != '\0'; x++)
    if (str[x] == velho)
      str[x] = novo;
}

void defProdutos()
{
  arqProdutos = fopen("../produtos.txt", "r");
  if (!arqProdutos)
    perror("Erro ao abrir o arquivo produtos.txt");

  produto = (Produto *)realloc(produto, sizeof(Produto) * QTDPRODUTO);
  for (int x = 0; x < QTDPRODUTO; x++)
  {
    produto[x].idProduto = ++idProduto;
    produto[x].qtdProduto = QUANTIDADE_INICIAL_REPOSICAO;
    produto[x].qtdComprados = 0;
    fscanf(arqProdutos, "%s %f", produto[x].nomeProduto, &produto[x].valor);
    subtituiCaracter(produto[x].nomeProduto, '_', ' ');
    pthread_mutex_init(&produto[x].product, NULL);
  }

  fclose(arqProdutos);
}

void defCliente()
{
  arqCLientes = fopen("../clientes.txt", "r");
  if (!arqCLientes)
    perror("Erro ao abrir o arquivo clientes.txt");

  cliente = (Cliente *)realloc(cliente, sizeof(Cliente) * QTDCLIENTE);
  for (int x = 0; x < QTDCLIENTE; x++)
  {
    cliente[x].id = ++idCliente;
    fscanf(arqCLientes, "%s", cliente[x].nome);
    subtituiCaracter(cliente[x].nome, '_', ' ');
  }
  fclose(arqCLientes);
}

void defRepositores()
{
  arqRepositores = fopen("../repositores.txt", "r");
  if (!arqRepositores)
    perror("Erro ao abrir o arquivo repositores.txt");

  repositores = (Repositores *)realloc(repositores, sizeof(Repositores) * QTDREPOSITOR);
  for (int x = 0; x < QTDREPOSITOR; x++)
  {
    repositores[x].id = ++idRepositores;
    fscanf(arqRepositores, "%s", repositores[x].nome);
    subtituiCaracter(repositores[x].nome, '_', ' ');
  }
}

void freeProdutos()
{
  for (int x = 0; x < QTDPRODUTO; x++)
    pthread_mutex_destroy(&produto[x].product);
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