#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 


//Função para descobrir qual predecessor acaba mais rapido
//void terminoDasAtividades(int *predecessor, int tamanho, int totalAtividades);


/*------------------------------------------------------------*/
/*                        STRUCT                              */

/*------------------------------------------------------------*/


typedef struct {
    int id;
    char nome[50];
    float duracao;
    float inicio;
    float termino;
    int critico;
    int flagMinimal; //Será 1 quando a atividade já tiver sido agendado

} atividade;

atividade *gVetorAtividades; // STRUCT DE ATIVIDADES 

/*matriz de relações */
int **R;
// matriz temporaria
int **T;

// Vetor de minimais
int *M;

// Vetor de predecessores
int *P;

int main(int argc, char** argv) {
    char linha[100]; // numero de caracteres  
    char *token; // variavel para receber os dados da linha 
    int t; // Total de atividades 
    int i = 0, j = 0; // contador da posição da matriz de relações 
    int c = 0; // contador da posição do vetor 
    int verificador = 0; // contador para verificar se é minimal
    int posicao_m = 0; // posição no vetor de minimais
    int verificaMatriz = 1; // verifica se a matriz esta vazia
    int qtdPredecessores = 0; //    contador das posiçoes do vetor de precedencias
    int posicao_P = 0; //posição no vetor de predecessores
    int p; // contador de coluna
    int contCritico = 0; //Contador para controlar o caminho critico
    float termino = 0; //Controla o termino para achar o caminho critico
    int controleCritico; //Guarda a posicao do caminho critico atual

    /*Bloqueando caso falte argumentos para rodar o programa*/
    if (argc < 4) {

        printf("Erro, falta de parâmetros!\n");


    } else {
        // abri o arquivo 
        FILE *arquivoEntrada = fopen(argv[1], "r");

        while (fgets(linha, 100, arquivoEntrada) != NULL) {

            /*igonara o comentário do do arquivo*/
            if (linha[0] == ' ' || linha[0] == '#') {


                continue;

                /*Condição para pegar quantidade de atividades*/
            } else if (linha[0] == 'T') {


                token = strtok(linha, " ");

                token = strtok(NULL, " ");

                t = atoi(token);

                gVetorAtividades = calloc(t, sizeof (atividade));

                R = (int**) calloc(t, sizeof (int *));
                for (i = 0; i < t; i++) {

                    R[i] = (int*) calloc(t, sizeof (int));

                }

                /*Condição para pegar dados de cada atividade, */
            } else if (linha[0] == 'n') {


                token = strtok(linha, " ");

                token = strtok(NULL, " ");

                gVetorAtividades[c].id = atoi(token);


                token = strtok(NULL, " ");

                gVetorAtividades[c].duracao = atof(token);


                token = strtok(NULL, " ");

                strcpy(gVetorAtividades[c].nome, token);

                gVetorAtividades[c].flagMinimal = 0;

                c++;



            } else if (linha[0] == 'a') {


                token = strtok(linha, " ");

                token = strtok(NULL, " ");

                i = atoi(token);

                token = strtok(NULL, " ");

                j = atoi(token);

                R[i][j] = 1;


            } else if (linha[0] == 'f') {


                break;

            }



        }

        fclose(arquivoEntrada);

        //alocando espaço para o vetor temporario
        T = (int**) calloc(t, sizeof (int *));
        for (i = 0; i < t; i++) {

            T[i] = (int*) calloc(t, sizeof (int));

        }
        /*Copiando a matriz R para T(temporaria)*/
        for (i = 0; i < t; i++) {

            for (j = 0; j < t; j++) {

                T[i][j] = R[i][j];

            }

        }



        /*Liberando todos os sucessores de INI*/
        for (i = 0; i < t; i++) {

            T[0][i] = 0;

        }

        for (size_t k = 0; k < t - 1; k++) {
            gVetorAtividades[k].critico = 0;
        }

        /*Colocando o tempo da atividade INI*/
        gVetorAtividades[0].flagMinimal = 1;
        gVetorAtividades[0].inicio = 0;
        gVetorAtividades[0].termino = 0;
        gVetorAtividades[0].critico = 1;
        /*Colocando o tempo da atividade FIM*/
        gVetorAtividades[t - 1].inicio = 0;
        gVetorAtividades[t - 1].termino = 0;
        gVetorAtividades[t - 1].critico = 1;




        // repete o loop até T ficar totalmente zerado
        while (verificaMatriz != 0) {

            // Alocando espaço de memoria para o vetor de minimais
            M = malloc(1 * sizeof (int));
            posicao_m = 0;
            //Flag para controlar se ainda tem atividades a ser agendado
            verificaMatriz = 0;


            /*Laço de repetição para pegar todos os minimais*/
            for (i = 0; i < t; i++) {

                verificador = 0;

                for (j = 0; j < t; j++) {

                    //Condição para percorre a matriz em colunas e ver que não é minimal, porque ainda tem predecessor
                    if (T[j][i] == 1) {

                        verificador++;

                    }



                }

                /*Se entrar na condição é porque a atividade é um minimal e ainda não foi agendado*/
                if (verificador == 0 && gVetorAtividades[i].flagMinimal != 1) {

                    //Atribuindo i ao vetor porque essa é a atividade que está sendo analisada no momento
                    M[posicao_m] = i;
                    gVetorAtividades[i].flagMinimal = 1;
                    posicao_m++;
                    M = realloc(M, (posicao_m + 1) * sizeof (int));


                }

            }

            contCritico = 0;
            /*Laço de repetição para realizar agendamento das atividades que são minimais no momento*/
            for (i = 0; i < posicao_m; i++) {




                // aloca espaço de memoria para o vetor de predecessores
                P = malloc(1 * sizeof (int));

                //Iniciando a quantidade de predecessores
                qtdPredecessores = 0;
                posicao_P = 0;
                /*Escolhendo uma atividade para realizar agendamento*/
                for (j = 0; j < t; j++) {


                    // Encontra os predecessores, fixando a coluna em um dos minimais do vetor M
                    if (R[j][(M[i])] == 1) {

                        //Passando j, que representa a linha, porque é o predecessor do minimal
                        P[qtdPredecessores] = j;
                        qtdPredecessores++;
                        P = realloc(P, qtdPredecessores + 1 * sizeof (int));

                        // Encontra o termino das atividades
                        if (qtdPredecessores == 1) {

                            gVetorAtividades[M[i]].inicio = gVetorAtividades[P[posicao_P]].termino;
                            gVetorAtividades[M[i]].termino = gVetorAtividades[M[i]].inicio + gVetorAtividades[M[i]].duracao;
                            posicao_P++;
                            //Caso o proximo predecessor tenha o tempo de termino maior que o já atribuido no minimal, será trocado
                        } else if (qtdPredecessores > 1) {
                            if (gVetorAtividades[M[i]].inicio < gVetorAtividades[P[posicao_P]].termino) {
                                gVetorAtividades[M[i]].inicio = gVetorAtividades[P[posicao_P]].termino;
                                //printf("Inicio: %d", gVetorAtividades[M[i]].inicio);                                
                                gVetorAtividades[M[i]].termino = gVetorAtividades[M[i]].inicio + gVetorAtividades[M[i]].duracao;
                                //printf("Término: %d", gVetorAtividades[M[i]].termino);
                                posicao_P++;

                            }

                        }


                    }


                }

                //Iremos zerar a linha do minimal para liberar os sucessores do minimal
                for (p = 0; p < t; p++) {

                    T[M[i]][p] = 0;

                }

            }




            /*PERCORRENDO A MATRIZ PARA SABER SE AINDA EXISTE ATIVIDADES A SER AGENDADO*/
            for (i = 0; i < t; i++) {

                for (j = 0; j < t; j++) {

                    if (T[i][j] == 1) {

                        verificaMatriz++;

                    }

                }

            }

        } //Fim do while


        /*
            for (i = 0; i < t; i++) {
                for (j = 0; j < t; j++) {

                    if (j == t - 1) {
                        printf("\t%d\n", T[i][j]);
                    } else {
                        printf("\t%d ", T[i][j]);
                    }

                }


            } 

         */

        // Verifica qual é o predecessor do FIM
        if (verificaMatriz == 0) {

            for (i = 0; i < t; i++) {

                if (gVetorAtividades[t - 1].inicio < gVetorAtividades[i].termino) {

                    gVetorAtividades[t - 1].inicio = gVetorAtividades[i].termino;

                }


            }


        }
        // o inicio do FIM é igual ao inicio dele mesmo
        gVetorAtividades[t - 1].termino = gVetorAtividades[t - 1].inicio;


        /*
        for(i = t -1; i > 0; i--) {

            for(j = 0; j < t; j++) {

                if(R[j][i] == 1) {

                    if(gVetorAtividades[j].inicio == gVetorAtividades[i].termino) {

                        gVetorAtividades[j].critico = 1;

                    }

                

            }


            }

        }
         

        for (i = 0; i < t; i++) {

            if (gVetorAtividades[i].critico == 1) {

                printf("%d\n", gVetorAtividades[i].id);

            }


        }
*/

        float tempoFinal = gVetorAtividades[t-1].inicio, tempoInicio;
        float *pTempoFinal;
        int numeroCritico = t - 1;
        int *numero = &numeroCritico;
        pTempoFinal = &tempoFinal;

    do {


        //Procurar os predecessores do critico atual
        for (i = 0; i< t; i++) {

            if (R[i][numeroCritico] == 1) {

                if (gVetorAtividades[numeroCritico].inicio == gVetorAtividades[i].termino) {
                    //printf("%d, ", gVetorAtividades[i].id);
                    *numero = i;
                    gVetorAtividades[i].critico = 1;
                    tempoInicio = gVetorAtividades[i].termino;
                }


            }

        }


    } while (tempoInicio != 0);

    float teste = 0;
    for (size_t i = 0; i < t; i++) {
        
        if (gVetorAtividades[i].critico == 1) {
            //printf("%d, ", gVetorAtividades[i].id);
            teste+= gVetorAtividades[i].duracao;
        }
           // printf("Termino %f\n", gVetorAtividades[i].termino);
        
    }
            //printf("Valor total %f\n", teste);
    

    }


    printf("Projeto: '%s'\n\n", argv[1]);
    printf("Total de Atividades: %d\n\n", t);
    printf("Makespan: %.2f\n\n", gVetorAtividades[t-1].termino);
    printf("Caminho Critico: \n\n");

    for(i = 0; i < t; i++) {

        if(gVetorAtividades[i].critico == 1) {

            printf("%s", gVetorAtividades[i].nome);
            printf("(%d)\t", gVetorAtividades[i].id);
            printf("Duração: %.2f\t", gVetorAtividades[i].duracao);
            printf("[%.2f....%.2f]\n\n", gVetorAtividades[i].inicio, gVetorAtividades[i].termino);

        }
        

    }

    printf("Fim do Processamento.\n\n");
    printf("Verificar os arquivos ‘%s’ e ‘%s’ para"
    " detalhes sobre a rede de atividades e o Gantt Chart correspondente.\n\n", argv[2], argv[3]);


    FILE *arquivoDot = fopen(argv[2], "w");

    fprintf(arquivoDot, "Digraph Pert\n");
    fprintf(arquivoDot, "{\n");
    fprintf(arquivoDot, "rankdir=TB\n");
    for(i = 0; i < t; i++) {



    }



    return (EXIT_SUCCESS);
}