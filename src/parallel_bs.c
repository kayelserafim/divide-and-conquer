/*
 ============================================================================
 Name        : parallel_bs.c
 Author      : Kayel Serafim
 Version     :
 Copyright   :
 Description : Parallel version of BS following the division and conquest model
 ============================================================================
 */
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define DEBUG 1            // comentar esta linha quando for medir tempo
#define ARRAY_SIZE 40      // trabalho final com o valores 10.000, 100.000, 1.000.000

/*
 * Buble sort algorithm.
 */
void bs(int n, int *vetor) {
	int c = 0, d, troca, trocou = 1;

	while ((c < (n - 1)) & trocou) {
		trocou = 0;
		for (d = 0; d < n - c - 1; d++)
			if (vetor[d] > vetor[d + 1]) {
				troca = vetor[d];
				vetor[d] = vetor[d + 1];
				vetor[d + 1] = troca;
				trocou = 1;
			}
		c++;
	}
}

/* Recebe um ponteiro para um vetor que contem as mensagens recebidas dos filhos e
 * intercala estes valores em um terceiro vetor auxiliar. Devolve um ponteiro para este vetor
 */
int* interleaving(int vetor[], int tam) {
	int *vetor_auxiliar;
	int i1, i2, i_aux;

	vetor_auxiliar = (int*) malloc(sizeof(int) * tam);

	i1 = 0;
	i2 = tam / 2;

	for (i_aux = 0; i_aux < tam; i_aux++) {
		if (((vetor[i1] <= vetor[i2]) && (i1 < (tam / 2))) || (i2 == tam))
			vetor_auxiliar[i_aux] = vetor[i1++];
		else
			vetor_auxiliar[i_aux] = vetor[i2++];
	}

	return vetor_auxiliar;
}

/*
 * Fills an array in the worst case
 */
void populate(int n, int *vector) {
	int i;
	for (i = 0; i < n; i++) {
		vector[i] = n - i;
	}
}

/*
 * Print an array.
 */
void print(int n, int *vector) {
	int i;
	for (i = 0; i < n; i++) {
		printf("%d ", vector[i]);
	}
}

int main() {


}
