//wersja druga kodu PI

#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <Windows.h>

long long num_steps = 1000000000;
double step;

int main(int argc, char* argv[])
{
	clock_t start, stop;
	double x, pi, sum = 0.0;
	int i;
	step = 1. / (double)num_steps;

#pragma omp parallel 
	{
		HANDLE thread_uchwyt = GetCurrentThread();
		int th_id = omp_get_thread_num();
		//otrzymanie w³asnego identyfikatora 
		//DWORD_PTR mask = (1 << (th_id % 4));
		DWORD_PTR mask;
		if (th_id % 2 == 0)
			mask = (1 << (0));
		else
			mask = (1 << (3));
		//okreœlenie maski dla przetwarzania w¹tku wy³¹cznie na jednym procesorze przydzielanym modulo
			DWORD_PTR result = SetThreadAffinityMask(thread_uchwyt, mask);
		//przekazanie do systemu operacyjnego maski powinowactwa 
		if (result == 0) printf("blad SetThreadAffnityMask \n");
		else
		{
			printf("maska poprzednia dla watku %d : %d\n", th_id, result);
			printf("maska nowa dla watku %d : %d\n", th_id, SetThreadAffinityMask(
				thread_uchwyt, mask));
		}
#pragma omp single
		start = clock();
#pragma omp for private(x) reduction(+:sum)
		for (i = 0; i < num_steps; i++)
		{
			x = (i + .5)*step;
			sum += 4.0 / (1. + x*x);
		}
	} //pragma omp parallel
	pi = sum*step;
	stop = clock();

	printf("Wartosc liczby PI wynosi %15.12f\n", pi);
	printf("Czas przetwarzania wynosi %f sekund\n", ((double)(stop - start) / 1000.0));
	return 0;
}