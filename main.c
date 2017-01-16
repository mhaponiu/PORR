#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <sys/time.h>
#include "wolf.h"
#include "directives.h"
#include "maps.h"

int main() {


    srand(GENERATOR);
    static struct Wolf wolves[COUNT_WOLVES];
    struct Best best;
    struct Map map0, map1, map2, map3, map4;
    struct timeval arrayTimerStart, arrayTimerStop, arrayTimerResult;
    //    TODO poprzypisywac dla kazdej mapy przynajmniej warunki stopu -> satisfied_value
    map0.map = calculate_sin; map0.satisfied_value = 0.99999; map0.max = 1; map0.min = -1;
    map1.map = calculate_sinc; map1.min = -2.17234;
    map2.map = calculate_threeExtremum;
    map3.map = calculate_rosenbrock; map3.max = 0;
    map4.map = calculate_ackleys; map4.max = 4.5901; map4.min = -44.8714;
    struct Map maps[] = {map0, map1, map2, map3, map4};

/*
    LOSUJE POLOZENIE WILKOW NA MAPIE
*/
#ifdef DEBUG
    printf("LOSUJE POLOZENIE WILKOW NA MAPIE\n");
#endif

#ifdef TIME
    //pomiar czasu wstawiania
    gettimeofday(&arrayTimerStart, NULL);
#endif

#ifdef PARALLEL
    #pragma omp parallel num_threads(THREADS_COUNTER) shared(wolves)
#endif
    {
#ifdef PARALLEL
    #pragma omp for
#endif
        for (int i = 0; i < COUNT_WOLVES; ++i) {
            wolves[i].id = i;
            wolves[i].x = rand_from_range(MIN_X, MAX_X);
            wolves[i].y = rand_from_range(MIN_Y, MAX_Y);
            wolves[i].h = maps[MAPA].map(wolves[i].x, wolves[i].y);
#ifdef DEBUG
            printf("Bec bec, tu watek nr %d, 2017 no witam witam bec \n",omp_get_thread_num());
            printf("Wilk %d = (%f, %f) h= %f\n", wolves[i].id, wolves[i].x, wolves[i].y, wolves[i].h);
#endif
        }
    };
#ifdef TIME
    //pomiar czasu wstawiania
    gettimeofday(&arrayTimerStop, NULL);
    timersub(&arrayTimerStop, &arrayTimerStart, &arrayTimerResult);
    printf("Wstawianie zajęło %ld.%06ld sekund\n", arrayTimerResult);
#endif


    double A;
    best = get_best(wolves);
    double model_a = MODEL_A;
    double a_decr = A_DECR;
    printf("Start %d wilków\n", COUNT_WOLVES);

#ifdef TIME
    //pomiar czasu wykonania
    struct timeval start, stop, result;
    gettimeofday(&start, NULL);
#endif

//    warunki zakonczenia algorytmu w zaleznosci od dyrektywy ITER
#ifdef ITER
    printf("łącznie %d iteracji\n", ITER);
    for (int k = 0; k < ITER; ) {
#endif
#ifndef ITER
    printf("biegają dopóki nie osiągną wartości %f\n", maps[MAPA].satisfied_value);
    int k = 0;
    while(best.alpha.h <= maps[MAPA].satisfied_value){
#endif
#ifdef DEBUG
        printf("iteracja %d ", k);
        printf("Alpha h=%f wilk %d\n", best.alpha.h, best.alpha.id);
#endif;

#ifdef PARALLEL
        #pragma omp parallel num_threads(THREADS_COUNTER) shared(wolves)
#endif
        {
#ifdef PARALLEL
#pragma omp for schedule(static)
#endif
            for (int j = 0; j < COUNT_WOLVES; ++j) {
                A = rand_from_range(-model_a, model_a);
                if (fabs(A) > 1) {
                    new_position_when_attack(best, &wolves[j]);
                } else {
                    new_position_when_not_attack(best, &wolves[j], A);
                }
                wolves[j].h = maps[MAPA].map(wolves[j].x, wolves[j].y);
            }


            model_a = model_a - a_decr;
            if (model_a < 0) {
                model_a = 2;
            }
            best = get_best_linear(wolves);
            ++k;
        }
    }

#ifdef TIME
    //pomiar czasu wykonania
    gettimeofday(&stop, NULL);
    timersub(&stop, &start, &result);
    printf("wykonanie zajęło %ld.%06ld sekund\n", result);
#endif

    printf("Alpha h=%f wilk %d\n", best.alpha.h, best.alpha.id);

    return 0;
}