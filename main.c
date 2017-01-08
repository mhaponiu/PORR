#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>


#define COUNT_WOLVES 10
#define ITER 100

//#define GENERATOR 1
#define GENERATOR time(NULL)

#define MIN_X -2
#define MAX_X 2
#define MIN_Y -2
#define MAX_Y 2

#define MAPA 0 //od 0 do 4

double MODEL_A = 2;
double A_DECR =  0.1;

struct Wolf{
    int id;
    double x;
    double y;
    double h;
};

struct Best{
    struct Wolf alpha;
    struct Wolf beta;
    struct Wolf delta;
};


double rand_from_range(double start, double end){
    double diff = fabs(end - start);
    double r = rand()%((int)(diff*100));
    r = r / 100;
    return start + r;
}

//MAPA NR 0
double calculate_sin(double x, double y){
    return sin(x*y);
}
//MAPA NR 1
double calculate_sinc(double x, double y){
    return 10*sin(sqrt(pow(x,2)+pow(y,2))) / (sqrt(pow(x,2)+pow(y,2)));
}

//MAPA NR 2
double calculate_threeExtremum(double x, double y){
    return 1 / (1 + pow(x,2) + (y + .2 * pow(sin(10*y),2)));
}

//MAPA NR 3
double calculate_rosenbrock(double x, double y){
    int a = 1;
    int b = 100;
    return -pow((a - x),2) - b*pow((y - pow(x,2)),2);
}

//MAPA NR 4
double calculate_ackleys(double x, double y){
    return -exp(-0.2)*sqrt(pow(x,2)+pow(y,2))-3*(cos(2*x)+sin(2*y));
}

//TABLICA POWYZSZYCH FUNKCJI TERENU
double (*math_funs[])(double, double)={calculate_sin,
                                        calculate_sinc,
                                        calculate_threeExtremum,
                                        calculate_rosenbrock,
                                        calculate_ackleys};

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
    double c1, c2, c3, da, db, dd, A1, A2, A3, x1, x2, x3, y1, y2, y3;

    c1 = rand_from_range(0, 2);
    c2 = rand_from_range(0, 2);
    c3 = rand_from_range(0, 2);

//    dla wspolrzednej x'owej'
    da = fabs(c1 * best.alpha.x - wolf->x);
    db = fabs(c2 * best.alpha.x - wolf->x);
    dd = fabs(c3 * best.alpha.x - wolf->x);
    A1 = rand_from_range(-MODEL_A, MODEL_A);
    A2 = rand_from_range(-MODEL_A, MODEL_A);
    A3 = rand_from_range(-MODEL_A, MODEL_A);
    x1 = best.alpha.x -A1 * da;
    x2 = best.beta.x -A2 * db;
    x3 = best.delta.x -A3 * dd;
    wolf->x = (x1 + x2 + x3)/3.0;

//    dla wspolrzednej y'owej'
    da = fabs(c1 * best.alpha.y - wolf->y);
    db = fabs(c2 * best.alpha.y - wolf->y);
    dd = fabs(c3 * best.alpha.y - wolf->y);
    A1 = rand_from_range(-MODEL_A, MODEL_A);
    A2 = rand_from_range(-MODEL_A, MODEL_A);
    A3 = rand_from_range(-MODEL_A, MODEL_A);
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

int main() {
    srand(GENERATOR);
    struct Wolf wolves[COUNT_WOLVES];
    struct Best best;

/*
    LOSUJE POLOZENIE WILKOW NA MAPIE
*/
    printf("LOSUJE POLOZENIE WILKOW NA MAPIE\n");
//<<<<<<<<<<<<<<  ZROWNOLEGL MNIE  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    for (int i = 0; i < COUNT_WOLVES; ++i) {
        wolves[i].id = i;
        wolves[i].x = rand_from_range(MIN_X, MAX_X);
        wolves[i].y = rand_from_range(MIN_Y, MAX_Y);
        wolves[i].h = math_funs[MAPA](wolves[i].x, wolves[i].y);
        printf("Wilk %d = (%f, %f)%f\n", wolves[i].id, wolves[i].x, wolves[i].y, wolves[i].h);
    }

    double A;
    best = get_best(wolves);
//    printf("Alpha h=%f wilk %d\n", best.alpha.h, best.alpha.id);

    for (int k = 0; k < ITER; ++k) {
        printf("iteracja %d ", k);
        printf("Alpha h=%f wilk %d\n", best.alpha.h, best.alpha.id);
//<<<<<<<<<<<<<<  ZROWNOLEGL MNIE  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        for (int j = 0; j < COUNT_WOLVES; ++j) {
            A = rand_from_range(-MODEL_A, MODEL_A);
            if(fabs(A) > 1){
                new_position_when_attack(best, &wolves[j]);
            }
            else{
                new_position_when_not_attack(best, &wolves[j], A);
            }
            wolves[j].h = math_funs[MAPA](wolves[j].x, wolves[j].y);
        }
        MODEL_A = MODEL_A - A_DECR;
        if(MODEL_A < 0){
            MODEL_A = 2;
        }
        best = get_best(wolves);
    }
    printf("Alpha h=%f wilk %d\n", best.alpha.h, best.alpha.id);

//    printf("%f", rand_from_range(2.5, 3.5));
    return 0;
}