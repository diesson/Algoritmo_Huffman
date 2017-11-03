#ifndef GRAFO_VERTICE_H_
#define GRAFO_VERTICE_H_

#include "lista_enc.h"

typedef struct vertices vertice_t;
typedef unsigned char byte;

typedef enum status_aresta { VAZIO, EXPORTADA } status_aresta_t; // ?

vertice_t* cria_vertice(int id);

int vertice_get_id(vertice_t* vertice);

void vertice_set_pai(vertice_t* vertice, vertice_t* pai);
void vertice_set_esq(vertice_t* vertice, vertice_t* esq);
void vertice_set_dir(vertice_t* vertice, vertice_t* dir);
void vertice_set_simbolo(vertice_t* vertice, char s);

vertice_t* vertice_get_esq(vertice_t* vertice);
vertice_t* vertice_get_dir(vertice_t* vertice);
vertice_t* vertice_get_pai(vertice_t* vertice);
char vertice_get_simbolo(vertice_t* vertice);

int vertice_eh_folha(vertice_t* vertice);

int vertice_get_freq(vertice_t * vertice);
void vertice_set_freq(vertice_t * vertice, int freq);

#endif /* GRAFO_VERTICE_H_ */
