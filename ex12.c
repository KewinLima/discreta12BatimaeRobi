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

//Protótipos
int *cria_lugares(int numero_de_lugares);
    
int main(void)
{
    //q[0] = qunatidade de lugar, 
    //q[1] = quantidade de trans,
    //q[2] = quantidade de lugartoke,
    //q[3] = qarcotrans,
    //q[4] = qarcolugar,

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

    free(lugar); // limpando a alocação dinamica
    fclose(a);// fechando o arquivo de entrada
    return 0;
}


int *cria_lugares(int numero_de_lugares)
{
    return malloc(numero_de_lugares * (sizeof(int))); //Alocação dinâmica
}
