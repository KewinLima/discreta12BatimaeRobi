/********************************************************
**| Programa   : Rede de Petri com Processos Paralelos|**
**| Autores    : Isabella Galvão e Kewin Lima         |**
**| Orientador : Ruben Carlo                          |**
*********************************************************
**/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#define BLOCO_DE_ENTRADA 5
#define DEBUG 1
/*#undef DEBUG*/ /* Caso queira um debug,por favor comente essa linha. */ 

/************** Definição dos tipos para as listas  ****************/
/*Define uma transição*/
typedef struct struct_transicao transicao;
struct struct_transicao
{
    int coletor;
    int emissor;
};

/*Define um node que armazena um inteiro*/
typedef struct struct_node node;
struct struct_node
{
    int conteudo;
    node *proximo;
};

/*Define um arco lugar*/
typedef struct struct_arco_lugar arco_lugar;
struct struct_arco_lugar
{
    transicao *origem;
    node *destino;
};

/*Define um nó que armazena arcos transição.*/
typedef struct struct_node_at node_arco_transicao;
struct struct_node_at
{
    arco_transicao *conteudo;
    node_arco_transicao *proximo;
};

/*Define um nó que armazena arcos lugar*/
typedef struct struct_node_al node_arco_lugar;
struct struct_node_al
{
    arco_lugar *conteudo;
    node_arco_lugar *proximo;
};

typedef struct struct_node_transicao node_transicao;
struct struct_node_transicao
{
    transicao *conteudo;
    node_transicao *proximo;
};

/*A lista é definida como um ponteiro para o primeiro elemento da lista*/

/*Define a lista que guarda inteiros*/
typedef struct struct_lista lista;
struct struct_lista
{
    node *cabeca;
};

/*Define a lista que guarda arcos transição*/
typedef struct struct_lista_at lista_arco_transicao;
struct struct_lista_at
{
    node_arco_transicao *cabeca;
};

/*Define a lista que guarda arcos lugar*/
typedef struct struct_lista_al lista_arco_lugar;
struct struct_lista_al
{
    node_arco_lugar *cabeca;
};

typedef struct struct_lista_transicao lista_transicao;
struct struct_lista_transicao
{
    node_transicao *cabeca;
};

/********************* Protótipos **********************/

/*               Operações para a lista                */
lista *cria_lista(void);
void adiciona_na_lista(lista *l, int valor);
node_transicao *busca_elemento_por_indice_lista_transicao(lista_transicao *l, int indice);
void imprime_lista(lista *l);
void limpa_lista(lista *l);

/*         Operações para listas de arco-lugar         */
lista_arco_lugar *cria_lista_arco_lugar(void);
void adiciona_na_lista_arco_lugar(lista_arco_lugar *l, arco_lugar *valor);
node_arco_lugar *busca_elemento_por_indice_arco_lugar(lista_arco_lugar *l, int indice);
void imprime_lista_arco_lugar(lista_arco_lugar *l);
void limpa_lista_arco_lugar(lista_arco_lugar *l);

/*       Operações para listas de arco-transicoes      */
lista_arco_transicao *cria_lista_arco_transicao(void);
void adiciona_na_lista_arco_transicao(lista_arco_transicao *l, arco_transicao *valor);
node_arco_transicao *busca_elemento_por_indice_arco_transicao(lista_arco_transicao *l, int indice);
void imprime_lista_arco_transicao(lista_arco_transicao *l);
void limpa_lista_arco_transicao(lista_arco_transicao *l);

/*           Operações para listas transições          */
lista_transicao *cria_lista_transicao(void);
void adiciona_na_lista_transicao(lista_transicao *l, transicao *valor);
node_transicao *busca_elemento_por_indice_lista_transicao(lista_transicao *l, int indice);
void imprime_lista_transicao(lista_transicao *l);
void limpa_lista_transicao(lista_transicao *l);

int main(void)
{ 
    int n; /* variável para o laço da leitura do primeiro bloco de entrada */
    int x; /* Variavel para determinar o limite do laço */
    lista *lugares;                         /*Cria ponteiro de  lista de lugares, que serão inteiros*/
    lista_arco_lugar *arcos_lugar;          /*Cria ponteiro de lista de arcos lugar*/
    lista_arco_transicao *arcos_transicao;  /*Cria ponteiro de lista de arcos transição*/
    lista_transicao *transicoes;            /*Cria ponteiro de lista de inteiros*/
    lista *entrada = cria_lista();          /*Cria uma lista para armazenar as entradas iniciais*/
    FILE *a = fopen("entrada1.txt","r");    /* Abrindo o arquivo de entrada*/

    for(n = 0; n < BLOCO_DE_ENTRADA; n++)   /*Laço para leitura do primeiro bloco de entradas*/
    {
        int leitura;
        fscanf(arquivo,"%d", &leitura);

        adiciona_na_lista(entrada, leitura);
    }
#ifdef DEBUG
    imprime_lista(entrada);
#endif

    /*Lê quantidade de lugares requerida e cria x lugares com 0 (zero) tokens*/
    x = busca_elemento_por_indice(entrada, 0)->conteudo;
    lugares = cria_lista();
    for(n = 0; n < x; n++)
        adiciona_na_lista(lugares, 0); 
#ifdef DEBUG
    imprime_lista(lugares);
#endif

    /* Lê quantidade de transições e cria x transições vazias*/
    x = busca_elemento_por_indice(entrada, 1)->conteudo;
    transicoes = cria_lista_transicao();
    for(n = 0; n < x; n++)
    {
        transicao *t = malloc(sizeof(transicao));
        t->coletor = 0;
        t->emissor = 0;
        adiciona_na_lista_transicao(transicoes, t);
    }
#ifdef DEBUG
    imprime_lista_transicao(transicoes);
#endif


    //Lê a quantidade de tokens em um dado lugar e adiciona esses tokens apropriadamente
    x = busca_elemento_por_indice(entrada, 2)->conteudo;
    for(n = 0; n < x; n++)
    {
        int quantidade;
        int indice;
        fscanf(arquivo, "%d", &indice);
        fscanf(arquivo, "%d", &quantidade);
        busca_elemento_por_indice(lugares, indice)->conteudo = quantidade;
    }
    //Lê as informações sobre os arcos transicao, cria cada um e os guarda na lista adequada
    x = busca_elemento_por_indice(entrada, 3)->conteudo;
    arcos_transicao = cria_lista_arco_transicao();
    for(n = 0; n < x; n++)
    {
        int lugar;
        int quantidade;
        int transicao;
        arco_transicao *at = malloc(sizeof(arco_transicao));

        fscanf(arquivo, "%d", &lugar);
        fscanf(arquivo, "%d", &quantidade);
        fscanf(arquivo, "%d", &transicao);

        at->origem = busca_elemento_por_indice(lugares, lugar);
        at->destino = busca_elemento_por_indice_lista_transicao(transicoes, transicao)->conteudo;
        at->destino->coletor = quantidade;
        adiciona_na_lista_arco_transicao(arcos_transicao, at);
    }
    
    /* Ao fim do código, limpa todas as alocações dinâmicas realizadas */
    limpa_lista(lugares);
    limpa_lista_arco_lugar(arcos_lugar);
    limpa_lista_arco_transicao(arcos_transicao);
    limpa_lista_transicao(transicoes);

    fclose(a);/* fechando o arquivo de entrada*/
    return 0;
}

lista *cria_lista(void)
{
    lista *l = malloc(sizeof(lista));
    l->cabeca = NULL;
    return l;
}
/*Adiciona um valor ao final da lista*/
/*O valor deve ser do mesmo tipo que o conteúdo do node*/

void adiciona_na_lista(lista *l, int valor)
{
    /*Cria um novo node com o valor a ser adicionado*/
    de_arco_lugar *busca_elemento_por_indice_arco_lugar(lista_arco_lugar *l, int indice);
    node *novo_node = malloc(sizeof(node));
    novo_node->conteudo = valor;
    novo_node->proximo = NULL;

    /*Verifica se a lista é vazia*/
    /*Se sim, adiciona o novo_node na cabeça*/
    if(l->cabeca == NULL)
    {
        l->cabeca = novo_node;
    }
    else {
        /*Se não for vazia, procura o último node da lista*/
        node *no;
        for(no = l->cabeca; no->proximo != NULL; no = no->proximo);

        /*Nesse último node, adiciona o novo_node como proximo*/
        no->proximo = novo_node;
    }
}
node *busca_elemento_por_indice(lista *l, int indice)
{
    int i;
    node *no = l->cabeca;

    for(i = 0; i < indice; i++)
    {
        if(no == NULL)
            break;

        no = no->proximo;
    }

    return no;
}
/* Imprime a lista, elemento por elemento */
void imprime_lista(lista *l)
{
    node *no;
    for(no = l->cabeca; no != NULL; no = no->proximo)
    {
        printf(" %d ", no->conteudo);
    }
}
/*Libera a memória alocada para a lista e seus nós*/
void limpa_lista(lista *l)
{
    node *no = l->cabeca;
    node *proximo;
    while(no != NULL)
    {
        proximo = no->proximo;
        free(no);
        no = proximo;
    }   
    free(l);
}
/* Cria a lista arco_lugar */
lista_arco_lugar *cria_lista_arco_lugar(void)
{
    lista_arco_lugar *l = malloc(sizeof(lista_arco_lugar));
    l->cabeca = NULL;
    return l; /* retorna seu endereço */
}
/*Adiciona um arco lugar na lista*/
void adiciona_na_lista_arco_lugar(lista_arco_lugar *l, arco_lugar *valor)
{
    /*Cria um novo node com o valor a ser adicionado*/
    node_arco_lugar *novo_node = malloc(sizeof(node_arco_lugar));
    novo_node->conteudo = valor;
    novo_node->proximo = NULL;

    /*Verifica se a lista é vazia*/
    /*Se sim, adiciona o novo_node na cabeça*/
    if(l->cabeca == NULL)
    {
        l->cabeca = novo_node;
    }
    else {
        /*Se não for vazia, procura o último node da lista*/
        node_arco_lugar *no;
        for(no = l->cabeca; no->proximo != NULL; no = no->proximo);

        /*Nesse último node, adiciona o novo_node como proximo*/
        no->proximo = novo_node;
    }
}
node_arco_lugar *busca_elemento_por_indice_arco_lugar(lista_arco_lugar *l, int indice)
{
    int i;
    node_arco_lugar *no = l->cabeca;

    for(i = 0; i < indice; i++)
    {
        if(no == NULL)
            break;

        no = no->proximo;
    }

    return no;
}
/* Imprime a lista, elemento por elemento*/
void imprime_lista_arco_lugar(lista_arco_lugar *l)
{
    node_arco_lugar *no;
    for(no = l->cabeca; no != NULL; no = no->proximo)
    {
        printf(" al ");
    }
}
/* Limpa a alocação dinamica da lista*/
void limpa_lista_arco_lugar(lista_arco_lugar *l)
{
    node_arco_lugar *no = l->cabeca;
    node_arco_lugar *proximo;
    while(no != NULL)
    {
        proximo = no->proximo;
        free(no->conteudo);
        free(no);
        no = proximo;
    }   
    free(l);
}
/* Cria a lista arco_transicao */
lista_arco_transicao *cria_lista_arco_transicao(void)
{
    lista_arco_transicao *l = malloc(sizeof(lista_arco_transicao));
    l->cabeca = NULL;
    return l;
}
void adiciona_na_lista_arco_transicao(lista_arco_transicao *l, arco_transicao *valor)
{
    /*Cria um novo node com o valor a ser adicionado*/
    node_arco_transicao *novo_node = malloc(sizeof(node_arco_transicao));
    novo_node->conteudo = valor;
    novo_node->proximo = NULL;

    /*Verifica se a lista é vazia*/
    /*Se sim, adiciona o novo_node na cabeça*/
    if(l->cabeca == NULL)
    {
        l->cabeca = novo_node;
    }
    else {
        /*Se não for vazia, procura o último node da lista*/
        node_arco_transicao *no;
        for(no = l->cabeca; no->proximo != NULL; no = no->proximo);

        /*Nesse último node, adiciona o novo_node como proximo*/
        no->proximo = novo_node;
    }
}
node_arco_transicao *busca_elemento_por_indice_arco_transicao(lista_arco_transicao *l, int indice)
{
    int i;
    node_arco_transicao *no = l->cabeca;

    for(i = 0; i < indice; i++)
    {
        if(no == NULL)
            break;

        no = no->proximo;
    }

    return no;
}
/* Imprime a lista, elemento por elemento*/
void imprime_lista_arco_transicao(lista_arco_transicao *l)
{
    node_arco_transicao *no;
    for(no = l->cabeca; no != NULL; no = no->proximo)
    {
        printf(" at ");
    }
}
/*Libera a memória alocada para a lista e seus nós*/
void limpa_lista_arco_transicao(lista_arco_transicao *l)
{
    node_arco_transicao *no = l->cabeca;
    node_arco_transicao *proximo;
    while(no != NULL)
    {
        proximo = no->proximo;
        free(no->conteudo);
        free(no);
        no = proximo;
    }   

    free(l);
}
/* Cria a lista transicao */
lista_transicao *cria_lista_transicao(void)
{
    lista_transicao *l = malloc(sizeof(lista_transicao));
    l->cabeca = NULL;
    return l;
}

/*Adiciona um valor ao final da lista*/
/*O valor deve ser do mesmo tipo que o conteúdo do node*/
void adiciona_na_lista_transicao(lista_transicao *l, transicao *valor)
{
    /*Cria um novo node com o valor a ser adicionado*/
    node_transicao *novo_node = malloc(sizeof(node_transicao));
    novo_node->conteudo = valor;
    novo_node->proximo = NULL;

    /* Verifica se a lista é vazia*/
    /* Se sim, adiciona o novo_node na cabeça*/
    if(l->cabeca == NULL)
    {
        l->cabeca = novo_node;
    }
    else {
        /* Se não for vazia, procura o último node da lista*/
        node_transicao *no;
        for(no = l->cabeca; no->proximo != NULL; no = no->proximo);

        /* Nesse último node, adiciona o novo_node como proximo*/
        no->proximo = novo_node;
    }
}

/*Recebe uma lista e um indice*/
/*Retorna o elemento posicionado no índice desejado*/
/*Caso o indice não exista, a função retorna null*/
node_transicao *busca_elemento_por_indice_lista_transicao(lista_transicao *l, int indice)
{
    int i;
    node_transicao *no = l->cabeca;

    for(i = 0; i < indice; i++)
    {
        no = no->proximo;
    }

    return no;
}
/* Imprime a lista, elemento por elemento */
void imprime_lista_transicao(lista_transicao *l)
{
    node_transicao *no;
    for(no = l->cabeca; no != NULL; no = no->proximo)
    {
        printf(" tr ");
    }
}
//Libera a memória alocada para a lista e seus nós
void limpa_lista_transicao(lista_transicao *l)
{
    node_transicao *no = l->cabeca;
    node_transicao *proximo;
    while(no != NULL)
    {
        proximo = no->proximo;
        free(no->conteudo);
        free(no);
        no = proximo;
    }   

    free(l);
}
