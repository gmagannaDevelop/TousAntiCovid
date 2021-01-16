#ifndef __23lknf92_PERSON_DATASTRUCTURE_H
#define __23lknf92_PERSON_DATASTRUCTURE_H

    #define N_DIM 2


    /* 
        On the screen, the origin of the coordinate system is not the bottom left 
        corner, but the top left one. This means that the horizontal axis remains 
        unchanged whilst the vertical one is inverted. 
        So going NORTH or UP means going down the -y direction.
    */
    typedef struct Coordinates
    { // Two-dimension position markers.
        int x;  
        int y;
    } Coordinate;

    /*
        We decided to define only one data structure, common to both
        "bonhomme lambda" and "medecin". A priori, there are not significant 
        structural differences to justify having two different data types.
        Reading the explanation of the struct will give you better insight of how
        this works.
    */
    typedef struct Persons
    {  
        // boolean, 1 if true, 0 if false.
        int symptomatic; 
        // When a person is infected, their viral charge
        // is set to the mean infection length. 
        // Each "day" this viral charge is reduced by one
        // so that asymptomatic patients will be automatically healed.
        int viral_charge; 
        // The coordinate structure is needed to know the person's
        // position on the simulation grid.
        Coordinate pos;
        // In simulation.c we created a "directions" vector, 
        // enumerating all possible directions, which are 8.
        // therefore : direction is in the interval [0, 7]  
        int direction;
        // Does the person have a healing capacity ?
        // If so, they are a doctor. The "healing" flag
        // is the only difference between a person and a doctor.
        int healing;
        // In order to turn off the "danger gradient" that surrounds
        // a person, we must have a boolean flag, which is the following :
        int being_healed;
        // A person might need a pointer to another person.
        // This helps the doctor know not only that he is healing a 
        // patient, but which patient. 
        // The reference is symmetrical : patient <-> doctor
        struct Persons *p;
    } Person;


    /*
        The basic bulding block of a the simulation grid.
        Each position is a "small case" which can contain
        at most one person. Checking if this reference is
        NULL or not helps the simulation functions determine
        whether a person's move is feasible or not.
        It also contains the "danger gradient" indicating 
        that there is a sick person nearby.
    */
    typedef struct Cases
    {
        // viral charge of the place
        int viral_charge;
        // danger marker
        // high ~ 100 or greater near a sick person
        // low  ~ 1 on the 2-vicinity
        // nil  = 0 elsewhere 
        int danger;
        // Reference to a person (or a doctor)
        Person *p;
    } Case;

    

#endif
