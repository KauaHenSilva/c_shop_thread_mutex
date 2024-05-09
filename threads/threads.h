typedef struct produto Produto;

typedef struct cliente Cliente;

typedef struct repositores Repositores;

void *run_client(void *positionCliente);
void *run_Repositore(void *positionRepostor);

void runTreadCliente();
void runTreadRepositor();

void exit_client();
void exit_repositore();