
#include "libraries.h"

int p(int a, int n)
{/* One step forward, with periodic boundaries */
    return (a + 1) % n;
}

int m(int a, int n)
{/* One step backward, with periodic boundaries */
    return (a + n - 1) % n;
}

void move_N(Coordinate *pos, int N, int M)
{
    pos->y = m(pos->y, N);
}


void move_S(Coordinate *pos, int N, int M)
{
    pos->y = p(pos->y, N);
}

void move_E(Coordinate *pos, int N, int M)
{
    pos->x = p(pos->x, M);
}

void move_W(Coordinate *pos, int N, int M)
{
    pos->x = m(pos->x, M);
}

void move_NE(Coordinate *pos, int N, int M)
{
    pos->x = p(pos->x, M);
    pos->y = m(pos->y, N);
}

void move_NW(Coordinate *pos, int N, int M)
{
    pos->x = m(pos->x, M);
    pos->y = m(pos->y, N);
}

void move_SE(Coordinate *pos, int N, int M)
{
    pos->x = p(pos->x, M);
    pos->y = p(pos->y, N);
}

void move_SW(Coordinate *pos, int N, int M)
{
    pos->x = m(pos->x, M);
    pos->y = p(pos->y, N);
}

void (*directions[])(Coordinate *pos, int, int) = {
    /* This "directions" array will stablish an
       order for the directions, and more importantly,
       it will allow "counter-gradient" movements.
            {NW,  N, NW,  O, SE, S, SW, E}
            {-1, -2, -3, -4, 1,  2,  3, 4} 
            { 0,  1,  2,  3, 4,  5,  6, 7}
       as one can see here, oposite directions 
       have the same value but different sign.
       With this layout we can easily 
       "inverse the direction", i.e. 
       "move against the gradient".
     */
    move_NW, move_N, move_NE, move_W,
    move_SE, move_S, move_SW, move_E
};

int oposite_direction(Person *p)
{
    if ((p->direction >= 0) && (p->direction < 4)){
        return p->direction + 4;
    }
    else if ((p->direction >= 4) && (p->direction <= 7)){
        return p->direction - 4;
    } 
    else {
        printf("ERROR : Invalid direction %d in Person %p\n\n", p->direction, (void *)p);
        exit(EXIT_FAILURE);
    }
}