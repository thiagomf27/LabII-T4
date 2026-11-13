#ifndef RESTAURANTE_H
#define RESTAURANTE_H

#include "mesa.h"
#include "pilha.h"
#include "fila.h"

typedef struct restaurante Restaurante;

struct restaurante {
    Mesa** mesas;
    int linhas;
    int colunas;
    Pilha* pratos;
    Fila* espera;
    int prox_senha;
    int prox_comanda;
    int prox_id_prato;
    int aberto;
};

Restaurante* restaurante_cria();                                // aloca com tudo NULL/0
int restaurante_abrir(Restaurante* r, int lin, int col);        // opcao 1, 1 se ok, 0 se falhar
int restaurante_chegada(Restaurante* r, int pessoas);           // opcao 2, retorna a senha ou 0 se todos sentaram
int acomodar_pessoas(Restaurante* r, int pessoas);              // retorna quantos sentaram
int restaurante_liberar_mesa(Restaurante* r, int numero);       // opcao 3, 1 se ok, 0 se mesa invalida ou ja livre
void chamar_fila(Restaurante* r);                               // senta os grupos da fila enquanto houver mesa livre
int restaurante_desistir(Restaurante* r, int senha);            // opcao 4, 1 se removeu, 0 se senha nao existe
void restaurante_repor_pratos(Restaurante* r, int qtd);         // opcao 5
void restaurante_imprime_mesa(Restaurante* r, int numero);      // opcao 6.1, uma mesa
void restaurante_imprime_mesas(Restaurante* r);                 // opcao 6.1, todas
void restaurante_imprime_fila(Restaurante* r);                  // opcao 6.2
void restaurante_imprime_pilha(Restaurante* r);                 // opcao 6.3
void restaurante_libera(Restaurante* r);                        // libera tudo ao sair

#endif