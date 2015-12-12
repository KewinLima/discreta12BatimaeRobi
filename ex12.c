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


int main(void)
{

    int n,q[NENTRADA],n1; 
    FILE *a = fopen("entrada.txt","r"); // Abrindo o arquivo de entrada

    for(n=0;n<NENTRADA;n++) //Laço para o vetor q
    {
        fscanf(a,"%d", &q[n]);
    }
    int *lugar = cria_lugares(q[0]); //cria n-lugares
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
    // arct[n][0] = lugar de origem
    // arct[n][1] = quantidade de tokens consumidos
    // arct[n][2] = transição de destino

    for(n=0; n<q[4]; n++)
    {
        for(n1=0; n1<QINFO; n1++)
        {
            fscanf(a,"%d", &arcolugar[n][n1]);
        }
    }

    // arlg[n][0] = transição de origem
    // arlg[n][1] = quantidade de tokens consumidos
    // arlg[n][2] = lugar de destino

    free(lugar); // limpando a alocação dinamica
    fclose(a);// fechando o arquivo de entrada
    return 0;
}

lista *cria_lista(void)
{
    lista *l = malloc(sizeof(lista));
    l->cabeca = NULL
    return l;
}
