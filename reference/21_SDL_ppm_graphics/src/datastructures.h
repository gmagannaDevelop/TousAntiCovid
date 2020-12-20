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
        int alive;
        int viral_charge;
        Coordinate pos;
        int direction; /* This should be between [0,7] */
    } Person;

    typedef struct Doctors
    {
      Person p;

    } Doctor;

    typedef struct Cases
    {
        int viral_charge;
        Person *p;
    } Case;

    

#endif