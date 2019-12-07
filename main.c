//                                    Aplicativo PERT SOLVER
/*
                                        Integrantes do grupo
                                          
                                          Nome/Matrícula:

                                1- Henrique Ribeiro da Silva 0011453
                                2- João Geraldo Borges Sales 0049541
                                3- Rafael Souza Bernardo 0041154
                                4- Samuel de Castro Ribeiro 0032743
                                5- Yuri Gandra Santos 00494666
*/

/*Para compilar o arquivo é necessário atribuir alguns parâmetros na seguinte ordem: ./pert-solver <arquivo>  d.dot futebol.r

                            Ambiente de desenvolvimento: Visual Studio / Linux 
                            Data: 07/12/2019
                            Objetivo: Construir um algorimo para calular caminho crítico de projetos, alem de gerar arquivos 

*/

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
        // abre o arquivo 
        FILE *arquivoEntrada = fopen(argv[1], "r");

        while (fgets(linha, 100, arquivoEntrada) != NULL) {

            /*ignora o comentário do arquivo*/
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


            // condição para pegar os  predecessores
            } else if (linha[0] == 'a') {


                token = strtok(linha, " ");

                token = strtok(NULL, " ");

                i = atoi(token);

                token = strtok(NULL, " ");

                j = atoi(token);

                R[i][j] = 1;

            //fim do arquivo
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




        float tempoFinal = gVetorAtividades[t - 1].inicio, tempoInicio;
        float *pTempoFinal;
        int numeroCritico = t - 1;
        int *numero = &numeroCritico;
        pTempoFinal = &tempoFinal;

        do {


            //Procurar os predecessores do critico atual
            for (i = 0; i < t; i++) {

                // Acha o caminho critico
                if (R[i][numeroCritico] == 1) {

                    if (gVetorAtividades[numeroCritico].inicio == gVetorAtividades[i].termino) {
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
                
                teste += gVetorAtividades[i].duracao;
            }
            

        }

        printf("Projeto: '%s'\n\n", argv[1]);
        printf("Total de Atividades: %d\n\n", t);
        printf("Makespan: %.2f\n\n", gVetorAtividades[t - 1].termino);
        printf("Caminho Critico: \n\n");

        for (i = 0; i < t; i++) {

            if (gVetorAtividades[i].critico == 1) {

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
        char *aux;

        fprintf(arquivoDot, "digraph Pert\n");
        fprintf(arquivoDot, "{\n");
        fprintf(arquivoDot, "\trankdir=TB\n");

        for (i = 0; i < t; i++) {

            if (gVetorAtividades[i].critico == 1) {

                aux = strtok(gVetorAtividades[i].nome, "\n");
                strcpy(gVetorAtividades[i].nome, aux);
                fprintf(arquivoDot, "\t%d [label=\"(%d) %s \", fontcolor=red, color=\"#ff0000\" ];\n", i, gVetorAtividades[i].id, gVetorAtividades[i].nome);

            } else {

                aux = strtok(gVetorAtividades[i].nome, "\n");
                strcpy(gVetorAtividades[i].nome, aux);
                fprintf(arquivoDot, "\t%d [label=\"(%d) %s\" ];\n", i, gVetorAtividades[i].id, gVetorAtividades[i].nome);

            }

        }

        fprintf(arquivoDot, "\n");

        for (i = 0; i < t; i++) {


            for (j = 0; j < t; j++) {


                /*if (R[i][j] == 1 ){
                
                    if (gVetorAtividades[j].critico == 1) {
                        fprintf(arquivoDot, "\t%d -> %d [color=\"#ff0000\"];\n",i,j);    
                    } else {
                        fprintf(arquivoDot, "\t%d -> %d;\n", i, j);    
                    }
                
                }*/


                if (R[i][j] == 1) {

                    if (gVetorAtividades[i].critico == 1 && gVetorAtividades[j].critico == 1) {

                        fprintf(arquivoDot, "\t%d -> %d [color=\"#ff0000\"];\n", i, j);

                    } else {

                        fprintf(arquivoDot, "\t%d -> %d;\n", i, j);

                    }



                }

            }

        }


        fprintf(arquivoDot, "}\n");

        char *auxiliar;
        int fflush(FILE * arquivoDot);

        fclose(arquivoDot);


        FILE *graficoGant = fopen(argv[3], "w");

        auxiliar = strtok(argv[3], ".");

        fprintf(graficoGant, "# Nome do arquivo de saida, informado pela aplicacao\n");
        fprintf(graficoGant, "ArqSaida <- \"%s.png\"\n", auxiliar);        
        fprintf(graficoGant, "#\n");
        fprintf(graficoGant, "# Parametros que devem ser informados pelo pert-solver\n");
        fprintf(graficoGant, "#\n\n");
        fprintf(graficoGant, "# Numero de atividades e makespan calculado\n\n");
        fprintf(graficoGant, "Makespan <- %.2f\n", gVetorAtividades[t - 1].termino);
        fprintf(graficoGant, "Atividades <- %d\n", t);
        fprintf(graficoGant, "# Vetor com as flags das atividades criticas\n\n");
        fprintf(graficoGant, "Cp <- c(");
        for (size_t i = 0; i < t; i++) {

            if (i < t - 1) {

                if (gVetorAtividades[i].critico == 1) {
                    fprintf(graficoGant, "T,");
                } else {
                    fprintf(graficoGant, "F,");
                }

            } else {

                if (gVetorAtividades[i].critico == 1) {
                    fprintf(graficoGant, "T)\n");
                } else {
                    fprintf(graficoGant, "F)\n");
                }
            }

            
        }
        
        fprintf(graficoGant,"#Vetor com os indices das atividades\n");
        fprintf(graficoGant, "K <- c(");
        for (i = 0; i < t; i++) {

            if (i < t - 1) {
                
                fprintf(graficoGant, "%d,", gVetorAtividades[i].id);
                
            } else {

                fprintf(graficoGant, "%d)\n", gVetorAtividades[i].id);
            }

            
        }
                                
        fprintf(graficoGant,"#Vetor com os tempos de inicio das atividades\n");
        fprintf(graficoGant, "Sk <- c(");
        for (size_t i = 0; i < t; i++) {

            if (i < t - 1) {

                    fprintf(graficoGant, "%.2f,", gVetorAtividades[i].inicio);

            } else {

               fprintf(graficoGant, "Makespan)\n");
               
            }

            
        }    


        fprintf(graficoGant,"#Vetor com os tempos de termino das atividades\n");
        fprintf(graficoGant, "Tk <- c(");
        for (size_t i = 0; i < t; i++) {

            if (i < t - 1) {

                fprintf(graficoGant, "%.2f,", gVetorAtividades[i].termino);

            } else {

               fprintf(graficoGant, "Makespan)\n");
               
            }

            
        }
        
        fprintf(graficoGant,"#\n");
        fprintf(graficoGant,"# Fim dos parametros. Daqui pra frente deve ser automatico.\n");
        fprintf(graficoGant,"#\n\n");
        fprintf(graficoGant,"# Calculos auxiliares\n");
        fprintf(graficoGant,"xMin <- 0\n");
        fprintf(graficoGant,"xMax <- Makespan + 1\n");
        fprintf(graficoGant,"yMin <- 0\n");
        fprintf(graficoGant,"yMax <- Atividades + 1\n");
        fprintf(graficoGant,"# Geracao de arquivo\n");
        fprintf(graficoGant,"png(ArqSaida, width=1200)\n");
        fprintf(graficoGant,"# Cria o plot\n");
        fprintf(graficoGant,"titulo <- \"Gráfico de Gantt\"\n");
        fprintf(graficoGant,"plot(c(xMin, xMax), c(yMin, yMax), type=\"n\",\n");
        fprintf(graficoGant,"\tmain=titulo, xlab=\"Tempo\", ylab=\"Atividades\");\n");
        fprintf(graficoGant,"# Desenha um retangulo para cada atividade\n");
        fprintf(graficoGant,"for(indice in 1:length(K))\n");
        fprintf(graficoGant,"{\n");
        fprintf(graficoGant,"\t# Busca as coordenadas dos pontos do retangulo\n");
        fprintf(graficoGant,"\tPx1 <- Sk[indice]\n");
        fprintf(graficoGant,"\tPy1 <- K[indice]\n");
        fprintf(graficoGant,"\tPx2 <- Tk[indice]\n");
        fprintf(graficoGant,"\tPy2 <- K[indice]\n");
        fprintf(graficoGant,"\tPx3 <- Tk[indice]\n");
        fprintf(graficoGant,"\tPy3 <- K[indice]+1\n");
        fprintf(graficoGant,"\tPx4 <- Sk[indice]\n");
        fprintf(graficoGant,"\tPy4 <- K[indice]+1\n\n");
        fprintf(graficoGant,"\t# Define a cor de acordo com a flag de criticidade\n");
        fprintf(graficoGant,"\tif(Cp[indice] == T)\n");
        fprintf(graficoGant,"\t{\n");
        fprintf(graficoGant,"\tcor <- \"red\"\n");
        fprintf(graficoGant,"\tcortexto <- \"white\"\n");
        fprintf(graficoGant,"\t}\n");
        fprintf(graficoGant,"\telse\n");
        fprintf(graficoGant,"\t{\n");
        fprintf(graficoGant,"\tcor <- \"yellow\"\n");
        fprintf(graficoGant,"\tcortexto <- \"black\"\n");
        fprintf(graficoGant,"\t}\n\n");
        fprintf(graficoGant,"\t# Desenha o polígono, fechando as coordenadas\n");
        fprintf(graficoGant,"\tpolygon(c(Px1, Px2, Px3, Px4, Px1),\n");
        fprintf(graficoGant,"\t\tc(Py1, Py2, Py3, Py4, Py1), col=cor)\n\n");
        fprintf(graficoGant,"\ttext((Px1 + Px2)/2, (Py1 + Py3)/2, K[indice], col=cortexto)\n");
        fprintf(graficoGant,"}\n");
        fprintf(graficoGant,"dev.off()\n");


        int fflush(FILE *graficoGant);
        fclose(graficoGant);
        

    }
    return (EXIT_SUCCESS);
}