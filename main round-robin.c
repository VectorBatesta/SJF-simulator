#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>






/////////////////////////////////////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////
//
//
// DETALHE: q = 1
//
//
/////////////////////////////////////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////
/////////////////////////////////////////////////








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





    //inicializa os ponteiros de stack de processos ainda não terminados (PREEMPTIVO)
    //
    int stackProcesso[2000];
    int stackPointer = 0;
    stackProcesso[0] = -1;
    //

    printf("stack proceso: %i\nprocesso[0].chegada: %i, processo[0].tempo: %i\n\n",
            stackProcesso[0], processo[0].chegada, processo[0].tempo);
    //seleciona primeiro processo
    int processoSelecionado = 0;


    //percorre tabela começando do tempo zero até o tempo máximo (soma de todos os tempos)
    for(int tempoAtual = 0; tempoAtual < tempoMax; tempoAtual++){
        //tempo de finalizaçao++ para cada processo iniciado e com tempo maior q zero
        for (int j = 1; j <= stackPointer; j++){
            //processo já iniciado, portanto ele ainda não terminou
            processo[stackProcesso[j]].tempoFinalizacao++;
            //processo esperando ser computado, portanto tempo de espera sobe
            processo[stackProcesso[j]].tempoEspera++;
        }

        //imprime stack
        printf("stack: ");
        for(int i = 0; i <= stackPointer; i++){
            printf("%i, ", stackProcesso[i]);
        }

        //printa tempo atual
        printf("\t\t[tempo:%i]", tempoAtual);

        //faz modificações no processo selecionado (tempo-- e tempo de finalizaçao++)
        if (processo[processoSelecionado].tempo > 0){
            printf(" %i", processoSelecionado);
            processo[processoSelecionado].tempo--;
            processo[processoSelecionado].tempoFinalizacao++;
        }
        else if (stackPointer == 0 && processo[processoSelecionado].tempo == 0){
            //caso nao tenha processo sendo rodado
            printf(" idle");
        }
        
        //pega processo do stack caso acabe o tempo do selecionado E tenha processo no stack
        if (stackPointer > 0 && processo[processoSelecionado].tempo == 0){
            processoSelecionado = stackProcesso[stackPointer];
            stackPointer--;
        }

        //detecta se prox processo irá interromper o anterior
        if (tempoAtual + 1 == processo[processoSelecionado + 1].chegada && processo[processoSelecionado + 1].tempo > 0){
            //detecta se o processo atual ainda tem tempo para ser processado e ser colocado na stack
            if (processo[processoSelecionado].tempo > 0){
                stackPointer++;
                stackProcesso[stackPointer] = processoSelecionado;
            }

            //seleciona o prox processo já que ele interrompe o anterior
            processoSelecionado++;
        }

        //tabulação para completar a formatação de print
        printf("\n");
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