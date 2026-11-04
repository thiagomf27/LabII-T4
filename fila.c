#include "fila.h"
#include <stdio.h>
#include <stdlib.h>

Fila* fila_cria() {
    Fila* fila = malloc(sizeof(Fila));
    if (fila == NULL) return NULL;

    fila->inicio = NULL;
    fila->fim = NULL;
    fila->qtd_grupos = 0;
    fila->qtd_pessoas = 0;

    return fila;
}   

int fila_vazia(Fila* f) {
    return f->inicio == NULL ? 1 : 0;
}

int fila_insere(Fila* f, int senha, int qtd) {
    Grupo* grupo_novo = malloc(sizeof(Grupo));
    if (grupo_novo == NULL) return 0;
    grupo_novo->qtd_pessoas = qtd;
    grupo_novo->senha = senha;
    grupo_novo->prox = NULL;

    if (f->inicio == NULL) f->inicio = grupo_novo;
    else f->fim->prox = grupo_novo;

    f->fim = grupo_novo;
    f->qtd_grupos++;
    f->qtd_pessoas += qtd;

    return 1;
}

int fila_remove(Fila* f, int* senha, int* qtd) {
    if (fila_vazia(f)) return 0;

    Grupo* grupo = f->inicio;
    f->inicio = grupo->prox;
    if (grupo == f->fim) f->fim = f->inicio;
    *senha = grupo->senha;
    *qtd = grupo->qtd_pessoas;

    f->qtd_grupos--;
    f->qtd_pessoas -= grupo->qtd_pessoas;

    free(grupo);
    return 1;
}

Grupo* fila_primeiro(Fila* f) {
    return f->inicio;
}   

void fila_atualiza_primeiro(Fila* f, int sentaram) {
    if (fila_vazia(f)) return;
    Grupo* grupo = f->inicio;

    if (sentaram > grupo->qtd_pessoas) sentaram = grupo->qtd_pessoas; //max de pessoas que podem sentar é a qtd restante

    grupo->qtd_pessoas -= sentaram;
    f->qtd_pessoas -= sentaram;
    int s = grupo->senha;
    int p = grupo->qtd_pessoas;

    if (grupo->qtd_pessoas < 1) fila_remove(f, &s, &p);
}

int fila_remove_senha(Fila* f, int senha) {
    Fila* f_aux = fila_cria();
    int encontrou = 0;

    while (!fila_vazia(f)) {
        int s = 0;
        int p = 0;
        fila_remove(f, &s, &p);
        if (s == senha) encontrou = 1;
        else fila_insere(f_aux, s, p);
    }

    while (!fila_vazia(f_aux)) {
        int s = 0;
        int p = 0;
        fila_remove(f_aux, &s, &p);
        fila_insere(f, s, p);
    }

    free(f_aux);
    return encontrou;
}

void fila_imprime(Fila* f) {
    Grupo* grupo = f->inicio;

    if (fila_vazia(f)) {
        printf("A fila está vazia\n");
        return;
    }

    printf("%d grupos aguardam na fila, totalizando %d pessoas:\n", f->qtd_grupos, f->qtd_pessoas);
    while (grupo != NULL) {
        printf("Grupo %d aguarda por %d lugares\n", grupo->senha, grupo->qtd_pessoas);
        grupo = grupo->prox;
    }
}

void fila_libera(Fila* f) {
    if (f == NULL) return;
    Grupo* grupo = f->inicio;
    Grupo* anterior = NULL;

    while (grupo != NULL) {
        anterior = grupo;
        grupo = grupo->prox;
        free(anterior);
    }

    free(f);
}