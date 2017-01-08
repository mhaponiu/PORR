//
// Created by mhaponiu on 08.01.17.
//

#include <math.h>
#include "maps.h"

//MAPA NR 0
double calculate_sin(double x, double y){
    //fale rochodzace sie na krzyz
    return sin(x*y);
}
//MAPA NR 1
double calculate_sinc(double x, double y){
    //fale rochodzace sie na krzyz
    return 10*sin(sqrt(pow(x,2)+pow(y,2))) / (sqrt(pow(x,2)+pow(y,2)));
}

//MAPA NR 2
double calculate_threeExtremum(double x, double y){
    // trzy gorki na srodku najwieksza
    return 1 / (1 + pow(x,2) + (y + .2 * pow(sin(10*y),2)));
}

//MAPA NR 3
double calculate_rosenbrock(double x, double y){
    // wulkan
    int a = 1;
    int b = 100;
    return -pow((a - x),2) - b*pow((y - pow(x,2)),2);
}

//MAPA NR 4
double calculate_ackleys(double x, double y){
    // bardzo duzo gorek
    return -exp(-0.2)*sqrt(pow(x,2)+pow(y,2))-3*(cos(2*x)+sin(2*y));
}
