#ifndef PILHA_H_INCLUDED
#define PILHA_H_INCLUDED


typedef struct pilhas pilha_t;

pilha_t * cria_pilha (void);

void push(void *dado, pilha_t *pilha);
void* pop(pilha_t *pilha);

int pilha_vazia(pilha_t *pilha);
void libera_pilha(pilha_t *pilha);

int pop_int(pilha_t *pilha);
void push_int(int dado, pilha_t *pilha);

#endif // PILHA_H_INCLUDED
