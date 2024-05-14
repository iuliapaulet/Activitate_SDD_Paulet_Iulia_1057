#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct Rezervare Rezervare;
struct Rezervare {
	int id;
	char* numeClient;
	int nrPersoane;
};

typedef struct Nod Nod;
struct Nod {
	Nod* stanga;
	Nod* dreapta;
	Rezervare info;
};

Rezervare citireRezervare(FILE* f) {
	Rezervare r;
	fscanf(f, "%d", &r.id);
	char aux[30];
	fscanf(f, "%s", &aux);
	r.numeClient = (char*)malloc(strlen(aux) + 1);
	strcpy(r.numeClient, aux);
	fscanf(f, "%d", &r.nrPersoane);
	return r;
}

Nod* rotireLaDreapta(Nod* rad) {
	Nod* aux = rad->stanga;
	if (rad) {
		rad->stanga = aux->dreapta;
		aux->dreapta = rad;
	}
	return aux;
}

Nod* rotireLaStanga(Nod* rad) {
	Nod* aux = rad->dreapta;
	if (rad) {
		rad->dreapta = rad->stanga;
		aux->stanga = rad;
	}
	return aux;
}

int inaltimeArbore(Nod* rad) {
	if (rad) {
		int contInaltimeStanga = inaltimeArbore(rad->stanga);
		int contInaltimeDreapta = inaltimeArbore(rad->dreapta);
		int maxim;
		/*if (contInaltimeDreapta < contInaltimeStanga) {
			maxim = contInaltimeStanga;
		}
		else {
			maxim = contInaltimeDreapta;
		}
		maxim++;
		return maxim;*/
		return 1 + (contInaltimeStanga > contInaltimeDreapta ? contInaltimeStanga : contInaltimeDreapta);
	}
	return 0;
}

int calculEchilibru(Nod* rad) {
	if (rad) {
		int inaltimeStanga = inaltimeArbore(rad->stanga);
		int inaltimeDreapta = inaltimeArbore(rad->dreapta);
		return inaltimeStanga - inaltimeDreapta;
	}
	return 0;
}


Nod* inserareArbore(Rezervare r, Nod* arbore) {
	if (arbore != NULL) {
		if (arbore->info.id < r.id) {
			arbore->dreapta = inserareArbore(r, arbore->dreapta);
		}
		else {
			arbore->stanga = inserareArbore(r, arbore->stanga);
		}

		int gradEchilibru = calculEchilibru(arbore);
		if (gradEchilibru == 2) {
			if (calculEchilibru(arbore->stanga) == 1) {
				arbore = rotireLaDreapta(arbore);
			}
			else {
				arbore->stanga = rotireLaStanga(arbore->stanga);
				arbore = rotireLaDreapta(arbore->stanga);
			}
		}

		if (gradEchilibru == -2) {
			if (calculEchilibru(arbore->dreapta) == 1) {
				arbore->dreapta = rotireLaDreapta(arbore->dreapta);
			}
			arbore = rotireLaStanga(arbore);
		}

	}
	else {
		Nod* rad = (Nod*)malloc(sizeof(Nod));
		rad->dreapta = NULL;
		rad->stanga = NULL;
		rad->info = r;
		arbore = rad;
	}
	return arbore;
}

void citireFisier(const char* numeFisier, Nod** radacina) {
	FILE* f = fopen(numeFisier, "r");
	int nrNoduri;
	fscanf(f, "%d", &nrNoduri);
	for (int i = 0; i < nrNoduri; i++) {
		Rezervare r = citireRezervare(f);
		(*radacina) = inserareArbore(r, (*radacina));
	}
	fclose(f);
}

void afisareRezervare(Rezervare r) {
	printf("%d. %s are rezervare de %d persoane\n", r.id, r.numeClient, r.nrPersoane);
}

void afisareArborePreordine(Nod* arbore) {
	if (arbore != NULL) {
		afisareRezervare(arbore->info);
		afisareArborePreordine(arbore->stanga);
		afisareArborePreordine(arbore->dreapta);
	}
}

void afisareArboreInordine(Nod* arbore) {
	if (arbore != NULL) {
		afisareArboreInordine(arbore->stanga);
		afisareRezervare(arbore->info);
		afisareArboreInordine(arbore->dreapta);
	}
}

void afisareArborePostordine(Nod* arbore) {
	if (arbore != NULL) {
		afisareArborePostordine(arbore->stanga);
		afisareArborePostordine(arbore->dreapta);
		afisareRezervare(arbore->info);
	}
}

Rezervare cautaRezervareDupaId(Nod* rad, int id) {
	if (rad != NULL) {
		if (rad->info.id < id) {
			return cautaRezervareDupaId(rad->dreapta, id);
		}
		else {
			if (rad->info.id > id) {
				return cautaRezervareDupaId(rad->stanga, id);
			}
			else {
				return rad->info;
			}
		}
	}
	else {
		Rezervare r;
		r.id = -1;
		r.numeClient = NULL;
		r.nrPersoane = -1;
		return r;
	}
}

int calculNrTotalPersoane(Nod* rad) {
	if (rad != NULL) {
		int rPersoane = rad->info.nrPersoane;
		int sPersoane = calculNrTotalPersoane(rad->stanga);
		int dPersoane = calculNrTotalPersoane(rad->dreapta);
		int suma = rPersoane + sPersoane + dPersoane;
		return suma;
	}
	else {
		return 0;
	}
}

void stergereArbore(Nod** rad) {
	if ((*rad)) {
		stergereArbore(&(*rad)->stanga);
		stergereArbore(&(*rad)->dreapta);
		free((*rad)->info.numeClient);
		free((*rad));
		(*rad) = NULL;
	}
}

int main() {
	Nod* radacina = NULL;
	citireFisier("rezervari.txt", &radacina);
	afisareArborePreordine(radacina);
	//printf("\n\n");
	//afisareArboreInordine(radacina);
	//printf("\n\n");
	//afisareArborePostordine(radacina);
	//printf("\nRezervarea cautata: ");
	afisareRezervare(cautaRezervareDupaId(radacina, 8));
	printf("\nNumar total persoane: %d", calculNrTotalPersoane(radacina));
	stergereArbore(&radacina);
	afisareArboreInordine(radacina);
	return 0;
}