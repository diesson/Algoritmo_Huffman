#ifndef PILHA_H_INCLUDED
#define PILHA_H_INCLUDED

typedef struct pilhas pilha_t;

// Criação da pilha
pilha_t * cria_pilha (void);

//Informações da pilha
int pilha_vazia(pilha_t *pilha);

// Liberar o espaço alocado
void libera_pilha(pilha_t *pilha);

// Adicionar ou remover elementos
int pop_int(pilha_t *pilha);
void push_int(int dado, pilha_t *pilha);

#endif // PILHA_H_INCLUDED
