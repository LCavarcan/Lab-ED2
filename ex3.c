#include <stdio.h>

#define Tamanho 2

struct aluno {
    char nome[20];
    int idade;
    int nota;
};

int main() {
    struct aluno a;

    FILE *f;

    printf("\nLendo dados\n\n");

    f = fopen("saida.txt", "w");

    for(int i=0;i<Tamanho;i++) {
        printf("\nEntre com o nome do aluno: ");
        scanf("%s", a.nome);

        printf("\nEntre com a idade: ");
        scanf("%d", &a.idade);

        printf("\nEntre com a nota: ");
        scanf("%d", &a.nota);

        printf("\n");

        fprintf(f, "%s", a.nome);
        fprintf(f, "%d", a.idade);
        fprintf(f, "%d", a.nota);
    }

    fclose(f);

    printf("\nDados do arquivo txt\n\n");

    f=fopen("saida.txt", "r");

    for(int i=0;i<Tamanho;i++) {
        fscanf(f, "%s", a.nome);
        printf("\nNome: %s", a.nome);

        fscanf(f, "%d", &a.idade);
        printf("\nIdade: %d", a.idade);

        fscanf(f, "%d", &a.nota);
        printf("\nNota: %d", a.nota);
    }

    fclose(f);

    return 0;
}