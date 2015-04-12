#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*ukuran matrix N x N*/
#define  N 4
#define MASTER 0               /* taskid of first task */
#define FROM_MASTER 1          /* setting a message type */
#define FROM_WORKER 2          /* setting a message type */

void printFinalResult();
void writeMatrixOnFile(int matrix[N][N], char *file) ;

/*Variabel yang akan digunakan untuk menyimpan nilai matrik soal dan jawaban */
int A[N][N], B[N][N], C[N][N];

/*Untuk membuka file ketika majikan memulai tasknya*/
void readFile() {
	int i, j;
	FILE *f1, *f2;

	f1 = fopen("Matrix_4A.txt", "r");
	f2 = fopen("Matrix_4B.txt", "r");

	if (f1 == NULL) {
		printf("File 1 tidak bisa dibuka!\n");
	}
	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			fscanf(f1, "%d", &A[i][j]);

	if (f2 == NULL) {
		printf("File 2 tidak bisa dibuka!\n");
	}
	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			fscanf(f2, "%d", &B[i][j]);

	fclose(f1);
	fclose(f2);
}

int main(int argc, char *argv[]) {
	int name_len;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int numtasks,              /* number of tasks in partition */
	    taskid,                /* a task identifier */
	    numworkers,            /* number of worker tasks */
	    source,                /* task id of message source */
	    dest,                  /* task id of message destination */
	    mtype,                 /* message type */
	    rows,                  /* rows of matrix A sent to each worker */
	    averow, extra, offset, /* used to determine rows sent to each worker */
	    from, to,
	    i, j, k, rc;           /* misc */

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

	struct timeval stopw, startw;

	if (numtasks < 1 ) {
		printf("Need at least one MPI tasks. Quitting...\n");
		MPI_Abort(MPI_COMM_WORLD, rc);
		exit(1);
	}

	from = taskid * N / numtasks;
	to = (taskid + 1) * N / numtasks;

	if (taskid == 0) {
		readFile();

		gettimeofday(&startw, NULL);
	}

	MPI_Barrier(MPI_COMM_WORLD);

	MPI_Bcast(B, N * N, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Scatter (A, N * N / numtasks, MPI_INT, A[from], N * N / numtasks, MPI_INT, 0, MPI_COMM_WORLD);

	for (i = from; i < to; i++)
		for (j = 0; j < N; j++) {
			C[i][j] = 0;
			for (k = 0; k < N; k++)
				C[i][j] += A[i][k] * B[k][j];
		}

	MPI_Gather(C[from], N * N / numtasks, MPI_INT, C, N * N / numtasks, MPI_INT, 0, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);

	if (taskid == MASTER) {
		gettimeofday(&stopw, NULL);
		/* Print results */
		//printFinalResult();
		writeMatrixOnFile(C, "MatrixHasil.txt");
		printf("\nExecution time : %lu\n", stopw.tv_usec - startw.tv_usec);
	}

	MPI_Finalize();
	return 0;
}


/*Mencetak matrix yang telah disimpan divariabel ke layar*/
void printMatrix(int matrix[N][N]) {
	int i, j = 0;

	/*Membaca tiap nilai array*/
	for (i = 0; i < N; i++) {
		printf("\n\n");
		for (j = 0; j < N; j++)
			printf("%2d\t", matrix[i][j]);
	}
}

/*Digunakan untuk menulis matrik hasil perkalian ke file*/
void writeMatrixOnFile(int matrix[N][N], char *file) {
	int i, j;

	/*Membuka file dengan pointer to file*/
	FILE *f = fopen(file, "w");
	if (f == NULL) {
		printf("File tidak bisa dibuka!\n");
		exit(1);
	}

	/*Melakukan penulisan pada file perbaris */
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			fprintf(f, " %2d \t", matrix[i][j]);
		}
		fprintf(f, "\n");
	}

	/*Menutup file*/
	fclose(f);
}


/*Mencetak tampilan akhir dari perkalian dua buah matrix*/
void printFinalResult() {
	printf("\n");
	printMatrix(A);
	printf("\n\n\t      * \n");
	printMatrix(B);
	printf("\n\n\t      = \n");
	printMatrix(C);
	printf("\n");
}