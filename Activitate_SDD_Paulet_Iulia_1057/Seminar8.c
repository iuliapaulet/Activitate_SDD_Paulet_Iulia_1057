#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <malloc.h>
#include<string.h>

typedef struct Mail Mail;
struct Mail {
	char* text;
	int prioritate;
};

typedef struct Heap Heap;
struct Heap {
	Mail* vector;
	int dim;
};

Heap heap_init(int dim) {
	Heap h;
	h.vector = (Mail*)malloc(sizeof(Mail) * dim);
	h.dim = dim;

	return h;
}

Mail mail_init(const char* text, int prioritate) {
	Mail m;
	m.text = (char*)malloc(strlen(text) + 1);
	strcpy(m.text, text);
	m.prioritate = prioritate;

	return m;
}

void mail_afisare(Mail m) {
	printf("-Mesaj: %s\n", m.text);
	printf("-Prioritate: %d\n\n", m.prioritate);
}

void heap_afisare(Heap h) {
	for (int i = 0; i < h.dim; i++) {
		printf("Mail %d:\n", i);
		mail_afisare(h.vector[i]);
	}
}

void heap_dezalocare(Heap* h) {
	for (int i = 0; i < h->dim; i++) {
		free(h->vector[i].text);
	}
	free(h->vector);
	h->vector = NULL;
	h->dim = 0;
}

void heap_filter(Heap h, int poz) {
	int fs = poz * 2 + 1; //fiul din stanga
	int fd = poz * 2 + 2; //fiul din dreapta
	int poz_max = poz;


	if (fs < h.dim && h.vector[poz_max].prioritate < h.vector[fs].prioritate) {
		poz_max = fs;
	}

	if (fd < h.dim && h.vector[poz_max].prioritate < h.vector[fd].prioritate) {
		poz_max = fd;
	}

	if (poz_max != poz) {
		if (h.vector[poz_max].prioritate > h.vector[poz].prioritate) {
			//deep copy
			Mail aux = mail_init(h.vector[poz_max].text, h.vector[poz_max].prioritate);
			h.vector[poz_max] = mail_init(h.vector[poz].text, h.vector[poz].prioritate);
			h.vector[poz] = mail_init(aux.text, aux.prioritate);

			if (poz_max <= (h.dim - 2) / 2) {
				heap_filter(h, poz_max);
			}
		}
	}
}

Mail heap_extragere(Heap* h) {

	if (h->dim > 0) {
		Mail m = (*h).vector[0];

		Mail* aux = (Mail*)malloc(sizeof(Mail) * (*h).dim - 1);

		for (int i = 1; i < h->dim; i++) {
			aux[i - 1] = h->vector[i];
		}

		free(h->vector);
		h->dim--;
		h->vector = aux;

		for (int i = (h->dim - 2) / 2; i >= 0; i--) {
			heap_filter((*h), i);
		}

		return m;
	}
}

void heap_insert(Heap* h, Mail m) {
	Mail* aux = (Mail*)malloc(sizeof(Mail) * ((*h).dim + 1));

	aux[0] = m;
	for (int i = 1; i < h->dim + 1; i++) {
		aux[i] = h->vector[i - 1];
	}


	free(h->vector);
	h->dim++;
	h->vector = aux;
	heap_filter((*h), 0);
}

void main() {
	Heap heap = heap_init(6);

	heap.vector[0] = mail_init("what0", 7);
	heap.vector[1] = mail_init("what1", 9);
	heap.vector[2] = mail_init("what2", 3);
	heap.vector[3] = mail_init("what3", 8);
	heap.vector[4] = mail_init("what4", 6);
	heap.vector[5] = mail_init("what5", 10);

	heap_afisare(heap);

	printf("\n--post-filtrare--\n");
	for (int i = (heap.dim - 2) / 2; i >= 0; i--) {
		heap_filter(heap, i);
	}

	heap_afisare(heap);

	printf("\n--post-extragere--\n");
	mail_afisare(heap_extragere(&heap));
	mail_afisare(heap_extragere(&heap));
	mail_afisare(heap_extragere(&heap)); //aceste 3 mailuri nu mai pot fi sterse

	printf("\n--post-inserare--\n");
	Mail mail = mail_init("inserted", 11);
	heap_insert(&heap, mail);
	heap_afisare(heap);

	heap_dezalocare(&heap);
}