#ifndef __DYNAMICS_H
#define __DYNAMICS_H

    #include "datastructures.h"

    /*
        Declaration of functions which are screen-aware.
        Given that the origin of the screen's coordinate system,
        the North is on -y direction,
        the South is on +y direction,
        the East is on +x direction 
    */


    /* One step forward, with periodic boundaries */
    int p(int a, int n);

    /* One step backward, with periodic boundaries */
    int m(int a, int n);

    void move_N(Coordinate *pos, int N, int M);

    void move_S(Coordinate *pos, int N, int M);
    
    void move_E(Coordinate *pos, int N, int M);
    
    void move_W(Coordinate *pos, int N, int M);
    
    void move_NE(Coordinate *pos, int N, int M);
    
    void move_NW(Coordinate *pos, int N, int M);
    
    void move_SE(Coordinate *pos, int N, int M);
    
    void move_SW(Coordinate *pos, int N, int M);

#endif