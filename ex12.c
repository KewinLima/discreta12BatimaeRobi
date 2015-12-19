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
#define NOME_IMAGEM      "ex12.bmp"
#define NOME_ENTRADA     "entrada-petri-1.txt"
#define BLOCO_DE_ENTRADA 5
#define PORCENTAGEM      50
#define DEBUG            1
#undef DEBUG /* Caso queira um debug,por favor comente essa linha. */

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
void threads(lista *entradas, lista *lugar,  lista_arco_lugar *a_lugar,  lista_arco_transicao *a_transicao,  lista_transicao *transicoes);
void *transicao_pt(void *arg);

/*                    Simulador                        */
void simulador(lista *entradas,  lista *lugar, lista_arco_lugar *a_lugar, lista_arco_transicao *a_transicao, lista_transicao *transicoes, int tran_n);

/****************** Fim dos Protótipos ******************/
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

    //imprimie_lugar_allegro(entrada);
    threads(entrada ,lugares ,arcos_lugar ,arcos_transicao ,transicoes);

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
    return EXIT_SUCCESS;
}
/****************Inicio das funções - Inicio Simulador****************/
void threads(lista *entradas, lista *lugar,  lista_arco_lugar *a_lugar,  lista_arco_transicao *a_transicao,  lista_transicao *transicoes)
{
    node *no;
    no = entradas->cabeca;
    no = no->proximo;
    int Qtran= no->conteudo;
    pthread_t threads[Qtran];
    int i=0, arg[5+Qtran];

    arg[0] = entradas;
    printf(" endereco entrada     = %d \n", entradas);
    arg[1] = lugar;
    printf(" endereco lugar       = %d \n", lugar);
    arg[2] = a_lugar;
    printf(" endereco a_lugar     = %d \n", a_lugar->cabeca->conteudo);
    arg[3] = a_transicao;
    printf(" endereco a_transicao = %d \n", a_transicao);
    arg[4] = transicoes;
    printf(" endereco transicoes  = %d \n", transicoes);
    for(i=0; i<5 ;i++)
    {
        printf(" arg[%d] = %d \n Endereco arg[%d] = %d \n",i,arg[i],i,&arg[i]);
    }
    /*  for(i=0; i < Qtran; i++)
     *      {
     *              arg[i+5] = i;*/
    arg[5]=0;
    printf("arg[5] == %d\n arg[6] == %d\n", arg[5],arg[6]);
    /*  pthread_create(&threads[i], NULL, transicao_pt, (void*) &arg[0]);*/
    transicao_pt(&arg[5]);
    /*  }*/
}

void *transicao_pt(void *arg)
{
    int *pvalor,n=0,tran_n,temporario;//deve apagar temporario!

    lista *entradas;
    lista *lugar;
    lista_arco_lugar *a_lugar;
    lista_arco_transicao *a_transicao;
    lista_transicao *transicoes;
    pvalor = arg;
    tran_n = *pvalor;

    printf(" arg[0] == *pvalor == %d\n Endereco pvalor == %d\n",*pvalor);
    printf(" Thread da transicao %d executando \n", tran_n);
    for(n=0; n<5; n++)
    {
        pvalor = pvalor - 1;
        switch(n)
        {
            case 0:
                transicoes = *pvalor;
                printf(" transicoes  pvalor[%d] = [%d] \n",n,*pvalor);
                break;
            case 1:
                a_transicao = *pvalor;
                printf(" arco_trans pvalor[%d] = [%d] \n",n,*pvalor);
                break;
            case 2:
                a_lugar = *pvalor;
                printf(" arco_lugar pvalor[%d] = [%d] \n",n,*pvalor);
                break;
            case 3:
                lugar = *pvalor;
                printf(" lugar pvalor[%d] = [%d] \n",n,*pvalor);
                break;
            case 4:
                entradas = *pvalor;
                printf(" entradas pvalor[%d] = [%d] \n",n,*pvalor);
                break;
        }
    }
    printf(" Cheguei aqui linha 338\n");
    simulador(entradas, lugar, a_lugar, a_transicao,transicoes,tran_n);
}

void simulador(lista *entradas,  lista *lugar, lista_arco_lugar *a_lugar, lista_arco_transicao *a_transicao, lista_transicao *transicoes, int tran_n)
{
    int n,n1,n2,n3,sorteio;
    printf(" ####### DEGUB Linha 346 ######\n");
    /* Referente a lista */
    
    node *no;
    no = lugar->cabeca;
    no= no->proximo->conteudo;
    
    node *no_e;
    no_e = entradas->cabeca;
    
    int Qlugar,Qarco_t,Qarco_l;
    Qlugar = no_e->conteudo;
    Qarco_t = no_e->proximo->proximo->proximo->conteudo;
    Qarco_l = no_e->proximo->proximo->proximo->proximo->conteudo;
    
    /* Referente a lista_arco_lugar */

    node_arco_lugar *no_al;
    arco_lugar *al_al;
    node *lugar_al;
    transicao *t_al;
    no_al = a_lugar->cabeca;

    //printf("#   SUPREMO: endereceo cabeca lista alugar %d \n",ponteiro);
    al_al = no_al->conteudo; 
    t_al  =  al_al->destino;
    lugar_al = al_al->origem;
    /* Referente a lista_arco_transicao */

    node_arco_transicao *no_at;
    arco_transicao *at_at;
    node *lugar_at;
    transicao *t_at;
/*
    no_at = a_transicao->cabeca;
    at_at = no_at->conteudo;
    t_at = at_at->origem;
    lugar_at= at_at->destino;
*/
    /*Referente as transições */

    node_transicao *no_t;
    transicao *t;
    
    printf(" transicao numero %d \n", tran_n);
    printf(" arcolugar parte de %d \n", al_al->origem->conteudo);
    printf(" Quantidade de tokens no lugar = %d \n", lugar_al->conteudo);
    printf(" Arcotransicao chega em %d \n", at_at->destino);
    printf(" Valor da transicao %d \n", t_al->coletor);
    printf(" Valor da doacao %d \n",t_al->emissor);

    printf("#Ok, nesse momento estamos trabalhando com os arcolugares da transicao %d \n", tran_n);

    if(tran_n == 0)/* Caso seja a transicao 0, recebe a cabeca da lista */
    {
        no = transicoes->cabeca;
    }
    if(tran_n != 0)/* Caso seja aponta para os proxmo */
    {
        no = transicoes->cabeca;
        for(n1=0;n1<tran_n;n1++)
        {
            no = no->proximo;   
        }
    }
    t = no->conteudo;
    printf(" Tranicao == %d \n", *t);
    printf(" Tranicao envia %d\n", t->emissor);
    printf(" Tranicao custa %d\n", t->coletor);

    for(n1=0;n1< Qarco_l;n1++)
    {
        no_al = a_lugar->cabeca;
        if(n1 != 0)
        {
            for(n=0; n<n1 ;n++)/* Encontra o elemento da lista numero n1*/
            {
                no_al = no_al->proximo;
                al_al = no_al->conteudo;
            }
        }
        printf("# Certo, vez do arcolugar numero: %d da transicao %d\n",n1, tran_n);
        if(al_al->destino == t)
        {
            printf(" Endereco de t =%d \n Endereco de al_al->destino == %d \n",t,al_al->destino);
            printf("# Sim, o arcolugar %d se refere a transicao %d\n",n1, tran_n);
            if( (lugar_al->conteudo) - (t->coletor) >=0 )/* Condicao para acionar a transicao */
            {
                /*
                n2=0;
                for(no = entradas->cabeca; no!=NULL ; no->proximo)
                {
                    printf(" oi \n");
                    if( no == lugar_al)
                        break;
                    n2++;
                }
                */
                printf("# Legal, temos tokens suficientes no lugar %d, onde parte o arco lugar %d \n", n2,n1);
                lugar_al->conteudo = lugar_al->conteudo - t->coletor;
                printf("# Tirei %d do lugar %d que agora tem %d\n",t->coletor, n2,lugar_al->conteudo);
                printf("# Ok, vamo agora trabalhar com os arcos transicoes da transicao %d\n",tran_n); 
                for(n3=0;n3<Qarco_t;n3++)
                {
                    printf("# Certo, vez do arcotransicao numero: %d da transicao %d\n",n3,tran_n);
                    printf("# Ok, vejamos se o arcotransicao %d e' referente a transicao %d\n",n3, tran_n);
                    no_at = transicoes->cabeca;
                    if(n3 != 0)
                    {
                        for(n=1; n<n3 ;n++)/* Encontra o elemento da lista numero n2*/
                        {
                            if(no_at->proximo != NULL)
                            no_at = no_at->proximo;
                            at_at = no_at->conteudo;
                        }
                        lugar_at = at_at->destino;
                    }
                    if(at_at->origem == t)
                    {
                        printf("# Sim, esse arco transicao e' referente a transicao %d\n",tran_n);
                        printf("# Vamos ao sorteio: ");
                        sorteio = rand()%( 100/PORCENTAGEM );
                        if(sorteio != 0)
                        {
                            printf(" Perdeu no sorteio \n");
                            continue;
                        }
                        else
                        {
                            printf(" Ganhou o sorteio trans %d ativada\n", tran_n);
                            lugar_at->conteudo = lugar_at->conteudo + t->emissor;
                            printf("# Adicionei %d ao lugar %d \n",t->emissor, n3);// <- errado);
                        }
                    }
                    else
                    {
                        printf("# Esse arcotransicao nao tem relacao com a transicao %d\n", tran_n);
                        continue;
                    }
                }
            }
            else
            {   
                printf("# Pessima noticia, Voce nao tem tokens suficientes no lugar %d \n",n1);// <-
                printf("# Precisaria de %d tokens mas so' tem %d \n",t->coletor,lugar_al->conteudo);
                continue;
            }

        }
        else
        {
            printf("# O arcolugar %d nao tem relacao com a transicao %d\n",n1, tran_n);
            continue;
        }
    }
}
void imprimie_lugar_allegro(lista *l)
{
    node *no;
    no = l->cabeca;
    int n;

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

    for(n=0; n< (no->conteudo) ; n++)
    {
        circle(buff, 50+(n*100), 100, 50, CORAMARELO);/* desenha um circulo */
    }
    /*textprintf_ex(buff, font, 50, 50, CORVERDE, CORPRETO, "Teste do circulo!");*/

    save_bitmap(NOME_IMAGEM, buff, pal);/* Salva a imagem no diretorio */
    destroy_bitmap(buff);               /* Destroi a imagem do buffer  */
    allegro_exit();                     /* Termina o allegro           */

#ifdef DEBUG
    printf("Imagem %s salva com sucesso!  \n", NOME_IMAGEM);
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
    else {
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
    printf("\n\n SUPREMO endereço arco_lugar = %d \n\n", ponteiro);
    node_arco_lugar *no;
    arco_lugar *al;
    node *lugar;
    transicao *t;
    for(no = l->cabeca; no != NULL; no = no->proximo)
    {
        ponteiro = no;
        printf("\n SUPREMO: endereco cabeca lista a_lugar = %d \n",ponteiro);
        al = no->conteudo;
        t = al->destino;
        lugar = al->origem;
        printf(" Arcolugarnumero %d Qtokensnesselugar =  %d valortransi = %d  transimanda -> %d \n",n1, lugar->conteudo, t->coletor, t->emissor);
        n1++;
    }
    printf(" XAU!!! \n");
}

/* Imprime a lista, elemento por elemento*/

void imprime_lista_arco_transicao(lista_arco_transicao *l)
{
    int n1=0;
    node_arco_transicao *no;
    arco_transicao *al;
    node *lugar;
    transicao *t;
    for(no = l->cabeca; no != NULL; no = no->proximo)
    {
        al = no->conteudo;
        t = al->origem;
        lugar = al->destino;
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
