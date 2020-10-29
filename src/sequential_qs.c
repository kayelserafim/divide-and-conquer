/*
 ============================================================================
 Name        : sequential_qs.c
 Author      : Kayel Serafim
 Version     :
 Copyright   :
 Description : The sequential version to measure the total time
 ============================================================================
 */
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define DEBUG 1            // comentar esta linha quando for medir tempo
#define ARRAY_SIZE 40      // trabalho final com o valores 10.000, 100.000, 1.000.000

/*
 * Comparison function.
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

#ifdef DEBUG
	printf("\nVetor desordenado:");
	print(ARRAY_SIZE, vetor);
#endif

	printf("\nOrdenando vetores...");
	qsort(vetor, ARRAY_SIZE, sizeof(int), compare);
	printf("\nVetor ordenado.");

#ifdef DEBUG
	printf("\nVetor ordenado:");
	print(ARRAY_SIZE, vetor);
#endif

	clock_t end = clock();
	printf("\nTempo: %f segundos\n", (double) (end - begin) / CLOCKS_PER_SEC);
	return 0;
}
