#ifndef HUFFMAN_INCLUDED
#define HUFFMAN_INCLUDED

#include "lista_enc.h"
#include "no.h"
#include "arvore.h"

void compactar(const char* arquivo_i, const char* arquivo_f);
void descompactar(const char* arquivo_i, const char* arquivo_f);

void frequencia_caracter(arvore_t* arvore, FILE* file);


#endif // HUFFMAN_INCLUDED
