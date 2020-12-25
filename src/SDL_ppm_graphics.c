
#include "libraries.h"

int update_positions(float *persons, int N)
{
/*
 * For n = 0 ... N-1
 * Every person n makes a unit-length step towards the person n+1
 * (except person N-1, who steps towards person 0).
 */
float buffer[2], connector[2];
float distance;
int n;

buffer[0] = persons[0];
buffer[1] = persons[1];

/* All persons expect for the last one (N-1): */
for(n = 0; n < N-1; n++)
  {
  connector[0] = persons[2*(n+1)  ] - persons[2*n  ];
  connector[1] = persons[2*(n+1)+1] - persons[2*n+1];

  distance  = connector[0] * connector[0];
  distance += connector[1] * connector[1];
  distance  = sqrt(distance);

  if(distance > SMALLDISTANCE)
    {
    connector[0] /= distance;
    connector[1] /= distance;
    }

  persons[2*n  ] += connector[0];
  persons[2*n+1] += connector[1];
  }

/* Last person (number N-1): */
connector[0] = buffer[0] - persons[2*(N-1)  ];
connector[1] = buffer[1] - persons[2*(N-1)+1];

distance  = connector[0] * connector[0];
distance += connector[1] * connector[1];
distance  = sqrt(distance);

if(distance > SMALLDISTANCE)
  {
  connector[0] /= distance;
  connector[1] /= distance;
  }

persons[2*(N-1)  ] += connector[0];
persons[2*(N-1)+1] += connector[1];

return(n);
}

