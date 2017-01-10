//
// Created by mhaponiu on 08.01.17.
//

#ifndef PORR_DIRECTIVES_H
#define PORR_DIRECTIVES_H

#endif //PORR_DIRECTIVES_H

#define MODEL_A 2;
#define A_DECR 0.1;

//########################## PARAMETRY DO USTAWIENIA ############################
//odkomentuj wybrany albo zrob to przez flage kompilacji np -DITER=100
//odkomentowane tutaj dyrektywy przyslaniaja flagi kompilacji

//#define ITER 100
//#define COUNT_WOLVES 10
//#define GENERATOR 1

//#define MAPA 0 //od 0 do 4

//#define MIN_X -2
//#define MAX_X 2
//#define MIN_Y -2
//#define MAX_Y 2

//#define DEBUG //drukuje na stdout dodatkowe informacje
//########################## KONIEC PARAMETROW ###################################


//domyslne ustawienia parametrow jesli nie zostaly wczesniej zdefiniowane
#ifndef COUNT_WOLVES
    #define COUNT_WOLVES 10
#endif

#ifndef GENERATOR
    #define GENERATOR time(NULL)
#endif

#ifndef MAPA
    #define MAPA 0
#endif

#ifndef MIN_X
    #define MIN_X -2
#endif

#ifndef MAX_X
    #define MAX_X 2
#endif

#ifndef MIN_Y
    #define MIN_Y -2
#endif

#ifndef MAX_Y
    #define MAX_Y 2
#endif