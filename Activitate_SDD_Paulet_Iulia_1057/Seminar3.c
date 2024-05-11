
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

struct Masina {
	int id;
	char* producator;
	int nrUsi;
	float pret;
};

struct Masina* inserareVector(struct Masina* vec_m, int* dim, struct Masina m) {
	struct Masina* aux = (struct Masina*)malloc(sizeof(struct Masina) * ((*dim) + 1));
	for (int i = 0; i < (*dim); i++) {
		aux[i] = vec_m[i];
	}
	aux[(*dim)] = m;
	(*dim)++;

	if (vec_m != NULL) {
		free(vec_m);
	}

	return aux;
}

struct Masina* citireFisier(const char* numeFisier, int* dim) {
	FILE* f = fopen(numeFisier, "r");
	if (!f) {
		printf("Fisierul nu exista.\n");
		return NULL;
	}

	struct Masina* vec_m = NULL;
	(*dim) = 0;

	char buffer[100];
	char delimiter[] = ",\n";
	while (fgets(buffer, 100, f) != NULL) {

		char* token = strtok(buffer, delimiter);

		struct Masina m;
		m.id = atoi(token);

		token = strtok(NULL, delimiter);
		m.producator = (char*)malloc(strlen(token) + 1);
		strcpy(m.producator, token);

		token = strtok(NULL, delimiter);
		m.nrUsi = atoi(token);

		token = strtok(NULL, delimiter);
		m.pret = atof(token);

		vec_m = inserareVector(vec_m, dim, m);
	}

	fclose(f);
	return vec_m;
}
