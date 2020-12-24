
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



int oposite_direction(Person *p)
{/* Returns the value of the opposite direction 
    with no side effects.
*/
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