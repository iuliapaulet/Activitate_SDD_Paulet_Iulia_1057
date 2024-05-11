
#include <stdio.h>
#include <malloc.h>


struct Cofetarie {
	char* nume;
	int nrAngajati;
	float* salarii;
};

void afiseazaCofetarie(struct Cofetarie c)
{
	if (c.nume != NULL)
	{
		printf("Nume: %s\n", c.nume);
	}
	if (c.salarii != NULL)
	{
		printf("Salarii: ");
		for (int i = 0; i < c.nrAngajati; i++)
		{
			//5.2 afiseaza cu 2 zecimale, f de la float
			printf("%5.2f, ", c.salarii[i]);
		}
	}
	printf("Numar angajati: %d:\n", c.nrAngajati);
	printf("\n");
}


//la pointeri char* si float* ii punem constanti ca sa nu ne permita modificare
struct Cofetarie initializareCofetarie(const char* nume, int nrAngajati, const float* salarii)
{
	struct Cofetarie c;
	c.nume = malloc(strlen(nume) + 1);
	strcpy(c.nume, nume);
	c.nrAngajati = nrAngajati;
	c.salarii = malloc(sizeof(float) * nrAngajati);
	for (int i = 0; i < nrAngajati; i++)
	{
		c.salarii[i] = salarii[i];
	}
	return c;
}

//daca nu transmitem prin referinta, o sa avem dangling pointers
void stergeCofetarie(struct Cofetarie* c)
{
	if (c->nume != NULL)
	{
		free(c->nume);
	}
	free(c->salarii);
	c->nume = NULL;
	c->salarii = NULL;
}

int main()
{
	struct Cofetarie cofetarie1;
	//malloc returneaza un pointer la o adresa de mem
	//facem cast explicit de la void la char --> (char*)
	//Dulce
	cofetarie1.nume = (char*)malloc(strlen("Dulce") + 1);
	strcpy(cofetarie1.nume, "Dulce");
	//initializare
	cofetarie1.nrAngajati = 3;
	//cast implicit, puteam face si cast explicit (float*)
	cofetarie1.salarii = malloc(sizeof(float) * cofetarie1.nrAngajati);


	for (int i = 0; i < cofetarie1.nrAngajati; i++)
	{
		cofetarie1.salarii[i] = (i + 1) * 1000;
	}


	//printf("%s", cofetarie1.nume);

	afiseazaCofetarie(cofetarie1);

	float* vector = malloc(sizeof(float) * 2);
	vector[0] = 4567;
	vector[1] = 3456;
	struct Cofetarie cofetarie2 = initializareCofetarie("Mara", 2, vector);

	afiseazaCofetarie(cofetarie2);

	free(vector);

	//aici ii dam adresa cofetariei
	stergeCofetarie(&cofetarie2);
	afiseazaCofetarie(cofetarie2);

	//free(cofetarie2.nume);
	//free(cofetarie2.salarii);

	free(cofetarie1.salarii);
	free(cofetarie1.nume);



	return 0;
}
