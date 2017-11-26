#include <stdlib.h>
#include <stdio.h>

#include "vertice.h"
#include "lista_enc.h"
#include "no.h"
#include "fila.h"
#include "pilha.h"

#define TRUE 1
#define FALSE 0

#define DEBUG

struct vertices{
    int id;
    char simbolo;
    int freq;

    fila_t* bits;
	//byte_t info;

    vertice_t* pai;
    vertice_t* direita;
    vertice_t* esquerda;

};

vertice_t *cria_vertice(int id){

    vertice_t *p = NULL;
    fila_t* fila;

	p = malloc(sizeof(vertice_t));
	if (p == NULL) {
		perror("cria_vertice:");
		exit(EXIT_FAILURE);
	}

	p->id = id;
	p->simbolo = 0;
	p->freq = 1;
	fila = cria_fila();
	p->bits = fila;
	p->direita = NULL;
	p->esquerda = NULL;
	p->pai = NULL;

	return p;

}

int vertice_get_id(vertice_t *vertice){

	if (vertice == NULL)
	{
		fprintf(stderr, "vertice_obter_id: vertice invalido!\n");
		exit(EXIT_FAILURE);
	}

	return vertice->id;

}


void vertice_set_pai(vertice_t *vertice, vertice_t *pai){

	if (vertice == NULL){
			fprintf(stderr, "vertice_set_pai: vertice invalido\n");
			exit(EXIT_FAILURE);
	}

	vertice->pai = pai;

}

void vertice_set_esq(vertice_t *vertice, vertice_t *esq){

	if (vertice == NULL){
			fprintf(stderr, "vertice_set_esq: vertice invalido\n");
			exit(EXIT_FAILURE);
	}

	vertice->esquerda = esq;

}

void vertice_set_dir(vertice_t *vertice, vertice_t *dir){

	if (vertice == NULL){
			fprintf(stderr, "vertice_set_esq: vertice invalido\n");
			exit(EXIT_FAILURE);
	}

	vertice->direita = dir;

}


vertice_t* vertice_get_esq(vertice_t *vertice){

	if (vertice == NULL){
			fprintf(stderr, "vertice_get_esq: vertice invalido\n");
			exit(EXIT_FAILURE);
	}

	return vertice->esquerda;

}

vertice_t* vertice_get_dir(vertice_t *vertice){

	if (vertice == NULL){
			fprintf(stderr, "vertice_get_dir: vertice invalido\n");
			exit(EXIT_FAILURE);
	}

	return vertice->direita;

}


char vertice_get_simbolo(vertice_t * vertice){

	if (vertice == NULL){
			fprintf(stderr, "vertice_get_simbolo: vertice invalido\n");
			exit(EXIT_FAILURE);
	}

	return vertice->simbolo;

}

void vertice_set_simbolo(vertice_t * vertice, char s){

	if (vertice == NULL){
			fprintf(stderr, "vertice_set_simbolo: vertice invalido\n");
			exit(EXIT_FAILURE);
	}

	vertice->simbolo = s;

}

vertice_t *vertice_get_pai(vertice_t *vertice){

	if (vertice == NULL){
			fprintf(stderr, "vertice_get_pai: vertice invalido\n");
			exit(EXIT_FAILURE);
	}

	return vertice->pai;

}

int vertice_eh_folha(vertice_t *vertice){

	if (vertice == NULL){
		fprintf(stderr, "eh_folha: vertice invalido\n");
		exit(EXIT_FAILURE);
	}

	if (vertice->direita == NULL && vertice->esquerda == NULL)
		return TRUE;

	return FALSE;

}

int vertice_get_freq(vertice_t * vertice){

	if (vertice == NULL){
			fprintf(stderr, "vertice_get_freq: vertice invalido\n");
			exit(EXIT_FAILURE);
	}

	return vertice->freq;

}

void vertice_set_freq(vertice_t* vertice, int freq){

	if (vertice == NULL){
			fprintf(stderr, "vertice_set_freq: vertice invalido\n");
			exit(EXIT_FAILURE);
	}

	vertice->freq = freq;

}

fila_t* vertice_get_bits(vertice_t *vertice){

	if (vertice == NULL)
	{
		fprintf(stderr, "vertice_obter_id: vertice invalido!\n");
		exit(EXIT_FAILURE);
	}

	return vertice->bits;

}

void guardar_bits(pilha_t* pilha, vertice_t* vert){

    int i;
    pilha_t* pilha_2;
    pilha_t* pilha_3;

    pilha_2 = cria_pilha();
    pilha_3 = cria_pilha();

    if (pilha == NULL){
			fprintf(stderr, "guardar_bits: pilha invalida\n");
			exit(EXIT_FAILURE);
	}

    while(!pilha_vazia(pilha)){
        i = pop_int(pilha);
        push_int(i, pilha_2);
        push_int(i, pilha_3);
    }
#ifdef DEBUG
    if(vert->simbolo > 32)
        printf("%d - Bits [%c]: ", vert->id, vert->simbolo);
    else
        printf("%d - Bits [ ]: ", vert->id);
#endif // DEBUG

	while(!pilha_vazia(pilha_2)){

        i = pop_int(pilha_2);
#ifdef DEBUG
        printf("%d", i);
#endif // DEBUGcd
        enqueue_int(i, vert->bits);

    }
#ifdef DEBUG
    printf("\n");
#endif // DEBUG

    while(!pilha_vazia(pilha_3))
        push_int(pop_int(pilha_3), pilha);

    libera_pilha(pilha_2);
    libera_pilha(pilha_3);
}
