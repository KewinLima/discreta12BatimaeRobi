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

int *cria_lugares(int numero_de_lugares);
    
int main(void)
{
    //q[0] = qunatidade de lugar, 
    //q[1] = quantidade de trans,
    //q[2] = quantidade de lugartoke,
    //q[3] = qarcolugar,
    //q[4] = qarcotrans,

    int n,q[NENTRADA];
    
    FILE *a = fopen("entrada.txt","r"); // Abrindo o arquivo de entrada

    for(n=0;n<NENTRADA;n++) //Laço para o vetor q
    {
        fscanf(a,"%d", &q[n]);
    }

    int *lugar = cria_lugares(q[0]);
    lugar[0] = 1;
    lugar[1] = 2;
    lugar[2] = 3;

    for(n=0;n<3;n++)
        printf("%d", lugar[n]);

    free(lugar); // limpando a alocação dinamica
    fclose(a);// fechando o arquivo de entrada
    return 0;
}

int *cria_lugares(int numero_de_lugares)
{
    return malloc(numero_de_lugares * (sizeof(int)));
}
