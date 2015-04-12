#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main() {
	int size ;
	char Name1[30], Name2[30];
	printf("\nInput :\n");
	printf("Masukkan ukuran matrix : "); scanf("%d", &size);
	printf("Masukkan nama file : "); scanf("%s", &Name1);
	strcpy (Name2, Name1);
	FILE *f;
	int i, j, randNum;
	srand(time(NULL));
	strcat (Name1, "A.txt");

	f = fopen(Name1, "w");

	printf("\nResult :\n");
	if (f == NULL) {
		printf("Error opening file\n");
		exit(1);
	} else {
		printf("Matrix generated successfully as %s\n", Name1);
	}
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			randNum = rand() % 19 + (-9);
			fprintf(f, "%2d", randNum);
			fprintf(f, "  ");
		}
		fprintf(f, "\n");
	}

	fclose(f);

	// Generate file kedua
	strcat (Name2, "B.txt");

	f = fopen(Name2, "w");

	if (f == NULL) {
		printf("Error opening file\n");
		exit(1);
	} else {
		printf("Matrix generated successfully as %s\n", Name2);
	}
	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			randNum = rand() % 19 + (-9);
			fprintf(f, "%2d", randNum);
			fprintf(f, "  ");
		}
		fprintf(f, "\n");
	}

	fclose(f);

	return 0;
}
