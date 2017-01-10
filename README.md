# PORR - Optymalizator szarego wilka (GWO)

### Ogólna koncepcja:
Puszczamy na jakiś teren (funkcję matematyczną 3 zmiennych w stylu h=sin(xy)) *n* wilków,
które szukają najwyższego punktu terenu przez zadaną liczbę iteracji, bądź do osiągnięcia wysokości bliskiej
ekstremum danej mapy.

Mamy 5 [map](maps.h) - będą opisane w sprawozdaniu

Wilk *alpha* to najlepszy w aktualnej iteracji (najwyżej położony)

### Sprawozdanie
[google doc](https://docs.google.com/document/d/1tbeOzdhqqln_eS6P_Kz4HFn-MPOnzkvJtEY-77_txPc/edit?usp=sharing)

### Kompilacja i uruchomienie
w głównym folderze projektu należy wydać polecenie:

    gcc *.c -fopenmp -lm
    ./a.out
    
albo:

    cmake .
    make
    ./PORR
    
### Parametry
sterujemy parametrami poprzez ustawienie dyrektyw preprocesora:
  
**ITER** - liczba iteracji algorytmu (domyślnie leci do osiągnięcia 
zadanej wartości mapy (blisko ekstremum) - do ustawienia *Map.satisfied_value*)  
**COUNT_WOLVES** - liczba wilków latających po terenie (domyślnie 10)  
**GENERATOR** - ziarno generatora losowego (domyślnie losowy)  
**MAPA** - wybrana mapa po której latają wilki 0-4 (domyślnie 0)

zakres (x,y) do początkowego rozlosowania wilków na mapie:  
**MIN_X** (domyślnie -2)    
**MAX_X** (domyślnie 2)  
**MIN_Y** (domyślnie -2)  
**MAX_Y** (domyślnie 2)  

**DEBUG** drukuje na stdout dodatkowe informacje o przebiegu programu (domyślnie *false*)  
**TIME** pomiar czasu wykonania algorytmu (domyślnie *false*)  
**PARALLEL** uruchamia program równolegle w trybie z pamięcią wspólną (domyślnie *false*)  

żeby ustawić parametry, do wyboru mamy 2 warianty:
- przez flagi kompilacji, przykład:
    
        gcc *.c -lm -DITER=100 -DGENERATOR=1
- odkomentowanie wybranego #define z pliku [directives.h](directives.h) (przysłania metodę "przez flagi")

### Zadania do wykonania:
- wersja algorytmu zrównoleglona  
zaproponowałem w kodzie miejsca gdzie dopisać dyrektywy OpenMP 
ciągiem mocno zachęcających znaków <<<<<<<<<  ZROWNOLEGL MNIE  >>>>>>>>>

- pomiary czasu dla sekwencyjnej i zrównoleglonej  
myśle, że trzeba to porobić dla wszystkich map i dla każdej dwa warianty:
    - do np 1000 iteracji żeby tam się coś zdążyło przemielić
    - do osiągnięcia dla danej mapy wartości *satisfied_value* -> trzeba też
     ją wystarczająco wysoką dobrać żeby po 5 iteracjach się nie skończyło  
      Należy uśrednić wartości z kilku pomiarów - w różnym tempie algorytm może je osiągać.
 
- statystyki, wykresy  
trzeba ładnie do sprawozdania to wszystko dołączyć i opisać.
