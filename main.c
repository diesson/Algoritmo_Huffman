#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char* argv[]){

    if(argc < 4){

        if( (argc == 2) && (strcmp("--help", argv[1]) == 0) ){

            printf("Uso: huffman [OPCAO] [ARQUIVO] [ARQUIVO]\n");
            printf("Algoritmo de compactacao e descompactacao de arquivos.\n\n");
            printf("Opcoes:\n");
            printf("\t-c\tCompacta o arquivo, gerando um novo com a extensao '.dej'\n");
            printf("\t-d\tDescompacta um arquivo '.dej'\n");
            printf("\t--help\tmostra esta ajuda e sai\n");
            //printf("\nPara mais informaoes, com exemplos de uso, verifique o arquivo README.txt\n");
            printf("\nRelate erros do algoritmo huffman: <diesson.floripa@gmail.com> ou <joao.v28@aluno.ifsc.edu.br>\n");
            printf("Documentacao completa em: <https://github.com/diesson/Algoritmo_Huffman/README.txt>\n");
            printf("ou disponivel localmente via: README.txt\n");

            return 0;

        }else{

            //printf("Teste: %d\n", argc);
            printf("huffman: Falta operando\n");
            printf("Tente: \"huffman --help\" para mais informacoes.\n");

            return 0;

        }
    }

    if (strcmp("-c", argv[1]) == 0){

        if ( strstr(argv[3], ".dej") ){

            //Compactar

        }else{

            printf("O arquivo resultante da compactacao deve ter a extensao '.dej'.\n");

            return 0;

        }

    }else if (strcmp("-d", argv[1]) == 0){

        if (strstr(argv[2], ".dej")){

            //Descompactar

        }else{

            printf("O arquivo a ser descompactado deve ter a extensao '.dej'.\n");

            return 0;

        }

    }else{

        printf("huffman: Operando invalido\n");
        printf("Tente: \"huffman --help\" para mais informacoes.\n");

        return 0;
    }

    return 0;

}
