#include <stdio.h>
//#include <sys/time.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "wolf.h"
#include "directives.h"
#include "maps.h"
#include "mpi.h"

int main(int argc, char * argv[]) {

#ifdef MPI
	// rank -> numer procesu; size -> ogolna liczba procesow; values-> zmienne x,y,h przekazywane w komunikacie
	int rank;
	int size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Request send;
	double values[3];
	double start, stop;
	//tablica uber przechowuje w sobie X,Y,H 3 najlepszych wilkow kazdego procesu
	// nie moge po prostu wyslac best, bo Best nie jest typem prostym
	double uber[3];
	double *uberWolfpack;
	uberWolfpack = (double *)malloc(3 * size * sizeof(double));
	struct Wolf top;
	top.id = -1;
	top.x = 0;
	top.y = 0;
	top.h = 0;
	struct Wolf *pretendents;
	pretendents = (struct Wolf *)malloc(size * sizeof(struct Wolf));
#endif

	int iter = atoi(argv[1]);
	int count_wolves = atoi(argv[2]);
	int mapa = atoi(argv[3]);

	srand(time(NULL)*rank);
	//dla kazdego procesora wyliczamy ile bedzie mial wilkow
	int number = count_wolves / size;
	if (rank < count_wolves % size) { number++; }

	struct Wolf *wolves = (struct Wolf *)malloc(number * sizeof(struct Wolf));
	struct Best best;
	struct Map map0, map1, map2, map3, map4;
	map0.map = calculate_sin; map0.satisfied_value = 0.99999; map0.max = 1; map0.min = -1;
	map1.map = calculate_sinc; map1.satisfied_value = 9.999; map1.min = -2.17234;
	map2.map = calculate_threeExtremum;
	map3.map = calculate_rosenbrock; map3.max = 0;
	map4.map = calculate_ackleys; map4.max = 4.5901; map4.min = -44.8714;
	struct Map maps[] = { map0, map1, map2, map3, map4 };

	/*
	LOSUJE POLOZENIE WILKOW NA MAPIE
	*/
	for (int i = 0; i < number; ++i) {
		wolves[i].id = i;
		wolves[i].x = rand_from_range(MIN_X, MAX_X);
		wolves[i].y = rand_from_range(MIN_Y, MAX_Y);
		wolves[i].h = maps[mapa].map(wolves[i].x, wolves[i].y);
	}
	double A;
	double model_a = MODEL_A;
	// a jest losowane jedynie przez roota i rozsylane broadcastem do reszty
	if (rank == 0) {
		A = rand_from_range(-model_a, model_a);
	}
	MPI_Bcast(&A, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);
	double a_decr = A_DECR;
	// kazdy procesor wylicza sobie najlepsze wilki, podajemy tez liczbe wilkow, zeby funkcja wiedziala jak liczyc
	best = get_best(wolves, number);

#ifdef MPI
	int flag = 0;
	MPI_Barrier(MPI_COMM_WORLD); 
	start = MPI_Wtime();
#endif
	//    warunki zakonczenia algorytmu w zaleznosci od dyrektywy iter
#ifdef ITER
	for (int k = 0; k < iter; ) {
#endif
				//root losuje nowe A i potem wysyla je do reszty
				if (rank == 0) {
					A = rand_from_range(-model_a, model_a);
				}
				MPI_Bcast(&A, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
				for (int j = 0; j < number; ++j) {
					if (fabs(A) > 1) {
						new_position_when_attack(best, &wolves[j]);
					}
					else {
						new_position_when_not_attack(best, &wolves[j], A);
					}									
					wolves[j].h = maps[mapa].map(wolves[j].x, wolves[j].y);
				}
				model_a = model_a - a_decr;
				if (model_a < 0) {
					model_a = 2;
				}
#ifdef MPI
				// przygotowuje wilka do przeslania
				best = get_best(wolves, number);
				uber[0] = best.alpha.x;
				uber[1] = best.alpha.y;
				uber[2] = best.alpha.h;
				
				// Gather: jesli nie jestes rootem, wyslij 9 wartosci double z tablicy uber
				// jesli jestes rootem przyjmij po 9 wartosci double od kazdego (wlacznie ze soba) do tablicy uberWolfpack
				MPI_Barrier(MPI_COMM_WORLD);
				MPI_Gather(uber, 3, MPI_DOUBLE, uberWolfpack, 3, MPI_DOUBLE, 0, MPI_COMM_WORLD);

				if (rank == 0) {
					// tworze tablice pretendentow do wilka alpha
					for (int counter = 0; counter < size; counter++) {
						pretendents[counter].id = counter;
						pretendents[counter].x = uberWolfpack[counter * 3];
						pretendents[counter].y = uberWolfpack[(counter * 3 + 1)];
						pretendents[counter].h = uberWolfpack[counter * 3 + 2];
					}
					best = get_best(pretendents, size);
					uber[0] = best.alpha.x;
					uber[1] = best.alpha.y;
					uber[2] = best.alpha.h;
				}
				MPI_Barrier(MPI_COMM_WORLD);
				MPI_Bcast(&uber, 3, MPI_DOUBLE, 0, MPI_COMM_WORLD);
				wolves[best.alpha.id].x = uber[0];
				wolves[best.alpha.id].y = uber[1];
				wolves[best.alpha.id].h = uber[2];
#ifdef ITER
				if (top.h< best.alpha.h) {
					top.x = best.alpha.x;
					top.y = best.alpha.y;
					top.h = best.alpha.h;
				}
#endif

#endif	
				k++;
			}


#ifdef MPI	
			stop = MPI_Wtime();
			if (!rank) {
				printf("Liczba wilków: %d. Liczba iteracji: %d. Liczba procesow: %d. Caly proces zajal: %f sekund.\n", count_wolves, iter, size, (stop - start));
			}
			MPI_Finalize();
#endif	
			return 0;
		}