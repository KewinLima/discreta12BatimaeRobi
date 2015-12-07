#define NENTRADA 5;
#include <stdio.h>

int main(void)
{
    //q[0] = qunatidade de lugar, 
    //q[1] = quantidade de trans,
    //q[2] = quantidade de lugartoke,
    //q[3] = qarcolugar,
    //q[4] = qarcotrans,

    int n;
    
    FILE *a = fopen("sena1450.txt","r"); // Abrindo o arquivo de entrada

    for(n=0;n<NENTRADA;n++) //Laço para o vetor q
    {
        fscanf("%d", &q[n]);
    }

    int *lugares = cria_lugares(q[0]);

    free(lugares); // limpando a alocação dinamica
    fclose(a);// fechando o arquivo de entrada
    return 0;
}

int *cria_lugares(int numero_de_lugares)
{
    return malloc(numero_de_lugares * (sizeof int));
}
