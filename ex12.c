/* *******************************************************
 **| Programa   : Rede de Petri com Processos Paralelos|**
 **| Autores    : Isabella Galvão e Kewin Lima         |**
 **| Orientador : Ruben Carlo                          |**
 *********************************************************
 **/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#define NENTRADA 5
#define QINFO 3

//************** Definição dos tipos para as listas  ****************
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


int main(void)
{

    int n,q[NENTRADA],n1; 
    FILE *a = fopen("entrada.txt","r"); /* Abrindo o arquivo de entrada*/

    for(n=0;n<NENTRADA;n++) /*Laço para o vetor q*/
    {
        fscanf(a,"%d", &q[n]);
    }
    int *lugar = cria_lugares(q[0]); /*cria n-lugares*/
    int arcotrans[q[3]][3];  
    int arcolugar[q[4]][3];

    for(n=0;n<( q[2] );n++)
    {
        fscanf(a,"%d", &n1);
        fscanf(a,"%d", &lugar[n1]);
        printf(" %d %d\n", n1, lugar[n1]);
    }
    for(n=0; n<q[3]; n++)
    {
        for(n1=0; n1 < QINFO; n1++)
        {
            fscanf(a, "%d", &arcotrans[n][n1]);
        }
    }    
    for(n=0; n<q[4]; n++)
    {
        for(n1=0; n1<QINFO; n1++)
        {
            fscanf(a,"%d", &arcolugar[n][n1]);
        }
    }

    fclose(a);/* fechando o arquivo de entrada*/
    return 0;
}

lista *cria_lista(void)
{
    lista *l = malloc(sizeof(lista));
    l->cabeca = NULL
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

