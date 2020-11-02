/*
 ============================================================================
 Name        : parallel_bs.c
 Author      : Kayel Serafim
 Version     :
 Copyright   :
 Description : Parallel version of BS following the division and conquest model
 ============================================================================
 */
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define DIVIDE_CONQUER_TAG 0

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
	printf("[");
	for (i = 0; i < n; i++)
		printf(" %d ", vector[i]);
	printf("]\n");
}

int main(int argc, char **argv) {
	int proc_n;
	int my_rank;
	int tam_vetor;
	// Tempo inicial e final
	double t0, t1;
	MPI_Status status;

	// função que inicializa o MPI
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

	int vector[ARRAY_SIZE];
	int *interleaving_vetor;
	t0 = MPI_Wtime();

	if (my_rank != 0) {
		// não sou a raiz, tenho pai
		MPI_Recv(&vector, ARRAY_SIZE, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		// descubro tamanho da mensagem recebida
		MPI_Get_count(&status, MPI_INT, &tam_vetor);
	} else {
		// defino tamanho inicial do vetor
		tam_vetor = ARRAY_SIZE;
		// sou a raiz e portanto gero o vetor - ordem reversa
		populate(tam_vetor, vector);
	}
	int delta = ARRAY_SIZE / ((proc_n + 1) / 2);
	int parent_leaf = (my_rank - 1) / 2;

#if DEBUG
	printf("Dividir ou conquistar? \n my_rank: %d, tam_vetor: %d e delta: %d \n", my_rank, tam_vetor, delta);
#endif
	// dividir ou conquistar?
	if (tam_vetor <= delta) {
		// conquisto
		printf("conquer at rank %d, parent_leaf %d \n", my_rank, parent_leaf);

		qsort(vector, tam_vetor, sizeof(int), compare);
		MPI_Send(vector, tam_vetor, MPI_INT, parent_leaf, status.MPI_TAG, MPI_COMM_WORLD);

#if DEBUG
		print(tam_vetor, vector);
#endif
	} else {
		// dividir
		if (tam_vetor % 2 != 0) {
			printf("vetor de tamanho ímpar [%d] sendo divido por 2 o que irá gerar inconsistência \n", tam_vetor);
			exit(1);
		}
		// quebrar em duas partes e manda para os filhos
		int size = tam_vetor / 2;
		int left_leaf = 2 * my_rank + 1;
		int right_leaf = 2 * my_rank + 2;

		printf("divide at rank %d -> left: %d and right:%d \n", my_rank, left_leaf, right_leaf);

		// mando metade inicial do vetor
		MPI_Send(&vector[0], size, MPI_INT, left_leaf, DIVIDE_CONQUER_TAG, MPI_COMM_WORLD);
		// mando metade final
		MPI_Send(&vector[size], size, MPI_INT, right_leaf, DIVIDE_CONQUER_TAG, MPI_COMM_WORLD);

		// receber dos filhos
		MPI_Recv(&vector[0], size, MPI_INT, left_leaf, DIVIDE_CONQUER_TAG, MPI_COMM_WORLD, &status);
		MPI_Recv(&vector[size], size, MPI_INT, right_leaf, DIVIDE_CONQUER_TAG, MPI_COMM_WORLD, &status);
	}

	// intercalo vetor inteiro
	interleaving_vetor = interleaving(vector, tam_vetor);

	// mando para o pai
	if (my_rank != 0) {
		// tenho pai, retorno vetor ordenado pra ele
		MPI_Send(interleaving_vetor, tam_vetor, MPI_INT, parent_leaf, DIVIDE_CONQUER_TAG, MPI_COMM_WORLD);
	} else {
		// sou o raiz, mostro vetor
		t1 = MPI_Wtime();
		printf("Time %f\n", t1 - t0);
#if DEBUG
		printf("sou a raiz, mostro vetor \n");
		print(tam_vetor, interleaving_vetor);
#endif
	}

	free(interleaving_vetor);

	MPI_Finalize();
	return 0;
}
