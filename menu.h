#ifndef MENU_H
#define MENU_H

#include "restaurante.h"

int  ler_inteiro(const char *mensagem, int min, int max);
void menu_executa();
void menu_exibe();

int menu_le_opcao();
void menu_exibe_imprimir();
int menu_le_opcao_imprimir();
void menu_chegar(Restaurante *r);
void menu_finalizar(Restaurante *r);
void menu_desistir(Restaurante *r);
void menu_imprimir(Restaurante *r);
void menu_repor(Restaurante *r);

Restaurante *menu_abrir(Restaurante *r);

#endif
