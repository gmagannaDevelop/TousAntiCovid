#ifndef __aw092h__SIMULATION_H
#define __aw092h__SIMULATION_H

    #include "datastructures.h"
    #include "singly_linked_list.h"

    #define DUMMY_REJECT_MAX -100
    #define DUMMY_REJECT_MIN 10000

    // Function protoypes :

    void infection (
        gsl_rng **randgen, Person *p, Case **p_table, 
        Case *next, int n, int m, int *infection_count
    );

    int find_and_link_patient(Person *d, Case **p_table, int n, int m);

    void init_person_at(Person *p, int x, int y, int d);

    void init_doctor_at(Person *p, int x, int y, int d);

    void add_danger(Person *p, Case **p_table, int n, int m);

    void rm_danger(Person *p, Case **p_table, int n, int m);

    int max_danger_direction(Person *p, Case **p_table, int n, int m);

    int min_danger_direction(Person *p, Case **p_table, int n, int m);

    int person_death(
        Person *p, struct singly_linked_list **persons,
        Case **table,
        int n, int m
    );

    int move_person(
        gsl_rng **randgen, Person *p,
        Case **p_table, int n, int m,
        int *infection_count
    );

    int move_doctor(
        gsl_rng **randgen, Person *p,
        Case **p_table, int n, int m,
        int *infection_count
    );

    int global_update(
        gsl_rng **randgen,
        struct singly_linked_list **people,
        struct singly_linked_list **doctors,
        Case **table,
        int N, int M,
        Epoch *day
    );

#endif
