#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


/*
tempo: quantidade de tempo que o processo precisa utilizar para ser terminado
chegada: quando o processo irá chegar, interrompe o anterior pois o programa é PREEMPTIVO
tempoFinalizacao: quanto tempo o processo está utilizando desde ser puxado
*/
typedef struct{
    int tempo;
    int chegada;
    int tempoFinalizacao;
    int tempoEspera;
}tipoProcesso;





/*
Printa os processos de 'tipoProcesso processo[20]'.

'int maxProcessos' é a quantidade obtida após leitura na main();
'bool finalizado' é utilizado caso esteja na etapa final do programa.
*/
void printaProcessos(int maxProcessos, tipoProcesso processo[20], bool finalizado){
    if (finalizado == false){
        printf("\n\t\t\tChegada\t\tTempo\n");
        for(int i = 0; i < maxProcessos; i++){
            printf("Processo %i:\t\t%i\t\t%i\n", i, processo[i].chegada, processo[i].tempo);
        }
    }
    else{
        printf("\n\t\t\tChegada\t\tTempo\t\tT. de Finalizacao\n");
        for(int i = 0; i < maxProcessos; i++){
            printf("Processo %i:\t\t%i\t\t%i\t\t%i\n", i, processo[i].chegada, processo[i].tempo, processo[i].tempoFinalizacao);
        }
    }
}




/*
Organiza a tabela 'tipoProcesso processo[20]' em relação ao tempo de CHEGADA.
Tempo de chegada = processo[id].chegada

Olhar documentação para tipoProcesso.
*/
void simpleBubbleSort(tipoProcesso processo[20], int maxProcessos){
    tipoProcesso aux;
    int n_modificacoes = 0;

    for (int k = 1; k < maxProcessos; k++) {
        for (int j = 0; j < maxProcessos - 1; j++) {
            if (processo[j].chegada > processo[j + 1].chegada) {
                aux             = processo[j];
                processo[j]     = processo[j + 1];
                processo[j + 1] = aux;
                n_modificacoes++;
            }
        }
    }

    printf("\nProcessos organizados, modificacoes feitas: %i\n", n_modificacoes);
}




int main() {
    //cria link para o arquivo criando tipo FILE
    //
    const char *nomeArquivo = "teste.csv";
    FILE *arquivo = fopen(nomeArquivo, "r");
    //

    //erro caso tenha erro abrindo arquivo
    if (arquivo == NULL) {
        printf("Não foi possível abrir o arquivo %s\n", nomeArquivo);
        return 1;
    }
    
    //inicia variaveis
    //
    char linha[256]; 
    char *token;
    int maxProcessos = 0;
    //

    tipoProcesso processo[20];

    // Lê cada linha do arquivo CSV
    for (int i = 0; fgets(linha, sizeof(linha), arquivo) != NULL; i++) {
        // Divide a linha em campos usando a vírgula como delimitador
        token = strtok(linha, ",");
        processo[i].tempo = atoi(token);

        token = strtok(NULL, ",\n");
        processo[i].chegada = atoi(token);

        maxProcessos++;
    }


    //inicializa valores dentro do vetor de structs
    for (int i = 0; i < maxProcessos; i++){
        processo[i].tempoFinalizacao = 0;
        processo[i].tempoEspera = 0;
    }




    //printa os processos, tempos e chegadas
    printf("\nTabela nao organizada:");
    printaProcessos(maxProcessos, processo, false);

    //organiza tabela
    simpleBubbleSort(processo, maxProcessos);

    //printa novamente após organizar com a função acima
    printf("\nTabela organizada:");
    printaProcessos(maxProcessos, processo, false);






    //define quanto de tempo máximo para calcular os processos
    int tempoMax = 0;
    for (int i = 0; i < maxProcessos; i++){
        tempoMax += processo[i].tempo;
    }

    //mostra quanto tempo foi obtido acima
    printf("\nTempo total: %is\n\n", tempoMax);

    //linhadotempo
    int *linhaDoTempo = (int *) malloc(tempoMax * sizeof(int));


    // //inicializa os ponteiros de stack de processos ainda não terminados (PREEMPTIVO)
    // //
    // int stackProcesso[2000];
    // int stackPointer = 0;
    // stackProcesso[0] = -1;
    // //

    //inicializa os ponteiros de array de processos abaixo do tempomax (PREEMPTIVO)
    //
    int arrayProcesso[2000];
    int arrayPointer = 0;
    //

    //cria variaveis para usar no percorrimento
    int processoMenorTempo = -1;
    int quantProcessosSelecionados = -1;
    int quantProcessosSelecionados_anterior = -1;

    //percorre tabela começando do tempo zero até o tempo máximo (soma de todos os tempos)
    for(int tempoAtual = 0; tempoAtual < tempoMax; tempoAtual++){
        //'reseta' array
        arrayPointer = 0;


        //seleciona todos processos abaixo do tempoAtual
        quantProcessosSelecionados = 0;
        for(int i = 0; i < maxProcessos; i++){
            if (processo[i].chegada <= tempoAtual){
                arrayProcesso[arrayPointer] = i;
                arrayPointer++;
                quantProcessosSelecionados++;

                //caso processo abaixo do tempo atual ainda não terminou, acrescenta tempo de finalização
                if (processo[i].tempo > 0){
                    processo[i].tempoFinalizacao++;
                }
            }
        }


        //detecta caso tenha aumentado o número de processos abaixo do tempo atual
        if (quantProcessosSelecionados != quantProcessosSelecionados_anterior){
            //reseta processo selecionado
            processoMenorTempo = -1;
            int tempoMinimo = 99999;

            //descobre menor tempo dos processos selecionados
            for(int i = 0; i < arrayPointer; i++){ //detalhe: for() é skipado caso esteja vazio
                int aux = arrayProcesso[i];

                if (processo[aux].tempo < tempoMinimo && processo[aux].tempo > 0){
                    tempoMinimo = processo[aux].tempo;
                    processoMenorTempo = aux;
                }
            }
        }

        //tempo de espera: todo processo não nulo q não seja o processo de menor tempo aumenta tempo de espera
        for(int i = 0; i < maxProcessos; i++){
            if (processo[i].chegada <= tempoAtual){
                if (processo[i].tempo > 0 && i != processoMenorTempo){
                    processo[i].tempoEspera++;
                }
            }
        }

        //mostra processo em execução
        printf("[tempo:%i] ", tempoAtual);
        if (arrayPointer == 0){ //caso não haja processo executando
            printf("idle");
        }
        else{ //caso haja
            printf("%i", processoMenorTempo);
            processo[processoMenorTempo].tempo--;
        }
        printf("\n"); //identação
    }

    //mostra processos após calcular o tempo final
    printaProcessos(maxProcessos, processo, true);





    printf("\n\n||||||||||||||||||||||||||||||\n\nCalculando tempos medios...\n");

    //tempo medio de resposta
    float tempoResposta_total = 0;
    for (int i = 0; i < maxProcessos; i++){
        printf("\nTempo resposta de processo[%i]: %i", i, processo[i].tempoFinalizacao);
        tempoResposta_total += processo[i].tempoFinalizacao;
    }
    float tempoResposta_medio = tempoResposta_total / maxProcessos;
    printf("\nTempo medio de resposta dos processos: %.2f t.u.\n", tempoResposta_medio);


    //tempo medio de espera aqui
    float tempoEspera_total = 0;
    for (int i = 0; i < maxProcessos; i++){
        printf("\nTempo espera de processo[%i]: %i", i, processo[i].tempoEspera);
        tempoEspera_total += processo[i].tempoEspera;
    }
    float tempoEspera_medio = tempoEspera_total / maxProcessos;
    printf("\nTempo medio de espera dos processos: %.2f t.u.\n", tempoEspera_medio);


    fclose(arquivo);
    return 0;
}