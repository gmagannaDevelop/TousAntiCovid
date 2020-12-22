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
        /* ^ using this is much simpler than removing
            playing around with linked lists
        */
        int viral_charge;
        Coordinate pos;
        /* This should be between [0,7] */
        int direction;
        /* THIS FLAG DISCRIMINATES 
           PEOPLE VS DOCTORS*/
        int healing;
    } Person;


    typedef struct Cases
    {
        int viral_charge;
        int danger;
        int p;
        /* Save the person's index instead */
        /* Person *p;
        */
    } Case;

    
    /* Function protoypes : */
    void init_person_at(Person *p, int x, int y, int d);
    
    void init_doctor_at(Person *p, int x, int y, int d);

#endif