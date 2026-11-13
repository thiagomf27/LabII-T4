#include <stdio.h>
#include <stdlib.h>
#include "mesa.h"
#include "restaurante.h"

Mesa **mesa_cria_matriz(int linhas, int colunas){
    if (linhas <= 0 || colunas <= 0) return NULL;

    Mesa **mesas = (Mesa **)malloc(linhas * sizeof(Mesa *));
    if (mesas == NULL) return NULL;

    for (int i = 0; i < linhas; i++) {
        mesas[i] = (Mesa *)malloc(colunas * sizeof(Mesa));
        if (mesas[i] == NULL) {
            mesa_libera_matriz(mesas, i);
            return NULL;
        }
        for (int j = 0; j < colunas; j++) {
            mesas[i][j].numero      = i * colunas + j + 1;
            mesas[i][j].ocupada     = 0;
            mesas[i][j].qtd_pessoas = 0;
            mesas[i][j].comanda     = 0;
            mesas[i][j].qtd_pratos  = 4;
        }
    }
    return mesas;
}

void mesa_libera_matriz(Mesa **mesas, int linhas){
    if (mesas == NULL) return;
    for (int i = 0; i < linhas; i++)
        free(mesas[i]);
    free(mesas);
    return;
}

Mesa *mesa_busca(Mesa **mesas, int linhas, int colunas, int numero){
    if (mesas == NULL || numero < 1 || numero > linhas * colunas)
        return NULL;
    int i = (numero - 1) / colunas;
    int j = (numero - 1) % colunas;
    return &mesas[i][j];
}

//so devolve mesa livre e arrumada (com os 4 pratos)
Mesa *mesa_proxima_livre(Mesa **mesas, int linhas, int colunas){
    for (int i = 0; i < linhas; i++)
        for (int j = 0; j < colunas; j++)
            if (!mesas[i][j].ocupada && mesas[i][j].qtd_pratos == LUGARES_POR_MESA)
                return &mesas[i][j];
    return NULL;
}

int mesa_contar_livres(Mesa **mesas, int linhas, int colunas){
    int livres = 0;
    for (int i = 0; i < linhas; i++)
        for (int j = 0; j < colunas; j++)
            if (!mesas[i][j].ocupada)
                livres++;
    return livres;
}

void mesa_ocupa(Mesa *m, int qtd_pessoas, int comanda){
    m->ocupada     = 1;
    m->qtd_pessoas = qtd_pessoas;
    m->comanda     = comanda;
    return;
}

void mesa_libera(Mesa *m){
    m->ocupada     = 0;
    m->qtd_pessoas = 0;
    m->comanda     = 0;
    m->qtd_pratos  = 0;   //pratos usados saem da mesa, ela precisa ser arrumada de novo
    return;
}

void mesa_imprime(const Mesa *m){
    if (m->ocupada)
        printf("Mesa %2d: OCUPADA | %d pessoa(s) | comanda %d | %d prato(s)\n",
               m->numero, m->qtd_pessoas, m->comanda, m->qtd_pratos);
    else if (m->qtd_pratos < LUGARES_POR_MESA)
        printf("Mesa %2d: livre, aguardando pratos (%d prato(s))\n", m->numero, m->qtd_pratos);
    else
        printf("Mesa %2d: livre | %d prato(s)\n", m->numero, m->qtd_pratos);
    return;
}

void restaurante_imprime_mesa(Restaurante *r, int numero){
    Mesa *m = mesa_busca(r->mesas, r->linhas, r->colunas, numero);
    if (m == NULL) {
        printf("Mesa %d nao existe (validas: 1 a %d).\n", numero, r->linhas * r->colunas);
        return;
    }
    mesa_imprime(m);
    return;
}

void restaurante_imprime_mesas(Restaurante *r){
    int total  = r->linhas * r->colunas;
    int livres = mesa_contar_livres(r->mesas, r->linhas, r->colunas);

    printf("\n=== Ocupacao das mesas (%d livres / %d ocupadas) ===\n", livres, total - livres);
    for (int i = 0; i < r->linhas; i++) {
        for (int j = 0; j < r->colunas; j++) {
            Mesa *m = &r->mesas[i][j];
            if (m->ocupada)
                printf("[M%-2d: %dp C%-3d] ", m->numero, m->qtd_pessoas, m->comanda);
            else if (m->qtd_pratos < LUGARES_POR_MESA)
                printf("[M%-2d: s/prato ] ", m->numero);
            else
                printf("[M%-2d: livre   ] ", m->numero);
        }
        printf("\n");
    }
    printf("(p = pessoas, C = comanda, s/prato = livre aguardando reposicao de pratos)\n");
    return;
}
