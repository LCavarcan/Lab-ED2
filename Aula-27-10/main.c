#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char tarefa[60];
    int dia, mes, ano, chave;
}ToDo;

int cadastrar_todo(ToDo **t, int quant, int tam) {
    int ok;
    
    if(quant<tam) {
        ToDo *novo = malloc(sizeof(ToDo));

        printf("\nDigite a Tarefa: ");
        scanf("%50[^\n]", novo->tarefa);

        printf("\nDigite a data prevista dd mm aaaa: ");
        scanf("%d%d%d", &novo->dia, &novo->mes, &novo->ano);
        getchar();

        do {
            printf("\nDigite a chave da tarefa: ");
            scanf("%d", &novo->chave);

            ok=1;

            for(int i=0;i<quant;i++) {
                if(novo->chave == t[quant]->chave) {
                    ok=0;
                    break;
                }
            }
        }while(ok==0);

        t[quant] = novo;
        return 1;
    }
    else {
        printf("\n\tERRO! Vetor cheio!");
        return 0;
    }
}

void imprimir(ToDo **t, int quant){
    int i;

    printf("\n\t------ Lista de Tarefas ------\n");

    for(i = 0; i < quant; i++){
        printf("\t%d = %d = %2d/%2d/%4d\t%s\n", t[i]->chave, i+1,t[i]->dia, t[i]->mes, t[i]->ano, t[i]->tarefa);
    }

    printf("\t--------------------------------\n");
}

void alterar_todo(ToDo **t, int quant){
    int id;

    imprimir(t, quant);

    printf("\n\tDigite a tarefa a ser alterada: \n");
    scanf("%d", &id);

    getchar();

    id--;

    if(id >= 0 && id < quant){
        ToDo *novo = malloc(sizeof(ToDo));

        printf("\nDigite a nova tarefa: ");
        scanf("%50[^\n]", novo->tarefa);

        printf("\nDigite a data da tarefa dd mm aaaa: ");
        scanf("%d%d%d", &novo->dia, &novo->mes, &novo->ano);

        t[id] = novo;
    }
    else
        printf("\n\tCodigo invalido!\n");
}

void salvar(ToDo **t, int quant, char arq[]){
    FILE *file = fopen(arq, "w");
    FILE *index = fopen("index.txt", "w");

    int i;

    if(file) {
        fprintf(file, "%d\n", quant);

        for(i = 0; i < quant; i++){
            fputs(t[i]->tarefa, file);
            fputc('\n', file);
            fprintf(file, "%d %d %d\n", t[i]->dia, t[i]->mes, t[i]->ano);
        }

        fclose(file);
    }
    else
        printf("\n\tERRO AO ABRIR/CRIAR O ARQUIVO!\n");
}

int ler_arquivo(ToDo **c, char arq[]){
    FILE *file = fopen(arq, "r");

    int quant = 0, i;
    ToDo *novo;

    if(file) {
        fscanf(file, "%d\n", &quant);

        for(i = 0; i < quant; i++){
            novo = malloc(sizeof(ToDo));

            fscanf(file, "%50[^\n]", novo->tarefa);
            fscanf(file, "%d %d %d\n", &novo->dia,  &novo->mes, &novo->ano);

            c[i] = novo;

        }
    }
    else
        printf("\n\tERROR AO ABRIR/CRIAR O ARQUIVO!\n");

    return quant;
}

int main(){
    ToDo *tarefa[100];
    char arquivo[] = {"ToDo.txt"};
    int opcao, tam = 100, quant = 0;

    do {
        printf("\n\t0 - Sair\n\t1 - Cadastrar\n\t2 -Alterar\n\t3 - Imprimir\n\t4 -Salvar\n\t5 - Ler arquivo\n");
        scanf("%d", &opcao);

        getchar();

        switch(opcao){
            case 1:
                quant += cadastrar_todo(tarefa, quant, tam);
                break;

            case 2:
                alterar_todo(tarefa, quant);
                break;

            case 3:
                imprimir(tarefa, quant);
                break;
            
            case 4:
                salvar(tarefa, quant, arquivo);
                break;

            case 5:
                quant = ler_arquivo(tarefa, arquivo);
                break;

            default:
                if(opcao != 0)
                    printf("\n\tOpcao invalida!!!\n");
        }
            
    }while(opcao != 0);

    return 0;
}