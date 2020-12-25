#ifndef __aw092h__SIMULATION_H
#define __aw092h__SIMULATION_H

    #include "datastructures.h"
    #include "singly_linked_list.h"

    int person_death(
        Person *p, struct singly_linked_list **persons,  
        Case **table,
        int n, int m
    );

    int move_person(
        gsl_rng **randgen, Person *p, Case **p_table, int n, int m
    );

    int global_update(
        gsl_rng **randgen,
        struct singly_linked_list **people, 
        struct singly_linked_list **doctors,
        Case **table
    );

#endif