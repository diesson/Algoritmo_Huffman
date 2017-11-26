#include <stdio.h>
#include <stdlib.h>

#include "vertice.h"
#include "arvore.h"
#include "lista_enc.h"
#include "no.h"
#include "pilha.h"
//#include "fila.h"

#define FALSE 0
#define TRUE 1

struct arvores{
	int id;
	vertice_t *raiz;
	lista_enc_t *vertices;
};

arvore_t *cria_arvore(int id){

	arvore_t* p = NULL;

	p = (arvore_t*) malloc(sizeof(arvore_t));
	if (p == NULL)	{
		perror("cria_grafo:");
		exit(EXIT_FAILURE);
	}

	p->id = id;
	p->vertices = cria_lista_enc();

	return p;

}

vertice_t* arvore_adicionar_vertice_id(arvore_t *arvore, int id){

	vertice_t *vertice;
	no_t *no;

	if (arvore == NULL)	{
			fprintf(stderr,"grafo_adicionar_vertice: grafo invalido!");
			exit(EXIT_FAILURE);
	}

	if (arvore_procura_vertice(arvore, id) != NULL) {
		fprintf(stderr,"grafo_adicionar_vertice: vertice duplicado!\n");
		exit(EXIT_FAILURE);
	}

	vertice = cria_vertice(id);
	no = cria_no(vertice);

	add_cauda(arvore->vertices, no);

	return vertice;

}

vertice_t* arvore_adicionar_vertice(arvore_t *arvore, vertice_t *vertice){

	no_t *no;

	if (arvore == NULL)	{
			fprintf(stderr,"grafo_adicionar_vertice: grafo invalido!");
			exit(EXIT_FAILURE);
	}

	no = cria_no(vertice);

	add_cauda(arvore->vertices, no);

	return vertice;

}

vertice_t* arvore_procura_vertice(arvore_t* arvore, int id){

	no_t *no_lista;
	vertice_t *vertice;
	int meu_id;

	if (arvore == NULL)	{
		fprintf(stderr,"procura_vertice: grafo invalido!");
		exit(EXIT_FAILURE);
	}

	if (lista_vazia(arvore->vertices) == TRUE)
		return FALSE;

	no_lista = obter_cabeca(arvore->vertices);

	while (no_lista){

		vertice = obter_dado(no_lista);

		meu_id = vertice_get_id(vertice);

		if (meu_id == id) {
			return vertice;
		}

		no_lista = obtem_proximo(no_lista);

	}

	return NULL;

}

void arvore_adiciona_filhos(arvore_t * arvore, vertice_t *vertice, int esq, int dir){

	vertice_t *esquerda;
	vertice_t *direita;

	if (arvore == NULL)	{
		fprintf(stderr,"arvore_adiciona_filhos: arvore invalida!");
		exit(EXIT_FAILURE);
	}

	if (esq != -1){
		esquerda = arvore_procura_vertice(arvore, esq);
		vertice_set_pai(esquerda, vertice);
		vertice_set_esq(vertice, esquerda);
	}

	if (dir != -1){
		direita = arvore_procura_vertice(arvore, dir);
		vertice_set_pai(direita, vertice);
		vertice_set_dir(vertice, direita);
	}

}

void arvore_exportar_grafo_dot(const char* filename, arvore_t* grafo){

    FILE* file;
	no_t* no_vert;
	vertice_t* vertice;

    if (filename == NULL || grafo == NULL){
		fprintf(stderr, "arvore_exportar_grafo_dot: ponteiros invalidos\n");
		exit(EXIT_FAILURE);
	}

	file = fopen(filename, "w");
	if (file == NULL){
		perror("arvore_exportar_grafo_dot:");
		exit(EXIT_FAILURE);
	}

	fprintf(file, "graph grafo_%d {\n", grafo->id);
	printf("\n");
	printf("graph grafo_%d {\n", grafo->id);

    no_vert = obter_cabeca(grafo->vertices);
    while (no_vert){
        vertice = obter_dado(no_vert);

        if(vertice_get_esq(vertice) != NULL){
            fprintf(file, "\t%d -- %d [label = 1];\n", vertice_get_id(vertice), vertice_get_id(vertice_get_esq(vertice) ));
            if(vertice_get_id(vertice_get_esq(vertice)) < 257){
                if(vertice_get_simbolo(vertice_get_esq(vertice)) < 32)
                    fprintf(file, "\t%d [ label = \" (FREQ: %d) ' ' \"];\n", vertice_get_id(vertice_get_esq(vertice)),
                        vertice_get_freq(vertice_get_esq(vertice)));
                else
                    fprintf(file, "\t%d [ label = \" (FREQ: %d) %c \"];\n", vertice_get_id(vertice_get_esq(vertice)),
                        vertice_get_freq(vertice_get_esq(vertice)), vertice_get_simbolo(vertice_get_esq(vertice)));
            }else{
                fprintf(file, "\t%d [ label = \" (FREQ: %d)\"];\n", vertice_get_id(vertice_get_esq(vertice)),
                        vertice_get_freq(vertice_get_esq(vertice)));
                printf("\t%d -- %d[FREQ: %d];\n", vertice_get_id(vertice), vertice_get_simbolo( vertice_get_esq(vertice) ), vertice_get_freq(vertice));
            }
        }

        if(vertice_get_dir(vertice) != NULL){
            fprintf(file, "\t%d -- %d [label = 0];\n", vertice_get_id(vertice), vertice_get_id( vertice_get_dir(vertice) ));
            if(vertice_get_id(vertice_get_dir(vertice)) < 257){
                if(vertice_get_simbolo(vertice_get_dir(vertice)) < 32)
                    fprintf(file, "\t%d [ label = \" (FREQ: %d) ' ' \"];\n", vertice_get_id(vertice_get_dir(vertice)),
                        vertice_get_freq(vertice_get_dir(vertice)));
                else
                    fprintf(file, "\t%d [ label = \" (FREQ: %d) %c \"];\n", vertice_get_id(vertice_get_dir(vertice)),
                        vertice_get_freq(vertice_get_dir(vertice)), vertice_get_simbolo(vertice_get_dir(vertice)));

            }else{
                fprintf(file, "\t%d [ label = \" (FREQ: %d) \"];\n", vertice_get_id(vertice_get_dir(vertice)),
                        vertice_get_freq(vertice_get_dir(vertice)));
                printf("\t%d -- %d[FREQ: %d];\n", vertice_get_id(vertice), vertice_get_simbolo( vertice_get_dir(vertice) ), vertice_get_freq(vertice));
            }
        }

        no_vert = obtem_proximo(no_vert);
    }

    no_vert = obter_cauda(grafo->vertices);
    vertice = obter_dado(no_vert);
    fprintf(file, "\t%d [ label = \" (FREQ: %d) \"];\n", vertice_get_id(vertice), vertice_get_freq(vertice));

	fprintf(file, "}\n");
	printf("}\n");
    fclose(file);

}

void libera_arvore(arvore_t* arvore){

	no_t* no_vert;
	no_t *no_liberado;
	vertice_t* vertice;

    if (arvore == NULL){
		fprintf(stderr, "libera_arvore: ponteiros invalidos\n");
		exit(EXIT_FAILURE);
	}

    no_vert = obter_cabeca(arvore->vertices);
    while (no_vert){
        vertice = obter_dado(no_vert);
        free(vertice);

        no_liberado = no_vert;
		no_vert = obtem_proximo(no_vert);
		free(no_liberado);
    }
    free(arvore->vertices);
    free(arvore);

}

void arvore_set_raiz(arvore_t* arvore, vertice_t* vertice){

	if (arvore == NULL){
			fprintf(stderr, "arvore_set_raiz: arvore invalida\n");
			exit(EXIT_FAILURE);
	}

	arvore->raiz = vertice;

}

vertice_t* arvore_get_raiz(arvore_t* arvore){

	if (arvore == NULL){
			fprintf(stderr, "arvore_get_raiz: arvore invalida\n");
			exit(EXIT_FAILURE);
	}

	return arvore->raiz;
}

lista_enc_t* arvore_obter_vertices(arvore_t* arvore){

	if (arvore == NULL){
		fprintf(stderr, "arvore_obter_vertices: arvore invalida\n");
		exit(EXIT_FAILURE);
	}

	return arvore->vertices;
}

vertice_t* arvore_procura_simbolo(arvore_t* arvore, char s){

	no_t *no_lista;
	vertice_t *vertice;
	int meu_s;

	if (arvore == NULL)	{
		fprintf(stderr,"arvore_procura_simbolo: arvore invalida!");
		exit(EXIT_FAILURE);
	}

	if (lista_vazia(arvore->vertices) == TRUE)
		return FALSE;

	no_lista = obter_cabeca(arvore->vertices);

	while (no_lista){

		vertice = obter_dado(no_lista);

		meu_s = vertice_get_simbolo(vertice);

		if (meu_s == s) {
			return vertice;
		}

		no_lista = obtem_proximo(no_lista);

	}

	return NULL;

}

void imprimi_freq(arvore_t* arvore){

    no_t* no_vert;
	vertice_t* vertice;

    no_vert = obter_cabeca(arvore->vertices);
    while (no_vert){
        vertice = obter_dado(no_vert);

        if(vertice_get_simbolo(vertice) == '\n')
            printf("%d(%s)[freq: %d];\n", vertice_get_id(vertice), "\\n", vertice_get_freq(vertice));
        else
            printf("%d(%c)[freq: %d];\n", vertice_get_id(vertice), vertice_get_simbolo(vertice), vertice_get_freq(vertice));

        no_vert = obtem_proximo(no_vert);
    }

}

void varrer_arvore(vertice_t* vertice, int bit, pilha_t* pilha){

    if(vertice == NULL)
        return;

    if(bit > -1)
        push_int(bit, pilha);

    varrer_arvore( vertice_get_esq(vertice), 1, pilha );
    varrer_arvore( vertice_get_dir(vertice), 0, pilha );

    if(vertice_eh_folha(vertice))
        guardar_bits(pilha, vertice);

    if(!pilha_vazia(pilha))
        pop_int(pilha);

}

char* arvore_cria_vetor_caracteres (arvore_t* arvore, vertice_t* inicial){

    int n = lista_get_tamanho(arvore->vertices);
    char* vetor_arvore = NULL;

	vetor_arvore = (char*) malloc(sizeof(char)*n);

    n = 0;
    fila_t* fila = cria_fila();

    enqueue(inicial,fila);

    while (!fila_vazia(fila)){
        vertice_t* elemento = dequeue(fila);
        vetor_arvore[n++] = vertice_get_simbolo(elemento);

        if(vertice_get_esq(elemento) != NULL){
            enqueue(vertice_get_esq(elemento), fila);
        }
        if(vertice_get_dir(elemento) != NULL){
            enqueue(vertice_get_dir(elemento), fila);
        }
    }
    libera_fila(fila);
    return vetor_arvore;
}
