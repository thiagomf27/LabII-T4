#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include "menu.h"
#include "mesa.h"
#include "restaurante.h"

enum {
    OP_SAIR = 0,
    OP_ABRIR,        /* 1 */
    OP_CHEGAR,       /* 2 */
    OP_FINALIZAR,    /* 3 */
    OP_DESISTIR,     /* 4 */
    OP_REPOR,        /* 5 */
    OP_IMPRIMIR      /* 6 */
};

enum {
    IMP_VOLTAR = 0,
    IMP_MESA,
    IMP_MESAS,
    IMP_FILA,
    IMP_PILHA
};

int ler_inteiro(const char *mensagem, int min, int max)
{
    int valor;
    int lido;
    int c;

    while (1) {
        printf("%s", mensagem);
        lido = scanf("%d", &valor);

        if (lido == EOF) {
            return min;
        }
        //limpa o resto da linha (evita loop infinito se digitar letra)
        while ((c = getchar()) != '\n' && c != EOF);

        if (lido != 1) {
            printf("Entrada invalida! Digite um numero.\n");
        }
        else if (valor >= min && valor <= max) {
            return valor;
        }
        else {
            printf("Valor invalido! Digite um numero entre %d e %d.\n", min, max);
        }
    }
}

//menus
void menu_exibe(){
    printf("\n=========== RESTAURANTE ===========\n");
    printf(" 1) Abrir restaurante\n");
    printf(" 2) Chegada de clientes\n");
    printf(" 3) Finalizar refeicao / liberar mesa\n");
    printf(" 4) Desistir de esperar (sair da fila)\n");
    printf(" 5) Repor pratos\n");
    printf(" 6) Imprimir (mesas, fila, pilha de pratos)\n");
    printf(" 0) Sair\n");
    printf("===================================\n");
}

void menu_exibe_imprimir(){
    printf("\n--- Imprimir ---\n");
    printf(" 1) Uma mesa (pelo numero)\n");
    printf(" 2) Todas as mesas\n");
    printf(" 3) Fila de espera\n");
    printf(" 4) Pilha de pratos\n");
    printf(" 0) Voltar\n");
}

int menu_le_opcao(){
    return ler_inteiro("Opcao: ", OP_SAIR, OP_IMPRIMIR);
}

int menu_le_opcao_imprimir(){
    return ler_inteiro("Opcao: ", IMP_VOLTAR, IMP_PILHA);
}

Restaurante *menu_abrir(Restaurante *r){
    if (r != NULL && r->aberto) {
        printf("O restaurante ja esta aberto.\n");
        int reabrir = ler_inteiro("Reabrir e descartar mesas, fila e pratos? (1 = sim, 0 = nao): ", 0, 1);
        if (!reabrir)
            return r;
    }
    int linhas  = ler_inteiro("Numero de linhas de mesas: ", 1, 1000);
    int colunas = ler_inteiro("Numero de colunas de mesas: ", 1, 1000);

    if (r == NULL)
        r = restaurante_cria();

    if (r == NULL || !restaurante_abrir(r, linhas, colunas))
        printf("Erro de memória\n");
    else
        printf("Restaurante aberto com %d mesas (%dx%d).\n", linhas * colunas, linhas, colunas);
    return r;
}

void menu_chegar(Restaurante *r){
    int qtd = ler_inteiro("Quantidade de pessoas do grupo: ", 1, INT_MAX);
    int senha = restaurante_chegada(r, qtd);

    if (senha == 0)
        printf("Todas as %d pessoa(s) do grupo sentaram.\n", qtd);
    else if (senha < 0)
        printf("Erro de memória\n");
    else {
        int esperando = r->espera->fim->qtd_pessoas;
        printf("%d pessoa(s) sentaram. Grupo recebeu a senha %d e %d pessoa(s) aguardam na fila.\n",
               qtd - esperando, senha, esperando);
    }
}

void menu_finalizar(Restaurante *r){
    int num = ler_inteiro("Numero da mesa a liberar: ", 1, r->linhas * r->colunas);
    Mesa *m = mesa_busca(r->mesas, r->linhas, r->colunas, num);

    if (!m->ocupada) {
        printf("A mesa %d ja esta livre.\n", num);
        return;
    }
    printf("Mesa %d liberada.\n", num);
    restaurante_liberar_mesa(r, num);

    if (!m->ocupada && m->qtd_pratos < LUGARES_POR_MESA)
        printf("Pratos insuficientes na pilha: a mesa %d aguarda reposicao para ser arrumada.\n", num);
}

void menu_desistir(Restaurante *r){
    int senha = ler_inteiro("Senha do grupo que desistiu: ", 1, INT_MAX);

    if (restaurante_desistir(r, senha))
        printf("Grupo de senha %d saiu da fila.\n", senha);
    else
        printf("Nao existe grupo com a senha %d na fila.\n", senha);
}

void menu_repor(Restaurante *r){
    int qtd = ler_inteiro("Quantidade de pratos a repor: ", 1, INT_MAX);
    restaurante_repor_pratos(r, qtd);
    printf("%d prato(s) repostos. ", qtd);
    restaurante_imprime_pilha(r);
}

void menu_imprimir(Restaurante *r){
    int sub;
    do {
        menu_exibe_imprimir();
        sub = menu_le_opcao_imprimir();

        switch (sub) {
        case IMP_MESA: {
            int num = ler_inteiro("Numero da mesa: ", 1, r->linhas * r->colunas);
            restaurante_imprime_mesa(r, num);
            break;
        }
        case IMP_MESAS: restaurante_imprime_mesas(r);
            break;
        case IMP_FILA:  restaurante_imprime_fila(r);   
            break;
        case IMP_PILHA: restaurante_imprime_pilha(r);
            break;
        default:
            break;
        }
    } while (sub != IMP_VOLTAR);
}

void menu_executa(){
    Restaurante *r = NULL;
    int opcao;

    do {
        menu_exibe();
        opcao = menu_le_opcao();
        if ((r == NULL || !r->aberto) && opcao != OP_ABRIR && opcao != OP_SAIR) {
            printf("Abra o restaurante primeiro (opcao 1).\n");
            continue;
        }

        switch (opcao) {
        case OP_ABRIR:     r = menu_abrir(r);
            break;
        case OP_CHEGAR:    menu_chegar(r);
            break;
        case OP_FINALIZAR: menu_finalizar(r);
            break;
        case OP_DESISTIR:  menu_desistir(r);
            break;
        case OP_REPOR:     menu_repor(r);
            break;
        case OP_IMPRIMIR:  menu_imprimir(r);
            break;
        case OP_SAIR:      printf("Encerrando...\n");
            break;
        }
    } while (opcao != OP_SAIR);

    if (r != NULL)
        restaurante_libera(r);
}
