#ifndef FILA_H
#define FILA_H

typedef struct grupo Grupo;
typedef struct fila Fila;

typedef struct grupo {           
    int senha;
    int qtd_pessoas;
    Grupo* prox;
} Grupo;

typedef struct fila {             
    Grupo* inicio;
    Grupo* fim;
    int qtd_grupos;
    int qtd_pessoas;
} Fila;

Fila* fila_cria();
int fila_vazia(Fila* f);
int fila_insere(Fila* f, int senha, int qtd);
int fila_remove(Fila* f, int* senha, int* qtd);
Grupo* fila_primeiro(Fila* f);
void fila_atualiza_primeiro(Fila* f, int sentaram);
int fila_remove_senha(Fila* f, int senha);   
void fila_imprime(Fila* f);
void fila_libera(Fila* f);

#endif