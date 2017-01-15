//
// Created by mhaponiu on 08.01.17.
//

#ifndef PORR_MAPS_H
#define PORR_MAPS_H

#endif //PORR_MAPS_H

/*
@satisfied_value: warunek stopu; wartosc, dla ktorej wilki moga juz przestac szukac dalej
@max: faktyczna wartosc funkcji w maksimum; przydatne do testow
@min: faktyczna wartosc funkcji w minimum; przydatne do testow
*/
struct Map{
    double (*map)(double, double);
    double satisfied_value;
    double max;
    double min;
};

double calculate_sin(double x, double y);

double calculate_sinc(double x, double y);

double calculate_threeExtremum(double x, double y);

double calculate_rosenbrock(double x, double y);

double calculate_ackleys(double x, double y);
