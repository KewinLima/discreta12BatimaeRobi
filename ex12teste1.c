#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#define REPETIR 100

int Qtran=2,Qlugar=4,Qlugar_token=2,Qarco_t=3,Qarco_l=3, Arco_l[3][3],Arco_t[3][3],lugar[4];
void simulador(int tran_n);
void threads(int Qtran);//char chave);
void *transicao_pt(void *arg);
void arco_transicoes(int tran_n);
int arco_lugares(int tran_n,int numero);

int main (void)
{
    srand(time(NULL));

    lugar[0] = 6;
    lugar[1] = 0;
    lugar[2] = 1;
    lugar[3] = 0;

    Arco_l[0][0]= 0; 
    Arco_l[0][1]= 2; 
    Arco_l[0][2]= 0; 

    Arco_l[1][0]= 1; 
    Arco_l[1][1]= 1; 
    Arco_l[1][2]= 1; 

    Arco_l[2][0]= 2; 
    Arco_l[2][1]= 1; 
    Arco_l[2][2]= 1; 

    Arco_t[0][0]= 0; 
    Arco_t[0][1]= 1; 
    Arco_t[0][2]= 1; 

    Arco_t[1][0]= 1; 
    Arco_t[1][1]= 1; 
    Arco_t[1][2]= 2; 

    Arco_t[2][0]= 1; 
    Arco_t[2][1]= 1; 
    Arco_t[2][2]= 3; 

    int n2;
    //printf(" 88888888888888888888 %d 88888888888888 linha 47 ", Qtran);
    threads(Qtran);//, 'a'); /*chamar a função transicao*/
   // printf("\n\n Resultado final \n\n");
   // for(n2=0;n2<Qlugar;n2++)
   // {
   //     printf("- lugar[%d] = %d tokens\n",n2,lugar[n2]);
  //  }
    

}

void threads(int Qtran)//, char chave)
{
    pthread_t thread[Qtran];
    int i,n1,arg[Qtran];
    for(i=0; i < Qtran; i++)
    {
        arg[i]=i;
        printf(" arg[%d] = %d  &arg[%d] = %d \n",i,i,arg[i],&arg[i]);
        pthread_create(&thread[i], NULL, transicao_pt, (void*) &arg[i]);
    }
    //pthread_exit(NULL); 
}
void *transicao_pt(void *arg)
{
    int *pvalor;
    int n2,x;
    pvalor = arg;
    printf("\n# Thread da transicao %d executando \n", *pvalor);
    for(n2=0;n2<Qlugar;n2++)
    {
        printf("- lugar[%d] = %d tokens R: transicao %d \n",n2,lugar[n2],*pvalor);
    }
    scanf("%d", &x);
   // arco_lugares(*pvalor, 0);
}

int arco_lugares(int tran_n, int numero)
{
    int n1,sorteio;
    printf("# Ok, nesse momento estamos trabalhando com os arcolugares da transicao %d \n", tran_n);
    for(n1=0;n1<Qarco_l;n1++)
    {
        printf("# Certo, vez do arcolugar numero: %d da transicao %d\n", n1, tran_n);
        if(Arco_l[n1][2] == tran_n)
        {
            printf("# Ok, o arcolugar %d se refere a transicao %d\n",n1, tran_n);
            if((lugar[Arco_l[n1][0]] - Arco_l[n1][1] ) >= 0)
            {
                printf("# Legal, temos tokens suficientes no lugar %d desse arco lugar \n", Arco_l[n1][0]);
                printf("# Temos: %d e necessitamos de %d \n",lugar[Arco_l[n1][0]],Arco_l[n1][1]);
                lugar[Arco_l[n1][0]] = lugar[Arco_l[n1][0]] - Arco_l[n1][1];
                printf("# Tirei %d do lugar %d\n", Arco_l[n1][1], Arco_l[n1][0]);
                printf("# Ok vamos agora trabalhar com os arco_transicoes da trans %d: \n", tran_n);
                /*O valor da transicao é aceito*/
                for(n1=0;n1<Qarco_t;n1++)
                {
                    printf("# Ok, vejamos se o arcotransicao numero %d e' referente a transicao %d \n",n1, tran_n);
                    if(Arco_t[n1][0] == tran_n)
                    {
                        printf("# Ok, esse arcotransicao e' referente a transicao %d \n", tran_n);
                        sorteio = rand()%2;
                        printf("# Vamos ao sorteio: ");
                        if(sorteio == 0)
                        {
                            printf("# Perdeu no sorteio \n");
                            continue;
                        }
                        else if(sorteio == 1)
                        {
                            printf("# Ganhou o sorteio trans %d ativada\n", tran_n);/* transi 1 envia 1 token para o lugar 2*/
                            lugar[Arco_t[n1][2]] = lugar[Arco_t[n1][2]] + Arco_t[n1][1];
                            printf("# Adicionei %d ao lugar %d \n",Arco_t[n1][1],Arco_t[n1][2]);
                            /* O lugar daquela transicao recebe o valor do token */
                        }
                    }
                    else
                    {
                        printf("# Esse arcotransicao nao tem relacao coma transicao %d\n", tran_n);
                        continue;
                    }
                }
            }
            else
            {
                printf("# Pessima noticia, Voce nao tem tokens suficientes no lugar %d \n", Arco_l[n1][0]);
                printf("# Precisaria de %d tokens mas so' tem %d \n",Arco_l[n1][1],lugar[Arco_l[n1][0]]);
                continue;
                /* Caso não tenha o valor pedido pela transicao. */
            }
        }
        else
        {
            printf("# Esse arcolugar nao tem relacao com a transicao %d\n", tran_n);
            continue;
        }
    }
/*
    for(n1=0;n1<Qlugar;n1++)
    {
        if( Arco_l[n1][2] == tran_n)
        {        
            if( lugar[Arco_l[n1][0]] != 0 && lugar[Arco_l[n1][0]] >= Arco_l[n1][1])
            {
                n2 = 1;   
            }
        } 
    } 
*/
 //   if(n2 == 0)//Não ha tokens suficientes para essa transicao em nem um lugar
 //       return 0;// threads(tran_n, 'f');
    
 //   else if(n2 = 1)//continua até acabar os tokens
        numero++;
       // if(numero < REPETIR)    
       // {
        //    return arco_lugares(tran_n, numero);
       // }
}
