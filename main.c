#include <stdio.h>
#include <stdlib.h>
#include <string.h>

# include "huffman.h"

int main(int argc, const char* argv[]){

    if(argc < 4){

        if( (argc == 2) && (strcmp("--help", argv[1]) == 0) ){ // Caso tenha apenas 2 argumentos, sendo um deles '--help'

            printf("Uso: huffman [OPCAO] [ARQUIVO] [ARQUIVO]\n");
            printf("Algoritmo de compactacao e descompactacao de arquivos.\n\n");
            printf("Opcoes:\n");
            printf("\t-c\tCompacta o arquivo, gerando um novo com a extensao .dej\n");
            printf("\t-d\tDescompacta um arquivo com a extensao .dej\n");
            printf("\t--help\tmostra esta ajuda e sai\n");
            printf("\nRelate erros do algoritmo huffman: <diesson.floripa@gmail.com> ou <joao.v28@aluno.ifsc.edu.br>\n");
            printf("Documentacao completa em: <https://raw.githubusercontent.com/diesson/Algoritmo_Huffman/master/README.txt>\n");
            printf("ou disponivel localmente via: README.txt\n");

            return 0;

        }else{ // Caso tenha menos de 4 argumentos [executavel, opcao, primeiro arquivo e segundo arquivo]

            //printf("Teste: %d\n", argc);
            printf("huffman: Falta operando\n");
            printf("Tente: \"huffman --help\" para mais informacoes.\n");

            return 0;

        }
    }

    if(strcmp("-c", argv[1]) == 0){ // Se escolhido a opcao -c -> compactar

        if( strstr(argv[3], ".dej") ){ // Verificacao da extencao

            compactar(argv[2], argv[3]);

        }else{

            printf("O arquivo resultante da compactacao deve ter a extensao .dej\n");

            return 0;

        }

    }else if(strcmp("-d", argv[1]) == 0){ // Se escolhido a opcao -d -> descompactar

        if(strstr(argv[2], ".dej")){ // Verificacao da extencao

            descompactar(argv[2], argv[3]);

        }else{

            printf("O arquivo a ser descompactado deve ter a extensao .dej\n");

            return 0;

        }

    }else{ // Caso seja digitado outra opcao

        printf("huffman: Operando invalido\n");
        printf("Tente: \"huffman --help\" para mais informacoes.\n");

        return 0;
    }

    return 0; // Fim

}
