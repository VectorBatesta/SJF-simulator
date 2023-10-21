#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


void printaProcessos(int maxProcessos, int tabela[20][3], bool finalizado){
    if (finalizado == false){
        printf("\n\t\t\tChegada\t\tTempo\n");
        for(int i = 0; i < maxProcessos; i++){
            printf("Processo %i:\t\t%i\t\t%i\n", i, tabela[i][0], tabela[i][1]);
        }
    }
    else{
        printf("\n\t\t\tChegada\t\tTempo\t\tT. de Finalizacao\n");
        for(int i = 0; i < maxProcessos; i++){
            printf("Processo %i:\t\t%i\t\t%i\t\t%i\n", i, tabela[i][0], tabela[i][1], tabela[i][2]);
        }
    }
}


void simpleMergeSort(int tabela[20][3]){
    int aux[20][3];
    memcpy(aux, tabela, 20*3*sizeof(int));
    
    //todo
    perror("\nsimplemergesort nao criado ainda");
    exit(EXIT_FAILURE);

    memcpy(tabela, aux, 20*3*sizeof(int));
}






int main() {
    const char *nomeArquivo = "teste.csv";
    FILE *arquivo = fopen(nomeArquivo, "r");
    
    if (arquivo == NULL) {
        printf("Não foi possível abrir o arquivo %s\n", nomeArquivo);
        return 1;
    }
    
    //inicia variaveis
    char linha[256]; 
    char *token;
    int maxProcessos = 0;
    int tabela[20][3];



    // Lê cada linha do arquivo CSV
    for (int i = 0; fgets(linha, sizeof(linha), arquivo) != NULL; i++) {
        // Divide a linha em campos usando a vírgula como delimitador
        token = strtok(linha, ",");
        tabela[i][1] = atoi(token);

        token = strtok(NULL, ",\n");
        tabela[i][0] = atoi(token);

        maxProcessos++;
    }

    //[i][] : processo
    //[] [1]: tempo
    //[] [0]: chegada
    //[] [2]: tempo de finalização


    //inicializa tempo de finalização em zero
    for (int i = 0; i < maxProcessos; i++){
        tabela[i][2] = 0;
    }




    //printa os processos, tempos e chegadas
    printf("\nTabela nao organizada:");
    printaProcessos(maxProcessos, tabela, false);

    //organiza tabela
    simpleMergeSort(tabela);

    printf("\nTabela organizada:");
    printaProcessos(maxProcessos, tabela, false);

    //temp
    return(0);





    //define quanto de tempo máximo para calcular os processos
    int tempoMax = 0;
    for (int i = 0; i < maxProcessos; i++){
        tempoMax += tabela[i][1];
    }

    printf("\nTempo total: %is\n\n", tempoMax);







    int stackProcesso[2000];
    int stackPointer = 0;
    stackProcesso[0] = -1;

    printf("stack proceso: %i\ntabela[0][0]: %i, tabela[0][1]: %i\n\n", stackProcesso[0], tabela[0][0], tabela[0][1]);
    //seleciona primeiro processo
    int processoSelecionado = 0;
    //percorre tabela começando do tempo zero até o tempo máximo (soma de todos os tempos)
    for(int tempoAtual = 0; tempoAtual < tempoMax; tempoAtual++){
        //tempo de finalizaçao++ para cada processo iniciado e com tempo maior q zero
        for (int j = 1; j <= stackPointer; j++){
            tabela[stackProcesso[j]][2]++;
        }

        //imprime stack
        printf("stack: ");
        for(int i = 0; i <= stackPointer; i++){
            printf("%i, ", stackProcesso[i]);
        }

        //printa tempo atual
        printf("\t\t[tempo:%i]", tempoAtual);

        //faz modificações no processo selecionado (tempo-- e tempo de finalizaçao++)
        if (tabela[processoSelecionado][1] > 0){
            printf("%i ", processoSelecionado);
            tabela[processoSelecionado][1]--;
            tabela[processoSelecionado][2]++;
        }
        
        //pega processo do stack caso acabe o tempo do selecionado E tenha processo no stack
        if (stackPointer > 0 && tabela[processoSelecionado][1] == 0){
            processoSelecionado = stackProcesso[stackPointer];
            stackPointer--;
        }
        

        //detecta se prox processo irá interromper o anterior
        if (tempoAtual + 1 == tabela[processoSelecionado + 1][0] && tabela[processoSelecionado + 1][1] > 0){
            //detecta se o processo atual ainda tem tempo para ser processado e ser colocado na stack
            if (tabela[processoSelecionado][1] > 0){
                stackPointer++;
                stackProcesso[stackPointer] = processoSelecionado;
            }

            //seleciona o prox processo já que ele interrompe o anterior
            processoSelecionado++;
        }


        printf("\n");
    }

    
    printaProcessos(maxProcessos, tabela, true);

    //tempo medio de espera aqui
    
    //tempo medio de resposta aqui



    fclose(arquivo);
    return 0;
}