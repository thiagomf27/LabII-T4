#ifndef PILHA_H
#define PILHA_H

#define PRATOS_POR_MESA 4

typedef struct prato Prato;
typedef struct pilha Pilha;

struct prato {
    int id;
    Prato* prox;
};

struct pilha {
    Prato* topo;
    int qtd;
};

Pilha* pilha_cria();
int pilha_vazia(Pilha* p);
int pilha_tamanho(Pilha* p);
int pilha_push(Pilha* p, int id);
int pilha_pop(Pilha* p);
void pilha_push_n(Pilha* p, int n, int* prox_id);
int pilha_pop_n(Pilha* p, int n);
void pilha_imprime(Pilha* p);
void pilha_libera(Pilha* p);

#endif