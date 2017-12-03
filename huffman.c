#include <stdio.h>
#include <stdlib.h>
#include "huffman.h"

// Lista encadeada
#include "lista_enc.h"
#include "no.h"

// Arvore
#include "arvore.h"

// Fila
#include "fila.h"

// Pilha
#include "pilha.h"

#define DEBUG
/*
    struct dicionario:
        Estrutura que armazena as informações relativas a cada caracter
        no processo de descompactação de um arquivo no formato '.dej'

*/
struct dicionario{
	char caracter;
	unsigned int frequencia;
};


/*
    frequencia_caracter:
        Função responsável por varrer o arquivo de entrada, identificando os caracteres presentes
        no arquivo, assim como a frequência de ocorrência de cada um
*/
void frequencia_caracter(arvore_t* arvore, FILE* file){

    int i = 0;
    unsigned int freq;
    vertice_t* vertice;
    char caracter;

    while(1){

        caracter = fgetc(file);
        if (feof(file))
            break;

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

    }
    rewind(file);

//Depuração que imprime cada caractere presente no texto assim como a frequência de aparecimento de cada um.
#ifdef DEBUG
    printf("\nfrequencia_caracter: valores de frequencia obtidos\n");
    imprimi_freq(arvore);
#endif // DEBUG

}

/*
    cria_arvore_huffman:
        Construção da árvore binária seguindo o algoritmo de Huffman, estrutura usada
        como modelo para a codificação dos caracteres de forma a reduzir a quantidade
        de bits necessária para representá-los.
*/
arvore_t* cria_arvore_huffman(arvore_t* arvore){

    if (arvore == NULL){
        perror("cria_arvore_huffman: ");
        exit(EXIT_FAILURE);
    }

    // Estrutura de destino para a árvore gerada
    arvore_t* arvore_huffman = cria_arvore(2);

    fila_t* fila_1 = cria_fila();
    fila_t* fila_2 = cria_fila();
    vertice_t* menor_vertice_1;
    vertice_t* menor_vertice_2;
    int i = 256;

    // Adiciona todos os vértices à fila de varredura
    no_t* no = obter_cabeca(arvore_obter_vertices(arvore));
    while(no){
        vertice_t* elemento = obter_dado(no);
        enqueue(elemento,fila_1);
        no = obtem_proximo(no);
    }

    while(lista_get_tamanho(fila_obter_dados(fila_1)) > 1){

        // Varredura da lista em busca do vértice com caractere de menor frequência
        menor_vertice_1 = dequeue(fila_1);
        while(!fila_vazia(fila_1)){

            vertice_t* vertice_tmp = dequeue(fila_1);

            // Caso vértice removido da fila seja menor que a referência, ocorre a troca da referência
            if(vertice_get_freq(vertice_tmp)<vertice_get_freq(menor_vertice_1)){

                //Quando sai da posição de referência, o vértice é colocado na segunda fila
                enqueue(menor_vertice_1,fila_2);
                menor_vertice_1 = vertice_tmp;
            }else{

                // Vértices não selecionados são jogados em uma segunda fila
                enqueue(vertice_tmp,fila_2);
            }
        }

        // No processo de retorno à fila original, procura-se o vértice da fila com segundo menor valor de frequência
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

//Depuração que verifica os valores dos vértices selecionados como menores encontrados na varredura da lista
#ifdef DEBUG
        printf("\ncria_arvore_huffman: menores valores de frequencia obtidos");
        printf("\nVertice: %d, Freq: %u\n",vertice_get_id(menor_vertice_1),vertice_get_freq(menor_vertice_1));
        printf("Vertice: %d, Freq: %u\n",vertice_get_id(menor_vertice_2),vertice_get_freq(menor_vertice_2));
#endif // DEBUG

        // Criação do novo vértice de referência que liga as menores frequências da etapa
        vertice_t* novo_vertice = cria_vertice(i++);
        vertice_set_esq(novo_vertice, menor_vertice_1);
        vertice_set_dir(novo_vertice, menor_vertice_2);
        vertice_set_freq(novo_vertice, vertice_get_freq(menor_vertice_1)+vertice_get_freq(menor_vertice_2));
        vertice_set_pai(menor_vertice_1, novo_vertice);
        vertice_set_pai(menor_vertice_2, novo_vertice);

        // Vértices já selecionados não entram novamente na busca e são jogados na estrutura que armazena a árvore de Huffman
        arvore_adicionar_vertice(arvore_huffman, menor_vertice_1);
        arvore_adicionar_vertice(arvore_huffman, menor_vertice_2);

        // O novo vértice criado como referência participa da próxima etapa da busca por menores frequências
        enqueue(novo_vertice, fila_1);

    }

    // O vértice restante (com maior frequência) ao final do processo é automaticamente adicionado como raiz da árvore de Huffman
    arvore_adicionar_vertice(arvore_huffman, vertice_get_pai(menor_vertice_1));
    arvore_set_raiz(arvore_huffman, vertice_get_pai(menor_vertice_1));

    return arvore_huffman;
}

/*
    criar_byte:
        Por limitações na manipulação direta de bits, a necessidade de manipular a informação
        apenas com bytes completos requer o uso de uma função que insira os bits, um a um, em
        em um conjunto do tamanho de um byte.
*/
byte_t criar_byte(unsigned int* bits){

    byte_t byte = 0;
    int i;

    for(i = 0; i < 8; i++){

        byte <<= 1;
        byte += bits[i];

    }

    return byte;

}
/*
    compactar:
        Responsável por chamar as funções que fazem a varredura do texto, determinam a frequência,
        criam a árvore de codificação de Huffman, além de criar o dicionário para a composição do
        texto compactado, imprimir o cabeçalho com as informações de tamanho de dicionário, tamanho
        do texto após a compactação e número de bits extras adicionados ao fim do último conjunto de
        bits exportados. Em seguida, a função ainda faz a leitura, um a um, dos caracteres a se
        codificar, processa as informações necessárias e conclui imprimindo-os no arquivo '.dej' de
        destino.
*/
void compactar(const char* arquivo_i, const char* arquivo_f){

    FILE* file_in;
    FILE* file_out;
    arvore_t* arvore;
    pilha_t* pilha;
    fila_t* fila_temp;
    vertice_t* vertice;
    int i, bit, bit_extra;
    unsigned int tamanho_texto = 0;
    char caracter;
    unsigned int vetor_bits[8];
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

    // Chama a função de varredura em busca das frequências
    frequencia_caracter(arvore, file_in);

    // Imprime cabeçalho do arquivo compactado
    exporta_arvore(arvore, file_out);
    arvore = cria_arvore_huffman(arvore);

    // Depuração que exporta a árvore de Huffman gerada
#ifdef DEBUG
    printf("\ncompactar: arvore de Huffman criada com base no arquivo de entrada");
    arvore_exportar_grafo_dot("arvore.dot", arvore);
#endif // DEBUG

    // Criação do dicionário codificado
    pilha = cria_pilha();

    #ifdef DEBUG
        printf("\nvarrer_arvore: codigos gerados pela codificacao\n");
    #endif // DEBUG
    varrer_arvore(arvore_get_raiz(arvore), -1, pilha);
    libera_pilha(pilha);

    fila_temp = cria_fila();
#ifdef DEBUG
    printf("\n\ncompactar: escrevendo as informacoes no arquivo \n\t");
#endif // DEBUG
    i = 0;
    while(1){

        caracter = fgetc(file_in);
        if(feof(file_in))
            break;

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
                fwrite(&byte, sizeof(byte_t), 1, file_out);
                tamanho_texto++;
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
        tamanho_texto++;
    }
#ifdef DEBUG
    printf("bit extra: %d \n", bit_extra);
#endif // DEBUG

    libera_fila(fila_temp);
    rewind(file_in);

    // Atualiza no arquivo as informções referentes ao processamento do texto
    fseek(file_out, 4, SEEK_SET);
    fwrite(&bit_extra, sizeof(bit_extra), 1, file_out); // Numero de bits adicionados no último byte
    fwrite(&tamanho_texto, sizeof(tamanho_texto), 1, file_out); // Tamanho do texto após compactação

    fclose(file_in);
    fclose(file_out);
}

/*
    descompactar:
        Função que decodifica a informação presente no arquivo '.dej' e recria o arquivo original.
        Primeiramente, ocorre a leitura do cabeçalho, tomando conta das informações sobre o dicionário,
        tamanho do arquivo e bits extras adicionados, reconstruindo ainda, a árvore que representa a
        codificação criada na compactação. Depois disso, lê toda a informação do arquivo, salvando em
        um vetor alocado dinamicamente. O processamento do vetor, byte a byte, faz a varredura da árvore,
        encontrando os caracteres correspondentes à sequência encontrada no arquivo compactado.
*/
void descompactar(const char* arquivo_i, const char* arquivo_f){

    FILE* file_in;
    FILE* file_out;
    arvore_t* arvore;
	int tamanho, bit_extra, i, j, qtd_byte;
	unsigned int tamanho_texto, mascara, bits;
	char simbolo;
	byte_t* texto;
    vertice_t* vertice;
    dicionario_t* dicio;


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
    // Obtenção das informações do cabeçalho do arquivo compactado
	fread(&tamanho, sizeof(tamanho), 1, file_in); // Tamanho do dicionário
	fread(&bit_extra, sizeof(bit_extra), 1, file_in); // Número de bits extras adicionados ao último byte
	fread(&tamanho_texto, sizeof(tamanho_texto), 1, file_in); // Tamanho do texto compactado

	arvore = cria_arvore(2);

    // Alocação do espaço destinado à informação codificada do arquivo
    dicio = malloc(sizeof(dicionario_t)*tamanho);

    // Obtenção do dicionário
    #ifdef DEBUG
     printf("\ndescompactar: leitura do dicionario a partir do arquivo compactado\n");
    #endif // DEBUG
    for(i=0; i<tamanho; i++){

        fread(&dicio[i].caracter, sizeof(char), 1, file_in); // Caractere
        fread(&dicio[i].frequencia, sizeof(unsigned int), 1, file_in); // Frequência do caractere

        #ifdef DEBUG
        printf("Caracter: %c; Frequencia: %u\n", dicio[i].caracter, dicio[i].frequencia);
        #endif // DEBUG

        // Criação e inclusão do caractere à árvore temporária
        vertice_t* novo_vertice = arvore_adicionar_vertice_id(arvore,i);
        vertice_set_simbolo(novo_vertice, dicio[i].caracter);
        vertice_set_freq(novo_vertice, dicio[i].frequencia);

    }

    // Cria árvore de Huffman baseda na árvore temporária contendo os caracteres do dicionário
    arvore = cria_arvore_huffman(arvore);

    // Alocação do espaço destinado ao texto lido
    texto = malloc(sizeof(byte_t)*tamanho_texto);

    // Leitura do texto por meio de chamada de sistema
    fread(texto, sizeof(byte_t), tamanho_texto, file_in);

    vertice = arvore_get_raiz(arvore);
    j = 0;

    // Varredura do vetor
    #ifdef DEBUG
    printf("\ndescompactar: Caracteres lidos e convertidos a partir do arquivo '.dej'\n");
    #endif // DEBUG
    for(qtd_byte = 0; qtd_byte < tamanho_texto; qtd_byte++){

        // Quando encontra o final do texto, diminui o laço proporcionala ao número de bits extras adicionados
        if(qtd_byte >= (tamanho_texto - 1) ){
            j = bit_extra;
        }

        mascara = 128;
        for(i = 0; i < 8 - j; i++){

            // Acesso a cada bit individualmente
            bits = texto[qtd_byte] & mascara;
            bits  >>= (7 - i);
            mascara >>= 1;

            // Navega pela árvore dependendo do bit encontrado
            if(bits == 1){
                vertice = vertice_get_esq(vertice);
            }else if(bits == 0){
                vertice = vertice_get_dir(vertice);
            }else{
                perror("descompactar_obter_bit: ");
                exit(EXIT_FAILURE);
            }

            // Ao encontrar uma folha, exporta o caractere encontrado
            if(vertice_eh_folha(vertice)){

                simbolo = vertice_get_simbolo(vertice);

                #ifdef DEBUG
                printf("\tcaractere: %c\n", simbolo);
                #endif // DEBUG

                fwrite(&simbolo, sizeof(simbolo), 1, file_out);
                vertice = arvore_get_raiz(arvore);
            }
        }
    }
}
