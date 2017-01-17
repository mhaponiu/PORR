//
// Created by mhaponiu on 08.01.17.
//

#ifndef PORR_WOLF_H
#define PORR_WOLF_H

#endif //PORR_WOLF_H

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

struct Best get_best_linear(struct Wolf *wolves);

void new_position_when_attack(struct Best best, struct Wolf *wolf);

void new_position_when_not_attack(struct Best best, struct Wolf *wolf, double A);

double rand_from_range(double start, double end);