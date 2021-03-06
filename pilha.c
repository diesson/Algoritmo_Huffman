#include <stdlib.h>
#include <stdio.h>

#include "lista_enc.h"
#include "no.h"
#include "pilha.h"

//#define DEBUG

#define FALSO 0
#define VERDADEIRO 1

struct pilhas {
	lista_enc_t *dados;
};


//cria uma pilha generica
pilha_t * cria_pilha (void)
{
    pilha_t *pilha = (pilha_t*)malloc(sizeof(pilha_t));

    if (pilha == NULL) {
        fprintf(stderr, "Erro alocando dados em cria_pilha!\n");
        exit(EXIT_FAILURE);
    }

    pilha->dados = cria_lista_enc();

    return pilha;
}

void libera_pilha(pilha_t* pilha)
{
	if (pilha == NULL) {
        fprintf(stderr, "Erro em libera_pilha\n");
        exit(EXIT_FAILURE);
    }

    if (!lista_vazia(pilha->dados))    {
    	fprintf(stderr, "Impossivel liberar pilha, ainda ha dados\n");
    	exit(EXIT_FAILURE);
    }

    free(pilha->dados);
    free(pilha);

}

int pilha_vazia(pilha_t *pilha)
{
    if (pilha == NULL) {
        fprintf(stderr, "pilha_vazia: pilha invalida\n");
        exit(EXIT_FAILURE);
    }

    return lista_vazia(pilha->dados);
}

// Insere elemento no topo da pilha
void push_int(int dado, pilha_t *pilha){

	no_t *no;

    if (pilha == NULL) {
        fprintf(stderr, "push: pilha invalida\n");
        exit(EXIT_FAILURE);
    }

    no = cria_no(dado);
    add_cauda(pilha->dados, no);

}

// Retira elemento do topo
int pop_int(pilha_t *pilha){

	no_t *no;
	int dado;

    if (pilha == NULL){
        fprintf(stderr, "pop: pilha invalida!\n");
        exit(EXIT_FAILURE);
    }

    no = remover_cauda(pilha->dados);
    dado = obter_dado(no);
    free(no);

    return dado;
}
