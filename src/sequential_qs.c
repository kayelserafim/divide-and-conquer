/*
 ============================================================================
 Name        : sequential_qs.c
 Author      : Kayel Serafim
 Version     :
 Copyright   :
 Description : The sequential version of QS to measure the total time
 ============================================================================
 */
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

// 0 para desabilitar os prints e 1 para habilitar
#define DEBUG 0
// trabalho final com o valores 10.000, 100.000, 1.000.000
#define ARRAY_SIZE 1000000

/*
 * Quick sort algorithm.
 */
int compare(const void *a, const void *b) {
	return (*(int*) a - *(int*) b);
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
	int vetor[ARRAY_SIZE];
	clock_t begin = clock();

	printf("Populando o vetor com inteiros no pior caso...");
	populate(ARRAY_SIZE, vetor);
	printf("\nVetor populado.");

#if DEBUG
	printf("\nVetor desordenado:");
	print(ARRAY_SIZE, vetor);
#endif

	printf("\nOrdenando vetores...");
	qsort(vetor, ARRAY_SIZE, sizeof(int), compare);
	printf("\nVetor ordenado.");

#if DEBUG
	printf("\nVetor ordenado:");
	print(ARRAY_SIZE, vetor);
#endif

	clock_t end = clock();
	printf("\nTempo: %f segundos\n", (double) (end - begin) / CLOCKS_PER_SEC);
	return 0;
}
