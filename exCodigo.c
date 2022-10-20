#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM 256

typedef struct no {
    unsigned char caracter;
    int frequencia;
    struct no *esq, *dir, *proximo;
}No;

typedef struct {
    No *inicio;
    int tam;
}Lista;

void criar_lista(Lista *lista) {
    lista->inicio = NULL;
    lista->tam = 0;
}

void inserir_ordenado(Lista *lista, No *no) {
    No *aux;

    //Vazia?
    if(lista->inicio == NULL) {
        lista->inicio = no;
    }

    //Tem frequencia menor que o inicio da lista
    else if(no->frequencia < lista->inicio->frequencia) {
        no->proximo = lista->inicio;
        lista->inicio = no;
    }

    else {
        aux = lista->inicio;

        while(aux->proximo && aux->proximo->frequencia <= no->frequencia) {
            aux = aux->proximo;
        }
        no->proximo = aux->proximo;
        aux->proximo = no;
    }

    lista->tam++;
}

void preencher_lista(unsigned int tab[], Lista *lista) {
    int i;

    No *novo;

    for(i=0;i<TAM;i++) {
        if(tab[i]>0) {
            novo = malloc(sizeof(No));

            if(novo) {
                novo->caracter = i;
                novo->frequencia = tab[i];
                novo->dir = NULL;
                novo->esq = NULL;
                novo->proximo = NULL;

                inserir_ordenado(lista, novo);
            }
            else {
                printf("\tERRO ao alocar memória em preencher_lista!\n");
                break;
            }
        }
    }
}

void imprimir_lista(Lista *lista) {
    No *aux = lista->inicio;

    printf("\n\tLista ordenada: Tamanho: %d\n", lista->tam);

    while(aux) {
        printf("\nCaracter: %c Frequencia: %d\n", aux->caracter, aux->frequencia);
        aux = aux->proximo;
    }
}

void inicializa_tabela_com_zero(unsigned int tab[]) {
    int i;

    for(i=0;i<TAM;i++) {
        tab[i]=0;
    }
}

void preenche_tab_frequencia(unsigned char texto[], unsigned int tab[]) {
    int i=0;

    while(texto[i] !='\0') {
        tab[texto[i]]++;
        i++;
    }
}

void imprime_tab_frequencia(unsigned int tab[]) {
    int i;

    printf("\tTABELA DE FREQUENCIA\n");

    for(i=0;i<TAM;i++) {
        if(tab[i]>0) {
            printf("\t%d = %u = %c\n", i, tab[i], i);
        }
    }
}

No *remove_no_inicio(Lista *lista) {
    No *aux = NULL;

    if(lista->inicio) {
        aux = lista->inicio;
        lista->inicio = aux->proximo;
        aux->proximo = NULL;
        lista->tam--;
    }

    return aux;
}

No *montar_arvore(Lista *lista) {
    No *primeiro, *segundo, *novo;

    while(lista->tam > 1) {
        primeiro = remove_no_inicio(lista);
        segundo = remove_no_inicio(lista);
        novo = malloc(sizeof(No));

        if(novo) {
            novo->caracter = '+';
            novo->frequencia = primeiro->frequencia + segundo->frequencia;
            novo->esq = primeiro;
            novo->dir = segundo;
            novo->proximo = NULL;

            inserir_ordenado(lista, novo);
        }

        else {
            printf("\n\tERRO ao alocar memoria em montar_arvore!\n");
            break;
        }
    }

    return lista->inicio; //nó raiz da árvore
}

void imprimir_arvore(No *raiz, int tam) {
    if(raiz->esq == NULL && raiz->dir == NULL) {
        printf("\tFolha: %c\tAltura: %d\n", raiz->caracter, tam);
    }
    
    else {
        imprimir_arvore(raiz->esq, tam+1);
        imprimir_arvore(raiz->dir, tam+1);
    }
}

int main() {
    unsigned char texto[] = "Lab estrutura de dados";
    unsigned int tabela_frequencia[TAM];
    int colunas;
    char **dicionario, *codificado, *decodificado;

    Lista lista;
    No *arvore;

    //Tabela de frequência
    inicializa_tabela_com_zero(tabela_frequencia);
    preenche_tab_frequencia(texto, tabela_frequencia);
    imprime_tab_frequencia(tabela_frequencia);

    //Lista encadeada ordenada
    criar_lista(&lista);
    preencher_lista(tabela_frequencia, &lista);
    imprimir_lista(&lista);

    //Montar árvore
    arvore = montar_arvore(&lista);
    printf("\n\tArvore de Huffman\n");
    imprimir_arvore(arvore, 0);
}