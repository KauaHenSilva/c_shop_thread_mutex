typedef struct produto Produto;

typedef struct cliente Cliente;

typedef struct repositores Repositores;

void *run_client(void *positionCliente);
void *run_Repositore(void *positionRepostor);
void *run_verificarMercado(void *arq);

void runThreadCliente();
void runThreadRepositor();
void runThreadVerificarMercado();

void finalizamentoThreads();