#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>

typedef struct Santier Santier;
typedef struct Nod Nod;

struct Santier {
    char* numeProiect;
    int nrMuncitori;
    float suprafata;
};

struct Nod {
    Santier santier;
    Nod* next;
};

Santier initializareSantier(const char* numeProiect, int nrMuncitori, float suprafata) {
    Santier santier;
    santier.numeProiect = (char*)malloc(strlen(numeProiect) + 1);
    strcpy(santier.numeProiect, numeProiect);
    santier.nrMuncitori = nrMuncitori;
    santier.suprafata = suprafata;

    return santier;
}

Nod* inserareInceput(Santier santier, Nod* lista) {
    Nod* nod = (Nod*)malloc(sizeof(Nod));
    nod->santier = santier;
    nod->next = lista;
    return nod;
}

void afisareSantier(Santier santier) {
    printf("\nSantierul %s are %d muncitori si %5.2f m^2", santier.numeProiect, santier.nrMuncitori, santier.suprafata);
}

void afiseazaLista(Nod* lista) {
    while (lista != NULL) {
        //procesare
        afisareSantier(lista->santier);
        lista = lista->next;
    }
}

int totalMuncitori(Nod* lista) {
    int total = 0;
    while (lista != NULL) {
        total += lista->santier.nrMuncitori;
        lista = lista->next;
    }
    return total;
}

//numele santierului cu densitatea cea mai mare de muncitori pe m^2

float densitate(Santier santier) {
    if (santier.suprafata != 0) {
        return santier.nrMuncitori / santier.suprafata;
    }
    return 0;
}

char* numeDensitateMax(Nod* lista) {
    float max = 0;
    char* aux = NULL;
    while (lista != NULL) {
        if (densitate(lista->santier) > max) {
            max = densitate(lista->santier);
            aux = lista->santier.numeProiect;
        }
        lista = lista->next;
    }
    if (aux != NULL) {
        char* nume = (char*)malloc(strlen(aux) + 1);
        strcpy(nume, aux);
    }
    else
        return NULL;
}

void stergereLista(Nod** lista) {
    while ((*lista) != NULL) {
        free((*lista)->santier.numeProiect);
        Nod* aux = (*lista)->next;
        free((*lista));
        (*lista) = aux;
    }
    (*lista) = NULL;
}

int main() {

    Nod* lista = NULL;
    Santier s1 = initializareSantier("Santier 1", 10, 300);
    Santier s2 = initializareSantier("Santier 2", 12, 400);
    Santier s3 = initializareSantier("Santier 3", 15, 200);
    lista = inserareInceput(s1, lista);
    lista = inserareInceput(s2, lista);
    lista = inserareInceput(s3, lista);
    lista = inserareInceput(initializareSantier("Santier 4", 10, 340), lista);
    afiseazaLista(lista);
    int totalM = totalMuncitori(lista);
    printf("\nTotal muncitori: %d", totalM);
    char* nume = numeDensitateMax(lista);
    printf("\nNumele santierului cu densitatea maxima: %s", nume);
    stergereLista(&lista);
    afiseazaLista(lista);
    return 0;
}