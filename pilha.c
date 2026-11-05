#include "pilha.h"
#include <stdio.h>
#include <stdlib.h>

Pilha* pilha_cria(){
    Pilha* pilha = malloc(sizeof(Pilha));
    if(pilha == NULL){
        return NULL;
    }
    pilha->topo = NULL;
    pilha->qtd = 0;

    return pilha;
}

int pilha_vazia(Pilha* p){
    if(p->topo == NULL){
        return 1;
    }else{
        return 0;
    }
}

int pilha_tamanho(Pilha* p){
    return p->qtd;
}

int pilha_push(Pilha* p, int id) {
    Prato* prato_novo;

    prato_novo = (Prato*)malloc(sizeof(Prato));
    if(prato_novo == NULL){
        return 0;
    }

    prato_novo->id = id;
    prato_novo->prox = p->topo;
    p->topo = prato_novo;
    p->qtd++;

    return 1;
}

int pilha_pop(Pilha* p) {
    Prato* prato_removido;

    if(pilha_vazia(p)){
        return 0;
    }

    prato_removido = p->topo;
    p->topo = prato_removido->prox;
    p->qtd--;
    free(prato_removido);

    return 1;
}

void pilha_push_n(Pilha* p, int n, int* prox_id) {
    int empilhou;

    for(int i = 0; i < n; i++){
        empilhou = pilha_push(p, *prox_id);

        if(empilhou == 0){
            return;
        }
        (*prox_id)++;
    }
}

int pilha_pop_n(Pilha* p, int n){
    int removidos = 0;

    while(removidos < n && pilha_pop(p)){
        removidos++;
    }
    return removidos;
    
}

void pilha_imprime(Pilha* p){
    printf("Pratos na pilha: %d\n", p->qtd);
}

void pilha_libera(Pilha* p){
    if(p == NULL){
        return;
    }

    while(pilha_pop(p)){
        free(pilha_pop);
    }
    free(p);
}
