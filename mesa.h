#ifndef MESA_H
#define MESA_H

#define LUGARES_POR_MESA 4

typedef struct mesa {
    int numero;
    int ocupada;       /* 0 = livre, 1 = ocupada */
    int qtd_pessoas;   /* so vale se ocupada */
    int comanda;       /* so vale se ocupada */
} Mesa;

struct restaurante; 

//  operacoes sobre a matriz de mesas
Mesa **mesa_cria_matriz(int linhas, int colunas);
void   mesa_libera_matriz(Mesa **mesas, int linhas);
Mesa  *mesa_busca(Mesa **mesas, int linhas, int colunas, int numero);
Mesa  *mesa_proxima_livre(Mesa **mesas, int linhas, int colunas);
int    mesa_contar_livres(Mesa **mesas, int linhas, int colunas);

// operacoes sobre uma mesa
void mesa_ocupa(Mesa *m, int qtd_pessoas, int comanda);
void mesa_libera(Mesa *m);
void mesa_imprime(const Mesa *m);

// operacoes no nivel do restaurante

int  acomodar_pessoas(struct restaurante *r, int qtd);
void restaurante_imprime_mesa(struct restaurante *r, int numero);
void restaurante_imprime_mesas(struct restaurante *r);

#endif
