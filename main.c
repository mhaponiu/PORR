#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "wolf.h"
#include "directives.h"
#include "maps.h"

int main() {
    srand(GENERATOR);
    struct Wolf wolves[COUNT_WOLVES];
    struct Best best;
    struct Map map0, map1, map2, map3, map4;
//    TODO poprzypisywac dla kazdej mapy przynajmniej warunki stopu -> satisfied_value
    map0.map = calculate_sin; map0.satisfied_value = 0.99999; map0.max = 1; map0.min = -1;
    map1.map = calculate_sinc; map1.min = -2.17234;
    map2.map = calculate_threeExtremum;
    map3.map = calculate_rosenbrock; map3.max = 0;
    map4.map = calculate_ackleys; map4.max = 4.5901; map4.min = -44.8714;
    struct Map maps[] = {map0, map1, map2, map3, map4};
    int thr_id, num_threads=1;
    long int i, j, k;

/*
    LOSUJE POLOZENIE WILKOW NA MAPIE
*/
#ifdef DEBUG
    printf("LOSUJE POLOZENIE WILKOW NA MAPIE\n");
#endif

#ifdef PARALLEL
#pragma omp parallel
{
#endif
#pragma omp for nowait
    for (i = 0; i < COUNT_WOLVES; ++i) {
        (wolves+i)->id = i;
        (wolves+i)->x = rand_from_range(MIN_X, MAX_X);
        (wolves+i)->y = rand_from_range(MIN_Y, MAX_Y);
        (wolves+i)->h = maps[MAPA].map((wolves+i)->x, (wolves+i)->y);
#ifdef DEBUG
        printf("Wilk %d = (%f, %f) h= %f\n", wolves[i].id, wolves[i].x, wolves[i].y, wolves[i].h);
#endif
    }
#ifdef PARALLEL
}
#endif

    best = get_best(wolves);
    double model_a = MODEL_A;
    double A = rand_from_range(-model_a, model_a);
    double a_decr = A_DECR;
    printf("Start %d wilków\n", COUNT_WOLVES);

#ifdef TIME
    //pomiar czasu wykonania
    struct timeval start, stop, result;
    gettimeofday(&start, NULL);
#endif
    printf("łącznie %d iteracji\n", ITER);

//#ifdef PARALLEL //powinien byc na wewnetrznej petli? nizej?
//#pragma omp parallel shared(model_a, best, num_threads, A) private(k) firstprivate(a_decr)
//    {
//#endif
    for (k=0; k < ITER; ++k) {
#ifdef DEBUG
        printf("iteracja %d\n ", k);
        printf("Alpha h=%f wilk %d\n", best.alpha.h, best.alpha.id);
#endif

#ifdef PARALLEL //powinien byc na zewnetrznej petli? wyzej?
#pragma omp parallel shared(model_a, best, num_threads, A) firstprivate(a_decr)
{
#endif
#pragma omp for nowait private(j)
        for (j = 0; j < COUNT_WOLVES; ++j) {
            if (omp_get_thread_num() == 0) {
                //tylko 1 watek powinien to policzyc
                A = rand_from_range(-model_a, model_a);
            }
            if (fabs(A) > 1) {
                new_position_when_attack(best, &wolves[j]);
            } else {
                new_position_when_not_attack(best, &wolves[j], A);
            }
                wolves[j].h = maps[MAPA].map(wolves[j].x, wolves[j].y);
        }

#ifndef ITER
            ++k; //do while'a potrzebne
#endif
        if (omp_get_thread_num() == 0) {
            //tylko 1 watek powinien to policzyc
            num_threads = omp_get_num_threads();
            model_a = model_a - a_decr;
            if (model_a < 0) {
                model_a = 2;
            }
            best = get_best(wolves);
        }
#pragma omp barrier
#ifdef PARALLEL
}
#endif
    } // zamyka zewnetrzną pętle


#ifdef TIME
    //pomiar czasu wykonania
    gettimeofday(&stop, NULL);
    timersub(&stop, &start, &result);
    printf("wykonanie zajęło %ld.%06ld sekund\n", result);
#endif
    printf("program wykonywało %d wątków\n", num_threads);
    printf("Alpha h=%f wilk %ld\n", best.alpha.h, best.alpha.id);

    return 0;
}