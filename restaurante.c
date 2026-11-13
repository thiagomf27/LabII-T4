#include "restaurante.h"
#include <stdio.h>
#include <stdlib.h>

// tira da pilha os pratos que faltam para a mesa ficar com 4
// so arruma se houver pratos suficientes, 1 se a mesa ficou completa
static int arrumar_mesa(Restaurante* r, Mesa* mesa) {
    int faltam = PRATOS_POR_MESA - mesa->qtd_pratos;
    if (faltam <= 0) return 1;
    if (pilha_tamanho(r->pratos) < faltam) return 0;

    mesa->qtd_pratos += pilha_pop_n(r->pratos, faltam);
    return 1;
}

// arruma as mesas livres que ficaram sem pratos
static void arrumar_mesas_livres(Restaurante* r) {
    for (int i = 0; i < r->linhas; i++) {
        for (int j = 0; j < r->colunas; j++) {
            Mesa* mesa = &r->mesas[i][j];
            if (!mesa->ocupada && mesa->qtd_pratos < PRATOS_POR_MESA) arrumar_mesa(r, mesa);
        }
    }
}

// libera mesas, fila e pilha, mas mantem a struct Restaurante
static void restaurante_limpa(Restaurante* r) {
    pilha_libera(r->pratos);
    fila_libera(r->espera);
    mesa_libera_matriz(r->mesas, r->linhas);

    r->mesas = NULL;
    r->pratos = NULL;
    r->espera = NULL;
    r->linhas = 0;
    r->colunas = 0;
    r->aberto = 0;
}

Restaurante* restaurante_cria() {
    Restaurante* r = malloc(sizeof(Restaurante));
    if (r == NULL) return NULL;

    r->mesas = NULL;
    r->linhas = 0;
    r->colunas = 0;
    r->pratos = NULL;
    r->espera = NULL;
    r->prox_senha = 1;
    r->prox_comanda = 1;
    r->prox_id_prato = 1;
    r->aberto = 0;

    return r;
}

int restaurante_abrir(Restaurante* r, int lin, int col) {
    if (r == NULL) return 0;
    restaurante_limpa(r);   // abrir de novo descarta tudo que existia antes

    Mesa** mesas = mesa_cria_matriz(lin, col);   // ja cria as mesas arrumadas com 4 pratos
    Fila* f = fila_cria();
    Pilha* p = pilha_cria();                      // pilha comeca vazia

    if (mesas == NULL || f == NULL || p == NULL) {
        mesa_libera_matriz(mesas, lin);
        fila_libera(f);
        pilha_libera(p);
        return 0;
    }

    r->mesas = mesas;
    r->linhas = lin;
    r->colunas = col;
    r->espera = f;
    r->pratos = p;
    r->prox_senha = 1;
    r->prox_comanda = 1;
    r->prox_id_prato = 1;
    r->aberto = 1;

    return 1;
}

int acomodar_pessoas(Restaurante* r, int pessoas) {
    int sentaram = 0;

    while (sentaram < pessoas) {
        Mesa* mesa = mesa_proxima_livre(r->mesas, r->linhas, r->colunas);
        if (mesa == NULL) break;   // nenhuma mesa livre e arrumada

        int nesta = pessoas - sentaram;
        if (nesta > LUGARES_POR_MESA) nesta = LUGARES_POR_MESA;

        mesa_ocupa(mesa, nesta, r->prox_comanda++);

        // pratos excedentes voltam para a pilha
        pilha_push_n(r->pratos, mesa->qtd_pratos - nesta, &(r->prox_id_prato));
        mesa->qtd_pratos = nesta;

        sentaram += nesta;
    }

    return sentaram;
}

int restaurante_chegada(Restaurante* r, int pessoas) {
    int sentaram = 0;

    // se ja tem gente na fila, o grupo novo nao fura a fila
    if (fila_vazia(r->espera)) sentaram = acomodar_pessoas(r, pessoas);
    if (sentaram == pessoas) return 0;

    int senha = r->prox_senha++;
    if (!fila_insere(r->espera, senha, pessoas - sentaram)) return -1;

    return senha;
}

int restaurante_liberar_mesa(Restaurante* r, int numero) {
    Mesa* mesa = mesa_busca(r->mesas, r->linhas, r->colunas, numero);
    if (mesa == NULL || mesa->ocupada == 0) return 0;

    mesa_libera(mesa);
    arrumar_mesa(r, mesa);
    chamar_fila(r);

    return 1;
}

void chamar_fila(Restaurante* r) {
    while (!fila_vazia(r->espera)) {
        Grupo* grupo = fila_primeiro(r->espera);
        int senha = grupo->senha;
        int esperando = grupo->qtd_pessoas;

        int sentaram = acomodar_pessoas(r, esperando);
        if (sentaram == 0) return;   // sem mesa livre e arrumada

        fila_atualiza_primeiro(r->espera, sentaram);   // remove o grupo se todos sentaram

        if (sentaram < esperando)
            printf("Grupo de senha %d: %d pessoa(s) sentaram, %d continuam na fila.\n", senha, sentaram, esperando - sentaram);
        else
            printf("Grupo de senha %d: %d pessoa(s) sentaram e o grupo saiu da fila.\n", senha, sentaram);
    }
}

int restaurante_desistir(Restaurante* r, int senha) {
    if (fila_remove_senha(r->espera, senha)) return 1;
    return 0;
}

void restaurante_repor_pratos(Restaurante* r, int qtd) {
    pilha_push_n(r->pratos, qtd, &(r->prox_id_prato));
    arrumar_mesas_livres(r);
    chamar_fila(r);
}

void restaurante_imprime_fila(Restaurante* r) {
    fila_imprime(r->espera);
}

void restaurante_imprime_pilha(Restaurante* r) {
    pilha_imprime(r->pratos);
}

void restaurante_libera(Restaurante* r) {
    if (r == NULL) return;
    restaurante_limpa(r);
    free(r);
}
