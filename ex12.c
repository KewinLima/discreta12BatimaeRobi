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

//Protótipos
int *cria_lugares(int numero_de_lugares);
    
int main(void)
{
    //q[0] = qunatidade de lugar, 
    //q[1] = quantidade de trans,
    //q[2] = quantidade de lugartoke,
    //q[3] = qarcolugar,
    //q[4] = qarcotrans,

    int n,q[NENTRADA],n1;
    
    FILE *a = fopen("entrada.txt","r"); // Abrindo o arquivo de entrada

    for(n=0;n<NENTRADA;n++) //Laço para o vetor q
    {
        fscanf(a,"%d", &q[n]);
    }
    int *lugar = cria_lugares(q[0]); //cria n-lugares
    
    for(n=0;n<( q[2] );n++)
    {
         fscanf(a,"%d", &n1);
         fscanf(a,"%d", &lugar[n1]);
         printf(" %d %d\n", n1, lugar[n1]);
    }
    
    
    free(lugar); // limpando a alocação dinamica
    fclose(a);// fechando o arquivo de entrada
    return 0;
}


int *cria_lugares(int numero_de_lugares)
{
    return malloc(numero_de_lugares * (sizeof(int))); //Alocação dinâmica
}
