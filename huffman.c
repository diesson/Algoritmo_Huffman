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
    rewind(file);

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
    int i = 256;

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

    byte_t byte = 0;
    int i;

    for(i = 0; i < 8; i++){

        byte <<= 1;
        byte += bits[i];

    }

    return byte;

}

void compactar(const char* arquivo_i, const char* arquivo_f){

    FILE* file_in;
    FILE* file_out;
    arvore_t* arvore;
    pilha_t* pilha;
    fila_t* fila_temp;
    vertice_t* vertice;
    int i, bit, bit_extra;
    char caracter;
    int vetor_bits[8];
    byte_t byte;

    if (arquivo_i == NULL || arquivo_f == NULL){
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
    exporta_arvore(arvore, file_out);
    arvore = cria_arvore_huffman(arvore);

    pilha = cria_pilha();
    varrer_arvore(arvore_get_raiz(arvore), -1, pilha);
    libera_pilha(pilha);

    fila_temp = cria_fila();
#ifdef DEBUG
    printf("\n\nEscrevendo no arquivo: \n\n\t");
#endif // DEBUG
    i = 0;
    while(!feof(file_in)){

        caracter = fgetc(file_in);
        vertice = arvore_procura_simbolo(arvore, caracter);
        while( !fila_vazia(vertice_get_bits(vertice)) ){

            bit = dequeue_int( vertice_get_bits(vertice) );
            enqueue_int(bit, fila_temp);

            vetor_bits[i] = bit;
        #ifdef DEBUG
            printf("%d", vetor_bits[i]);
        #endif // DEBUG

            i++;
            if(i == 8){
                byte = criar_byte(vetor_bits);
            #ifdef DEBUG
                printf("\tbyte: %x\n\t", byte);
            #endif // DEBUG
                fwrite(&byte, sizeof(char), 1, file_out);
                i = 0;
            }
        }

        while(!fila_vazia(fila_temp))
            enqueue_int(dequeue_int(fila_temp), vertice_get_bits(vertice));

    }

    bit_extra = 0;
    if(i != 0){
        while(i < 8){
            bit_extra++;
            vetor_bits[i] = 0;
        #ifdef DEBUG
            printf("%d", vetor_bits[i]);
        #endif // DEBUG
            i++;
        }
        byte = criar_byte(vetor_bits);
    #ifdef DEBUG
        printf("\tbyte: %x\n\t", byte);
    #endif // DEBUG
        fwrite(&byte, sizeof(char), 1, file_out);
    }
#ifdef DEBUG
    printf("bit extra: %d \n", bit_extra);
#endif // DEBUG

    libera_fila(fila_temp);
    rewind(file_in);

    fseek(file_out, 4, SEEK_SET);
    fwrite(&bit_extra, sizeof(bit_extra), 1, file_out);

#ifdef DEBUG
    arvore_exportar_grafo_dot("arvore.dot", arvore);
#endif // DEBUG

    fclose(file_in);
    fclose(file_out);
}

void descompactar(const char* arquivo_i, const char* arquivo_f){

    FILE* file_in;
    FILE* file_out;
    arvore_t* arvore;
	int tamanho, bit_extra, i, j, bits, mascara, freq_temp;
	char simbolo_temp;
    byte_t byte;
    vertice_t* vertice;

    if (arquivo_i == NULL || arquivo_f == NULL){
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

	fread(&tamanho, sizeof(int), 1, file_in);
	fread(&bit_extra, sizeof(int), 1, file_in);

	arvore = cria_arvore(2);

    for(i=0; i<tamanho; i++){

        fread(&simbolo_temp, sizeof(char), 1, file_in);
        fread(&freq_temp, sizeof(int), 1, file_in);

        vertice_t* novo_vertice = arvore_adicionar_vertice_id(arvore, i);
        vertice_set_simbolo(novo_vertice, simbolo_temp);
        vertice_set_freq(novo_vertice, freq_temp);

    }

    arvore = cria_arvore_huffman(arvore);

    vertice = arvore_get_raiz(arvore);
    j = 0;
    while(1){

        fread(&byte, sizeof(char), 1, file_in);

        if (feof(file_in))
            break;

        printf("byte: %x\n", byte);
        if(feof(file_in)){
            j = bit_extra;
            printf("Fala galera: %d - %x\n", bit_extra, byte);
        }

        mascara = 128;
        for(i = 0; i < 8 - j; i++){

            bits = byte & mascara;
            bits  >>= (7 - i);
            mascara /= 2;

            printf("%d", bits);

            if(bits == 1){
                vertice = vertice_get_esq(vertice);
            }else if(bits == 0){
                vertice = vertice_get_dir(vertice);
            }else{
                perror("Deu ruim em: ");
                exit(EXIT_FAILURE);
            }

            if(vertice_eh_folha(vertice)){

                simbolo_temp = vertice_get_simbolo(vertice);
                printf("\t: %c\n", simbolo_temp);
                fwrite(&simbolo_temp, sizeof(char), 1, file_out);
                vertice = arvore_get_raiz(arvore);
            }

        }

    }


}

