#include <stdio.h>
#include <stdlib.h>
# include "huffman.h"

// Lista encadeada
#include "lista_enc.h"
#include "no.h"

// Arvore
#include "arvore.h"

// Fila
//#include "fila.h"

// Pilha
//#include "pilha.h"

void frequencia_caracter(arvore_t* arvore, FILE* file){

    int i = 0, freq;
    vertice_t* vertice;
    char caracter;

    while(1){

        caracter = fgetc(file);

        vertice = arvore_procura_simbolo(arvore, caracter);

        if(vertice){

            freq = vertice_get_freq(vertice);
            freq++;
            vertice_set_freq(vertice, freq);

        }else{

            vertice = arvore_adicionar_vertice_id(arvore, i);
            vertice_set_simbolo(vertice, caracter);
            i++;

        }



        if (feof(file))
            break;


    }

    imprimi_freq(arvore);

}

void compactar(const char* arquivo_i, const char* arquivo_f){

    FILE* file_in;
    FILE* file_out;
    arvore_t* arvore;

    if (arquivo_i == NULL || arquivo_i == NULL){
		fprintf(stderr, "compactar: ponteiros invalidos\n");
		exit(EXIT_FAILURE);
	}

    file_in = fopen(arquivo_i, "rb");
    if (file_in == NULL){
		perror("compactar: erro fopen\n");
		exit(EXIT_FAILURE);
	}

    file_out = fopen(arquivo_f, "wb");
    if (file_out == NULL){
		perror("compactar: erro fopen\n");
		exit(EXIT_FAILURE);
	}

    arvore = cria_arvore(1);

    frequencia_caracter(arvore, file_in);








}

void descompactar(const char* arquivo_i, const char* arquivo_f){



}



