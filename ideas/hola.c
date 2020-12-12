


int main()
{
  population **Person;

  population = (Person *)malloc( N * sizeof(*Person) );
  if (population == NULL){
    printf("wesh ton ordi n'a pas voulu me donner de la memoire pour ta simulation\n");
  }

  for (i=0; i<N; i++){
    population[i] // pointeur vers la i-ème personne
    population[i]->soignant = true; 
                            // le bool, on utilise -> au lieu d'un
                            // point `population[i].soignant`
  }


}
