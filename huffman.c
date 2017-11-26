#include <stdio.h>
#include <stdlib.h>
# include "huffman.h"

// Lista encadeada
#include "lista_enc.h"
#include "no.h"

// Arvore
#include "arvore.h"

// Fila
#include "fila.h"

// Pilha
//#include "pilha.h"

#define DEBUG

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

#ifdef DEBUG
    imprimi_freq(arvore);
#endif // DEBUG

}

arvore_t* cria_arvore_huffman(arvore_t* arvore){

    if (arvore == NULL){
        perror("cria_arvore_huffman: ");
        exit(EXIT_FAILURE);
    }

    arvore_t* arvore_huffman = cria_arvore(2);
    fila_t* fila_1 = cria_fila();
    fila_t* fila_2 = cria_fila();
    vertice_t* menor_vertice_1;
    vertice_t* menor_vertice_2;
    int i = 257;

    no_t* no = obter_cabeca(arvore_obter_vertices(arvore));
    while(no){
        vertice_t* elemento = obter_dado(no);
        enqueue(elemento,fila_1);
        no = obtem_proximo(no);
    }

    while(lista_get_tamanho(fila_obter_dados(fila_1)) > 1){

        menor_vertice_1 = dequeue(fila_1);
        while(!fila_vazia(fila_1)){

            vertice_t* vertice_tmp = dequeue(fila_1);
            if(vertice_get_freq(vertice_tmp)<vertice_get_freq(menor_vertice_1)){
                enqueue(menor_vertice_1,fila_2);
                menor_vertice_1 = vertice_tmp;
            }else{
                enqueue(vertice_tmp,fila_2);
            }

        }

        menor_vertice_2 = dequeue(fila_2);
        while(!fila_vazia(fila_2)){

            vertice_t* vertice_tmp = dequeue(fila_2);
            if(vertice_get_freq(vertice_tmp)<vertice_get_freq(menor_vertice_2)){
                enqueue(menor_vertice_2,fila_1);
                menor_vertice_2 = vertice_tmp;
            }else{
                enqueue(vertice_tmp,fila_1);
            }

        }

#ifdef DEBUG
        printf("\ncria_arvore_huffman: menores valores de frequencia obtidos");
        printf("\nVertice: %d, Freq: %d\n",vertice_get_id(menor_vertice_1),vertice_get_freq(menor_vertice_1));
        printf("Vertice: %d, Freq: %d\n",vertice_get_id(menor_vertice_2),vertice_get_freq(menor_vertice_2));
#endif // DEBUG

        vertice_t* novo_vertice = cria_vertice(i++);
        vertice_set_esq(novo_vertice, menor_vertice_1);
        vertice_set_dir(novo_vertice, menor_vertice_2);
        vertice_set_freq(novo_vertice, vertice_get_freq(menor_vertice_1)+vertice_get_freq(menor_vertice_2));
        vertice_set_pai(menor_vertice_1, novo_vertice);
        vertice_set_pai(menor_vertice_2, novo_vertice);
        arvore_adicionar_vertice(arvore_huffman, menor_vertice_1);
        arvore_adicionar_vertice(arvore_huffman, menor_vertice_2);
        enqueue(novo_vertice, fila_1);

    }
    arvore_adicionar_vertice(arvore_huffman, vertice_get_pai(menor_vertice_1));
    arvore_set_raiz(arvore_huffman, vertice_get_pai(menor_vertice_1));

    return arvore_huffman;
}

byte_t criar_byte(int* bits){

    char byte = 0;
    int i;

    for(i = 0; i < 8; i++){

        byte += bits[i];
        byte <<= 1;

    }

    return byte;

}

void compactar(const char* arquivo_i, const char* arquivo_f){

    FILE* file_in;
    FILE* file_out;
    arvore_t* arvore;
    pilha_t* pilha;

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
    arvore = cria_arvore_huffman(arvore);
    char* vetor_arvore = arvore_cria_vetor_caracteres(arvore, arvore_get_raiz(arvore));

    int tamanho = lista_get_tamanho(arvore_obter_vertices(arvore));
    fwrite(&tamanho, sizeof(tamanho), 1, file_out);

    int i;
    for(i=0; i<lista_get_tamanho(arvore_obter_vertices(arvore)); i++){
        fwrite(&vetor_arvore[i], sizeof(char), 1, file_out);
    }
#ifdef DEBUG
    int n;

    printf("\n\nElementos do vetor gerado por arvore_cria_vetor_caracteres:\n");
    for(n=0; n<lista_get_tamanho(arvore_obter_vertices(arvore)); n++){
        printf("\tVertice [%d]: %c\n", n, vetor_arvore[n]);
    }
#endif // DEBUG

    pilha = cria_pilha();
    //varrer_arvore(arvore_get_raiz(arvore), -1, pilha);
    libera_pilha(pilha);

#ifdef DEBUG
    arvore_exportar_grafo_dot("arvore.dot", arvore);
#endif // DEBUG

    fclose(file_in);
    fclose(file_out);
}

void descompactar(const char* arquivo_i, const char* arquivo_f){



}

