//wersja druga kodu PI3

#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <fstream>
#include <iostream>

using namespace std;

long long num_steps = 500000000;
double step;

int main(int argc, char* argv[])
{
	omp_set_num_threads(2);
	fstream plik;
	plik.open("reuslts.txt", ios::out);
	if (plik.good() == true)
	{
		printf("Otworzono plik");
	}

	clock_t start, stop;
	double x, pi, sum = 0.0;
	double tab[21];
	int i;
	step = 1. / (double)num_steps;

	for (int j = 0; j < 20; j++){
		for (int i = 0; i < 21; i++)
			tab[i] = 0.0;
		sum = 0.0;
		start = clock();

#pragma omp parallel for private(x)
		for (i = 0; i < num_steps; i++)
		{
			//printf(" by thread %d\n", omp_get_thread_num());
			x = (i + .5)*step;
			tab[omp_get_thread_num() + j] += 4.0 / (1. + x*x);
		}

		for each (double k in tab){
			sum += k;
		}

		pi = sum*step;
		stop = clock();

		printf("Wartosc liczby PI wynosi %15.12f\n", pi);
		printf("Czas przetwarzania wynosi %f sekund\n\n", ((double)(stop - start) / 1000.0));
		plik << j << " " << ((double)(stop - start) / 1000.0) << endl;
	} // for
	plik.close();
	return 0;
}