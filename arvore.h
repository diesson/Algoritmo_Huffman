#ifndef SRC_ARVORE_ARVORE_H_
#define SRC_ARVORE_ARVORE_H_

#include "vertice.h"
#include "fila.h"
#include "pilha.h"

typedef struct arvores arvore_t;

// Cria��o e edi��o da �rvore
arvore_t* cria_arvore(int id);
vertice_t* arvore_get_raiz (arvore_t *arvore);
void arvore_set_raiz (arvore_t *arvore, vertice_t* vertice);
lista_enc_t *arvore_obter_vertices (arvore_t *arvore);

vertice_t* arvore_adicionar_vertice_id(arvore_t *arvore, int id);
vertice_t* arvore_adicionar_vertice(arvore_t *arvore, vertice_t *vertice);

// Fun��es para percorrer a �rvore
vertice_t* arvore_procura_vertice(arvore_t *arvore, int id);
vertice_t* arvore_procura_simbolo(arvore_t* arvore, char s);
void varrer_arvore(vertice_t* vertice, int bit, pilha_t* pilha);

// Exportar a �rvore
void arvore_exportar_grafo_dot(const char *filename, arvore_t *grafo);
void exporta_arvore(arvore_t* arvore, FILE* file_out);
void imprimi_freq(arvore_t* arvore);

 // Liberar �rvore
void libera_arvore(arvore_t *arvore);


#endif /* SRC_ARVORE_ARVORE_H_ */
