/********************************************************
 **| Programa   : Rede de Petri com Processos Paralelos|**
 **| Autores    : Isabella Galvão e Kewin Lima         |**
 **| Orientador : Ruben Carlo                          |**
 *********************************************************
 **/

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <allegro.h>
#include <time.h>
#include <math.h>

#define CORBRANCO        (makecol(255,255,255))
#define CORPRETO         (makecol(0,0,0))
#define TRANSPARENTE     (makecol(255,0,255))
#define CORCINZA         (makecol(160,160,160))
#define CORAZUL          (makecol(0, 0, 255))
#define CORVERDE         (makecol(0, 255, 0))
#define CORAMARELO       (makecol(255,255,100))
#define CORVERMELHO      (makecol(255, 0, 0))
#define TAMANHO_X        800
#define TAMANHO_Y        800
#define TAMANHO_C        50
#define NOME_IMAGEM      "ex12.bmp"
#define NOME_ENTRADA     "entrada-petri-1.txt"
#define BLOCO_DE_ENTRADA 5
#define PORCENTAGEM      50
#define DEBUG            1
#undef  DEBUG /* Caso queira um debug,por favor comente essa linha. */

/*************** Definição dos tipos para as listas  ****************/
/*Define uma transição*/
typedef struct struct_transicao transicao;
struct struct_transicao
{
    int coletor;
    int emissor;
    int indice;
};

/*Define um node que armazena um inteiro*/
typedef struct struct_node node;
struct struct_node
{
    int conteudo;
    node *proximo;
    int indice;
};
/*Define um arco transição*/
typedef struct struct_arco_transicao arco_transicao;
struct struct_arco_transicao
{
    node *destino;
    transicao *origem;
};

/*Define um arco lugar*/
typedef struct struct_arco_lugar arco_lugar;
struct struct_arco_lugar
{
    transicao *destino;
    node *origem;
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
node *busca_elemento_por_indice(lista *l, int indice);
void imprime_lista(lista *l, char entrada_lugar);
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

/*                     Allegro                         */
void imprimie_lugar_allegro(lista *l);

/*                     Thread                          */
void *transicao_pt(void *x);
void *imprima(void *x);
/****************** Fim dos Protótipos ******************/
int arg[5];/*variavel que ira guardar o endereço das listas */

int main(void)
{
    srand(time(NULL));
    int n;                                   /* variável para o laço da leitura do primeiro bloco de entrada */
    int x;                                   /* Variavel para determinar o limite do laço */
    lista *lugares;                          /*Cria ponteiro de  lista de lugares, que serão inteiros*/
    lista_arco_lugar *arcos_lugar;           /*Cria ponteiro de lista de arcos lugar*/
    lista_arco_transicao *arcos_transicao;   /*Cria ponteiro de lista de arcos transição*/
    lista_transicao *transicoes;             /*Cria ponteiro de lista de inteiros*/
    lista *entrada = cria_lista();           /*Cria uma lista para armazenar as entradas iniciais*/
    FILE *arquivo = fopen(NOME_ENTRADA,"r"); /* Abrindo o arquivo de entrada*/

    for(n = 0; n < BLOCO_DE_ENTRADA; n++)    /*Laço para leitura do primeiro bloco de entradas*/
    {
        int leitura;
        fscanf(arquivo,"%d", &leitura);

        adiciona_na_lista(entrada, leitura);
    }

    /*Lê quantidade de lugares requerida e cria x lugares com 0 (zero) tokens*/
    x = busca_elemento_por_indice(entrada, 0)->conteudo;
    lugares = cria_lista();
    for(n = 0; n < x; n++)
        adiciona_na_lista(lugares, 0);

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
    arcos_lugar = cria_lista_arco_lugar();
    for(n = 0; n < x; n++)
    {
        int lugar;
        int quantidade;
        int transicao;
        arco_lugar *al = malloc(sizeof(arco_lugar));

        fscanf(arquivo, "%d", &lugar);
        fscanf(arquivo, "%d", &quantidade);
        fscanf(arquivo, "%d", &transicao);
        al->origem = busca_elemento_por_indice(lugares, lugar);
        al->destino = busca_elemento_por_indice_lista_transicao(transicoes, transicao)->conteudo;
        al->destino->coletor = quantidade;
        adiciona_na_lista_arco_lugar(arcos_lugar, al);
    }

    /*Lê as informações sobre os arcos lugar, cria cada um e os guarda na lista adequada*/
    x = busca_elemento_por_indice(entrada, 3)->conteudo;
    arcos_transicao = cria_lista_arco_transicao();
    for(n = 0; n < x; n++)
    {
        int lugar;
        int quantidade;
        int transicao;
        arco_transicao *at = malloc(sizeof(arco_transicao));

        fscanf(arquivo, "%d", &transicao);
        fscanf(arquivo, "%d", &quantidade);
        fscanf(arquivo, "%d", &lugar);
        at->destino = busca_elemento_por_indice(lugares, lugar);
        at->origem = busca_elemento_por_indice_lista_transicao(transicoes, transicao)->conteudo;
        at->origem->emissor = quantidade;
        adiciona_na_lista_arco_transicao(arcos_transicao, at);
    }

    node *no;
    no = entrada->cabeca;
    no = no->proximo;
    int Qtran= no->conteudo;
    pthread_t threads[Qtran];
    int i,n1,art; /*arg[6];*/

    arg[0] = transicoes;
    printf(" endereco transicoes arg[%d] = %d \n",0, &arg[0]);//entrada);
    arg[1] = lugares;
    printf(" endereco lugares    arg[%d] = %d \n",1, &arg[1]);//lugares);
    arg[2] = arcos_lugar;
    printf(" endereco arcos_luga arg[%d] = %d \n",2, &arg[2]);//arcos_lugar);
    arg[3] = arcos_transicao;
    printf(" endereco arco_t     arg[%d] = %d \n",3, &arg[3]);//arcos_transicao);
    arg[4] = entrada;
    printf(" endereco entrada    arg[%d] = %d \n",4, &arg[4]);//transicoes);
    printf(" !!! Qtran = %d\n", Qtran);
    
    for(i=0; i<Qtran ;i++)
    {
        (int)art;
        art = i;
        //printf(" QTRAN = %d \n",Qtran);
   //     pthread_create(&threads[i], NULL,transicao_pt, (void*) &art);
        printf("-> Transicao %d criada <-\n",i);
        // for(n1=0;n1<1000;n1++)
        // {
        //    printf(" dentro do laço\n");
        // }
        sleep(1);
        //arg[5] = i;
        //transicao_pt((void*) &i);/*&arg);*/
    }
    for(i=0; i<Qtran; i++)
    {
   //     pthread_join(&threads[i],NULL);
    }
    // printf(" -> TODAS AS THREADS TERMINARAM!!\n");
    imprimie_lugar_allegro(entrada);
    /*Com o objetivo de um debug funções de imprimir na tela*/

#ifdef DEBUG
    imprime_lista(entrada, 'e');
    imprime_lista(lugares, 'l');
    imprime_lista_arco_lugar(arcos_lugar);
    imprime_lista_arco_transicao(arcos_transicao);
    imprime_lista_transicao(transicoes);
#endif
    /* Ao fim do código, limpa todas as alocações dinâmicas realizadas */
    limpa_lista(lugares);
    limpa_lista(entrada);
    limpa_lista_arco_lugar(arcos_lugar);
    limpa_lista_arco_transicao(arcos_transicao);
    limpa_lista_transicao(transicoes);

    fclose(arquivo);/* fechando o arquivo de entrada*/
 //   pthread_exit(NULL);
    return EXIT_SUCCESS;
}
/****************Inicio das funções - Inicio Simulador****************/
void *imprima(void *x)
{
    int *X;
    X=x;
    printf(" transicao %d \n", *X);
    sleep(1);
    printf(" transicao %d terminada.. \n",*X);
}

void *transicao_pt(void *x)
{
    int Qtran,*pvalor,numero =0,*cont;
    cont = x;
    pvalor = arg;
    lista *entradas;
    entradas = pvalor[4];
    node *no_e;
    no_e = entradas->cabeca;
    Qtran=no_e->proximo->conteudo;
    int tran_n[Qtran];
    tran_n[*cont] = *cont;
  //  printf("!!!! tran_n[%d] = %d !!!\n Qtran = %d\n",*cont,tran_n[*cont],Qtran);
 //   lista *entradas;
    lista *lugar;
    lista_arco_lugar *a_lugar;
    lista_arco_transicao *a_transicao;
    lista_transicao *transicoes;
    //tran_n = *pvalor;
/*
    printf(" linha 308\n");
    printf(" Endereço de tran_n = %d\n", tran_n);
    printf(" Valor de tran_n = %d\n", tran_n);
    printf(" Endereço de pvalor = %d\n", pvalor);
    printf(" Thread da transicao %d executando \n", tran_n);
    */
    // pvalor=pvalor-1;
    for(numero=0; numero<5; numero++)
    {
        switch(numero)
        {
            case 4:
       //         printf(" entradas  pvalor[%d] == %d \n",numero,pvalor);
                entradas = *pvalor;
                break;
            case 3:
        //        printf(" a_t    pvalor[%d] == %d \n",numero,pvalor);
                a_transicao = *pvalor;
                break;
            case 2:
       //         printf(" a_l     pvalor[%d] == %d \n",numero,pvalor);
                a_lugar = *pvalor;
                break;
            case 1:
     //           printf(" lugar   pvalor[%d] == %d \n",numero,pvalor);
                lugar = *pvalor;
                break;
            case 0:
    //            printf(" transi  pvalor[%d] == %d \n",numero,pvalor);
                transicoes = *pvalor;
                break;
        }
        pvalor=pvalor+1;
    }

    /* Referente a lista */ 
    int parada[Qtran];

    node *no;
    // node *no_e;
    no = lugar->cabeca;
    // no_e = entradas->cabeca;
    no= no->proximo->conteudo;

    node_arco_lugar *no_e_al;
    node_arco_transicao *no_e_at;
    arco_transicao *at_e;
    arco_lugar *al_e;
    //no_e_al = entradas->cabeca;

    int Qlugar,/*Qtran,*/Qarco_t,Qarco_l;
    Qlugar  = no_e->conteudo;
    // Qtran   = no_e->proximo->conteudo;
    Qarco_t = no_e->proximo->proximo->proximo->conteudo;
    Qarco_l = no_e->proximo->proximo->proximo->proximo->conteudo;
    int vazio[Qtran],n[Qtran],n1[Qtran],n2[Qtran],n3[Qtran],sorteio;

    /* Referente a lista_arco_lugar */

    node_arco_lugar *no_al;
    arco_lugar *al_al;
    node *lugar_al;
    transicao *t_al;
    no_al = a_lugar->cabeca;

    al_al = no_al->conteudo; 
    t_al  =  al_al->destino;
    lugar_al = al_al->origem;
    /* Referente a lista_arco_transicao */

    node_arco_transicao *no_at;
    arco_transicao *at_at;
    node *lugar_at;
    transicao *t_at;

    no_at = a_transicao->cabeca;
    at_at = no_at->conteudo;
    t_at = at_at->origem;
    lugar_at= at_at->destino;

    /*Referente as transições */

    node_transicao *no_t;
    transicao *t;

    //    printf(" transicao numero %d \n", tran_n);
    //    printf(" arcolugar parte de %d \n", al_al->origem);
    //    printf(" Arcolugar chega em %d \n", al_al->destino);
    //    printf(" Quantidade de tokens no lugar = %d \n", lugar_al->conteudo);
    //    printf(" Arcotransicao parte de %d \n", at_at->origem);
    //    printf(" Arcotransicao chega em %d \n", at_at->destino);
    //    printf(" Quantidade de tokens no lugar = %d \n", lugar_at->conteudo);
    //    printf(" Valor da transicao %d \n", t_al->coletor);
    //    printf(" Valor da doacao %d \n",t_al->emissor);

    printf("# Ok,estamos trabalhando com a transicao %d \n", tran_n[*cont]);

    if(tran_n[*cont] == 0)// Caso seja a transicao 0, recebe a cabeca da lista 
    {
        no = transicoes->cabeca;
    }
    if(tran_n[*cont] != 0)// Caso seja aponta para os proximo 
    {
        no = transicoes->cabeca;
        for(n1[tran_n[*cont]]=0;n1[tran_n[*cont]]<tran_n[*cont];n1[tran_n[*cont]]++)
        {
            no = no->proximo;   
        }
    }
    t = no->conteudo;
    parada[*cont] = 0;

    while(parada[tran_n[*cont]] != 999)
    {
        vazio[tran_n[*cont]] = 0;
        parada[tran_n[*cont]]++;
        for(n1[tran_n[*cont]]=0;n1[tran_n[*cont]]< Qarco_l;n1[tran_n[*cont]]++)
        {
            if(n1[tran_n[*cont]] == 0)
            {
                no_al = a_lugar->cabeca;
                al_al = no_al->conteudo;
            }
            else
            {
                no_al = a_lugar->cabeca;
                for(n[tran_n[*cont]]=0; n[tran_n[*cont]]<n1[tran_n[*cont]] ;n[tran_n[*cont]]++)// Encontra o elemento da lista numero n1
                {
                    no_al = no_al->proximo;
                    al_al = no_al->conteudo;
                }
            }

            lugar_al = al_al->origem; // Após encontrar o lugar desejado relaciona o mesmo com lugar_al 
            printf("# Certo, vez do arcolugar numero: %d em relacao a transicao %d\n",n1[tran_n[*cont]], tran_n[*cont]);
            if(al_al->destino == t)
            {
                //printf(" Endereco de t = %d \n Endereco de al_al->destino == %d \n",t,al_al->destino);
                printf("# Sim, o arcolugar %d se refere a transicao %d\n",n1[tran_n[*cont]], tran_n[*cont]);
                if( (lugar_al->conteudo) - (t->coletor) >=0 )// Condicao para acionar a transicao 
                {
                    n2[tran_n[*cont]]=0;
                    for(no_e_al=a_lugar->cabeca; no_e_al != NULL; no_e_al = no_e_al->proximo)
                    {
                        //                      printf(" Linha 455 \n");
                        al_e = no_e_al->conteudo;
                        if(al_e->origem == lugar_al)
                            break;
                        n2[tran_n[*cont]]++;
                    }

                    printf("# Legal, temos tokens suficientes no lugar %d, onde parte o arco lugar %d \n", n2[tran_n[*cont]],n1[tran_n[*cont]]);
                    vazio[tran_n[*cont]] = 0;
                    lugar_al->conteudo = lugar_al->conteudo - t->coletor;
                    printf("# Tirei %d do lugar %d que agora tem %d\n",t->coletor, n2[tran_n[*cont]],lugar_al->conteudo);
                    printf("# Ok, vamos agora trabalhar com os arcos transicoes da transicao %d\n",tran_n[*cont]); 
                    for(n3[tran_n[*cont]]=0;n3[tran_n[*cont]]<Qarco_t;n3[tran_n[*cont]]++)
                    {
                        printf("# Certo, vez do arcotransicao numero: %d em relacao a transicao %d\n",n3[tran_n[*cont]],tran_n[*cont]);
                        if(n3[tran_n[*cont]] == 0)
                        {
                            no_at = a_transicao->cabeca;
                            at_at = no_at->conteudo;

                        }
                        if(n3[tran_n[*cont]] != 0)
                        {
                            no_at = a_transicao->cabeca;
                            for(n[tran_n[*cont]]=0; n[tran_n[*cont]]<n3[tran_n[*cont]] ;n[tran_n[*cont]]++)// Encontra o elemento da lista numero n2
                            {
                                no_at = no_at->proximo;
                                at_at = no_at->conteudo;
                            }
                        }

                        lugar_at = at_at->destino;
                        //        printf(" Endereco de t = %d \n Endereco de at_at = %d \n",t,at_at->destino);
                        if(at_at->origem == t)
                        {
                            printf("# Sim, esse arco transicao e' referente a transicao %d\n",tran_n[*cont]);
                            n2[tran_n[*cont]]=0;
                            for(no_e_at = a_transicao->cabeca; no_e_at !=NULL; no_e_at = no_e_at->proximo)
                            {
                                //                            printf(" Linha 492 \n");
                                at_e = no->conteudo;
                                if(at_e->origem == t)
                                    break;
                                n2[tran_n[*cont]]++;
                            }
                            printf("# Vamos ao sorteio: ");
                            sorteio = rand()%( 100/PORCENTAGEM );
                            if(sorteio != 0)
                            {
                                printf(" Perdeu no sorteio \n");
                                continue;
                            }
                            else
                            {
                                printf(" Ganhou o sorteio trans %d ativada\n", tran_n[*cont]);
                                lugar_at->conteudo = lugar_at->conteudo + t->emissor;
                                printf("# Adicionei %d ao lugar %d \n",t->emissor, n2[tran_n[*cont]]);// <- errado);
                                printf("# Que agora tem %d \n",lugar_at->conteudo);
                            }
                        }
                        else
                        {
                            printf("# Esse arcotransicao nao tem relacao com a transicao %d\n", tran_n[*cont]);
                            continue;
                        }
                    }
                }
                else
                {   
                    printf("# Pessima noticia, Voce nao tem tokens suficientes no lugar %d \n",n1[tran_n[*cont]]);// <-
                    printf("# Precisaria de %d tokens mas so' tem %d \n",t->coletor,lugar_al->conteudo);
                }

            }
            else
            {
                vazio[tran_n[*cont]] ++;
                printf("# O arcolugar %d nao tem relacao com a transicao %d\n",n1[tran_n[*cont]], tran_n[*cont]);
                continue;
            }
        }
    }
    printf(" # FIM DA SIMULACAO \n");
    int n4=0;
    if(*cont+1 == Qtran)
    {
        printf(" Resultado final: \n");
        for(no = lugar->cabeca; no != NULL; no = no->proximo)
        {
            printf(" lugar %d | Quantidade de tokens %d\n",n4, no->conteudo);
            n4++;
        }
    }
    pthread_exit(NULL);
}
void imprimie_lugar_allegro(lista *l)
{
    node *no;
    no = l->cabeca;
    int Qtran,n,n2,n3,total;
    Qtran = no->conteudo;
    
    /* referente aos lugares */
    if(Qtran%2 ==0)//par
        total=Qtran;
    else if(Qtran%2==1)// Impar
        total=Qtran+1;
    float x_l[total], y_l[total];
    
    /*referente a origem */
    int xo,yo; 
    xo=(TAMANHO_X/2);
    yo=(TAMANHO_Y/2);

    /* Declaração das variáveis que irmão guardar as imagens */
    BITMAP *buff;  /* Arquivos de bitmap */
    PALETTE pal;   /* Paletas            */

    if(install_allegro(SYSTEM_NONE, &errno, atexit)!=0) /* Instala o allegro para que ele seja usado no codigo */
        exit(EXIT_FAILURE);

    set_color_depth(16);/* Definindo o numero de cores usadas  */
    get_palette(pal);   /* Pega as pelates para usar no codigo */

    /* Criando um buffer para a imagem.*/
    buff = create_bitmap(TAMANHO_X,TAMANHO_Y);
    if(buff == NULL)
    {
        printf(" Não foi possivel criar o buffer!\n");
        exit(EXIT_FAILURE);
    }
    else
        printf(" Imagem criada com sucesso! \n");
    
    int limite,parcela,raio;
    n2 = TAMANHO_X;
    n3 = TAMANHO_Y;
    if(n2<=n3)
        raio = (TAMANHO_X - 2*TAMANHO_C)/2;
    else if(n3<n2)
        raio = (TAMANHO_Y - 2*TAMANHO_C)/2;
    
    limite = total/2;
    parcela = raio/limite;
    x_l[0]= TAMANHO_C*2; // Defino a posicao x do mais proximo a parede
    x_l[total/2] = TAMANHO_X-TAMANHO_C*2; // Define a posicao x do mais afastado do centro
    y_l[0]= yo;
    y_l[total/2]=yo;
    
    n3=1;
    for( n=1; n<total ;n++)
    {
        if( n == limite)
            break;
        if(n < limite)
            x_l[n] = parcela*n + TAMANHO_C;
        if(n>limite)
        {
            x_l[n] = parcela*n3 + TAMANHO_C;
            n3++;
        }
    }
    for( n=1; n<total ;n++)
    {
        if(n == limite)
            break;
        
        if(n < limite)
            y_l[n] = y_l[0] - parcela*n + TAMANHO_C;
        
        if(n > limite)
            y_l[n] = y_l[0] + parcela*n - TAMANHO_C;
    }

    for(n=0; n<(Qtran) ; n++)
    {
        if(Qtran%2 ==1 && n+1==Qtran)
        {
            break;
        }
        circle(buff, x_l[n], y_l[n], TAMANHO_C, CORAMARELO);/* desenha um circulo */
    }
    /* textprintf_ex(buff, font, 50, 50, CORVERDE, CORPRETO, "Teste do circulo!");*/

    save_bitmap(NOME_IMAGEM, buff, pal);/* Salva a imagem no diretorio */
    destroy_bitmap(buff);               /* Destroi a imagem do buffer  */
    allegro_exit();                     /* Termina o allegro           */

#ifdef DEBUG
    printf("Imagem %s salva com sucesso!\n", NOME_IMAGEM);
#endif
}
END_OF_FUNCTION()

    /****************Inicio Cria lista - Fim Simulador********************/
lista *cria_lista(void)
{
    lista *l = malloc(sizeof(lista));
    l->cabeca = NULL;
    return l;
}
/* Cria a lista arco_lugar */
lista_arco_lugar *cria_lista_arco_lugar(void)
{
    lista_arco_lugar *l = malloc(sizeof(lista_arco_lugar));
    l->cabeca = NULL;
    return l; /* retorna seu endereço */
}
/* Cria a lista arco_transicao */
lista_arco_transicao *cria_lista_arco_transicao(void)
{
    lista_arco_transicao *l = malloc(sizeof(lista_arco_transicao));
    l->cabeca = NULL;
    return l;
}
/* Cria a lista transicao */
lista_transicao *cria_lista_transicao(void)
{
    lista_transicao *l = malloc(sizeof(lista_transicao));
    l->cabeca = NULL;
    return l;
}

/***************** Fim de criar lista  - Começo adicionar lista ************/
/*Adiciona um valor ao final da lista*/
/*O valor deve ser do mesmo tipo que o conteúdo do node*/
void adiciona_na_lista(lista *l, int valor)
{
    /*Cria um novo node com o valor a ser adicionado*/
    node *novo_node = malloc(sizeof(node));
    novo_node->conteudo = valor;
    novo_node->proximo = NULL;

    /*Verifica se a lista é vazia*/
    /*Se sim, adiciona o novo_node na cabeça*/
    if(l->cabeca == NULL)
    {
        novo_node->indice = 0;
        l->cabeca = novo_node;
    }
    else
    {
        /*Se não for vazia, procura o último node da lista*/
        node *no;
        for(no = l->cabeca; no->proximo != NULL; no = no->proximo);

        /*Nesse último node, adiciona o novo_node como proximo*/
        novo_node->indice = no->indice + 1;
        no->proximo = novo_node;
        /* Ao fim do código, limpa todas as alocações dinâmicas realizadas */
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
        valor->indice = 0;
        l->cabeca = novo_node;
    }
    else
    {
        /* Se não for vazia, procura o último node da lista*/
        node_transicao *no;
        for(no = l->cabeca; no->proximo != NULL; no = no->proximo);

        /* Nesse último node, adiciona o novo_node como proximo*/
        valor->indice = no->conteudo->indice + 1;
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

/***********************Fim de adiciona - Começo de limpa *********************************/

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
/* Libera a memória alocada para a lista e seus nós */
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

/**************************** Fim de libera - Começo de imprime *************************/
/* Imprime a lista, elemento por elemento */
void imprime_lista(lista *l, char entrada_lugar)
{
    node *no;
    int n=0;
    for(no = l->cabeca; no != NULL; no = no->proximo)
    {
        if(entrada_lugar == 'e')/* Caso seja referente a entrada */
        {
            printf(" %d ", no->conteudo);
            switch(n)
            {
                case 0:
                    printf(" Numero de lugares \n");
                    break;
                case 1:
                    printf(" Numero de transicaoes \n");
                    break;
                case 2:
                    printf(" Numero de lugares com tokens \n");
                    break;
                case 3:
                    printf(" Numero de arco transicões \n");
                    break;
                case 4:
                    printf(" Numero de arco lugares \n");
                    break;
            }
        }
        else
        {
            if(entrada_lugar == 'l')/* Caso seja referente a lugar */
                printf(" %dº lugar | %d Quantidade de tokens \n",n, no->conteudo);
        }
        n++;
    }
}

/* Imprime a lista, elemento por elemento */
void imprime_lista_arco_lugar(lista_arco_lugar *l)
{
    int n1=0,*ponteiro;
    ponteiro=l;
    printf(" endereço arco_lugar = %d \n", ponteiro);
    node_arco_lugar *no;
    arco_lugar *al;
    node *lugar;
    transicao *t;
    for(no = l->cabeca; no != NULL; no = no->proximo)
    {
        ponteiro = no;
        printf(" endereco cabeca lista a_lugar = %d \n",ponteiro);
        al = no->conteudo;
        t = al->destino;
        lugar = al->origem;
        printf(" Arcolugarnumero %d Qtokensnesselugar =  %d valortransi = %d  transimanda -> %d \n",n1, lugar->conteudo, t->coletor, t->emissor);
        n1++;
    }
}

/* Imprime a lista, elemento por elemento*/

void imprime_lista_arco_transicao(lista_arco_transicao *l)
{
    int n1=0;
    node_arco_transicao *no;
    arco_transicao *at;
    node *lugar;
    transicao *t;
    for(no = l->cabeca; no != NULL; no = no->proximo)
    {
        at = no->conteudo;
        t = at->origem;
        lugar = at->destino;
        printf(" Atransicaonumero = %d Qtokensnesselugar =  %d valortransi = %d  transimanda -> %d \n",n1, lugar->conteudo, t->coletor, t->emissor);
        n1++;
    }
}

/* Imprime a lista, elemento por elemento */
void imprime_lista_transicao(lista_transicao *l)
{
    node_transicao *no;
    transicao *t ;//= no->conteudo;
    int n=0;
    for(no = l->cabeca; no != NULL; no = no->proximo)
    {
        t = no->conteudo;
        printf(" Transicao = %d |  %d = Valor |  %d = Envia \n",n , t->coletor,t->emissor);
        n++;
    }
}
