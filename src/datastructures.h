#ifndef __23lknf92_PERSON_DATASTRUCTURE_H
#define __23lknf92_PERSON_DATASTRUCTURE_H

    #define N_DIM 2


    typedef struct Coordinates
    {
        int x;
        int y;
    } Coordinate;

    typedef struct Persons
    {
        int symptomatic;
        int viral_charge;
        Coordinate pos;
        // This should be between [0,7] 
        int direction;
        // THIS FLAG DISCRIMINATES PEOPLE VS DOCTORS
        int healing;
        int being_healed;
        struct Persons *p;
    } Person;


    typedef struct Cases
    {
        int viral_charge;
        int danger;
        Person *p;
    } Case;

    

#endif
