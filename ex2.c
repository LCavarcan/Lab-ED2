#include <stdio.h>
#include <stdlib.h>

int main() {
    float vet[10], soma=0;

    printf("\nInforme 10 elementos em ponto flutuante:");

    for(int i=0;i<10;i++) {
        printf("\nElemento %d: ", i+1);
        scanf("%f", &vet[i]);
        soma += vet[i];
    }

    printf("\nOs elementos digitados na ordem inversa sao: ");

    for(int i=9;i>=0;i--) {
        printf("%.3f ", vet[i]);
    }

    return 0;
}