//
// Created by mhaponiu on 08.01.17.
//

#include <stdlib.h>
#include <math.h>
#include "wolf.h"
#include "directives.h"


double rand_from_range(double start, double end){
    double diff = fabs(end - start);
    double r = rand()%((int)(diff*100));
    r = r / 100;
    return start + r;
}

int compare(const struct Wolf * a, const struct Wolf * b){
    if ( a->h <  b->h ) return 1;
    if ( a->h == b->h ) return 0;
    if ( a->h >  b->h ) return -1;
}

struct Best get_best(struct Wolf *wolves){
    struct Wolf local_wolves[COUNT_WOLVES];
    for (int i = 0; i < COUNT_WOLVES; ++i) {
        local_wolves[i] = *(wolves+i);
    }
    struct Best best;
    qsort(local_wolves, COUNT_WOLVES, sizeof(struct Wolf), compare);
    best.alpha = local_wolves[0];
    best.beta = local_wolves[1];
    best.delta = local_wolves[2];
    return best;
}

void new_position_when_attack(struct Best best, struct Wolf *wolf){
    double c1, c2, c3, da, db, dd, A1, A2, A3, x1, x2, x3, y1, y2, y3, model_a;
    model_a = MODEL_A

    c1 = rand_from_range(0, 2);
    c2 = rand_from_range(0, 2);
    c3 = rand_from_range(0, 2);

//    dla wspolrzednej x'owej'
    da = fabs(c1 * best.alpha.x - wolf->x);
    db = fabs(c2 * best.alpha.x - wolf->x);
    dd = fabs(c3 * best.alpha.x - wolf->x);
    A1 = rand_from_range(-model_a, model_a);
    A2 = rand_from_range(-model_a, model_a);
    A3 = rand_from_range(-model_a, model_a);
    x1 = best.alpha.x -A1 * da;
    x2 = best.beta.x -A2 * db;
    x3 = best.delta.x -A3 * dd;
    wolf->x = (x1 + x2 + x3)/3.0;

//    dla wspolrzednej y'owej'
    da = fabs(c1 * best.alpha.y - wolf->y);
    db = fabs(c2 * best.alpha.y - wolf->y);
    dd = fabs(c3 * best.alpha.y - wolf->y);
    A1 = rand_from_range(-model_a, model_a);
    A2 = rand_from_range(-model_a, model_a);
    A3 = rand_from_range(-model_a, model_a);
    y1 = best.alpha.y -A1 * da;
    y2 = best.beta.y -A2 * db;
    y3 = best.delta.y -A3 * dd;
    wolf->y = (y1 + y2 + y3)/3.0;
}

void new_position_when_not_attack(struct Best best, struct Wolf *wolf, double A){
    double c, d;
    struct Wolf xp;

    c = rand_from_range(0, 2);

//    dla wspolrzednej x'owej'
    // srednia wazona
    xp.x = (best.alpha.x * best.alpha.h +
            best.beta.x * best.beta.h +
            best.delta.x * best.delta.h) /
           (best.alpha.h + best.beta.h + best.delta.h);
    d = fabs(c * xp.x - wolf->x);
    wolf->x = xp.x - A*d;

//    dla wspolrzednej y'owej'
    // srednia wazona
    xp.y = (best.alpha.y * best.alpha.h +
            best.beta.y * best.beta.h +
            best.delta.y * best.delta.h) /
           (best.alpha.h + best.beta.h + best.delta.h);
    d = fabs(c * xp.y - wolf->y);
    wolf->y = xp.y - A*d;
}