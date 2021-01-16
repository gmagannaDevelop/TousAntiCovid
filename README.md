# TousAntiCovid![](images/c-vid-logo.png)

### Faculté des Sciences de l'Université Paris-Saclay

### Master en BioInformatique

**FR** : Projet Final pour l'UE Mise à Niveau en Informatique I

**EN** : Final project for *Mise à Niveau en Informatique I* course.

### Étudiants (Students)

* Gustavo Magaña López
* Alexandre Abhay



### Preview

![](images/simu.gif)



### Compile and run

1. Clone the repo to your machine

2. Install the dependencies

3. Compile the program
   
   ```bash
   cd src
   make clean
   make
   ```

4. Run the program (see bin/README.md for more in-depth explanation)
   
   ```bash
   cd ../bin # if you're still within src/
   cd bin    # if you are at the repo's root
   ./viral-simulation    
   ```

### Dependencies

This project leverages [GSL (GNU Scientific Library)](https://www.gnu.org/software/gsl/) and [SDL (Simple Directmedia Layer)](https://www.libsdl.org/). These are used in order to get beter quality random numbers than those directly available in plain C, and create an interface to visualise the simulation's evolution, respectively. 

In order to compile and run the C code present in this repository, you must install the dependencies beforehand (if you have not already installed them).

* GSL
  
  Under Ubuntu / Pop!\_OS / any other Debian-based distribution :
  
  * Essential (to compile and run) :
    
    ```bash
    sudo apt install gsl-bin libgsl0-dev
    ```
  
  * Optional (recommended) :
    
    ```bash
    sudo apt install gsl-doc-info gsl-doc-pdf gsl-ref-html gsl-ref-psdoc
    ```
  
  Under macOS (via [Homebrew](https://formulae.brew.sh/formula/gsl))
  
  * Essential (to compile and run) :
    
    ```bash
    brew install gsl
    ```

* SDL
  
  We have decided to use libsdl-1, as we already have an implementation using it. 
  
  Under Ubuntu / Pop!\_OS / any other Debian-based distribution :
  
  * Essential (to compile and run) :
    
    ```bash
    sudo apt-get install libsdl-image1.2-dev
    sudo apt install libsdl1.2-dev
    ```
    
    Under macOS (via [Homebrew](https://formulae.brew.sh/formula/gsl))
  
  * Essential (to compile and run) :
    
    ```bash
    brew install sdl
    ```

### Project Structure

The main project source is located at `src`. The directory `bin` stores the executable (it is needed as the recipe in the makefile expects it to exist.

```bash
.
├── bin
├── covid-simu
├── experiments
├── ideas
├── images
├── metrics
├── reference
│   ├── 09_singly_linked_list
│   │   ├── bin
│   │   └── src
│   ├── 13_binary_tree
│   │   ├── bin
│   │   │   └── removing_nodes_example
│   │   └── src
│   ├── 21_SDL_ppm_graphics
│   │   ├── bin
│   │   └── src
│   └── 26_Mersenne_Twister
│       ├── bin
│       └── src
├── src
└── stray
    ├── graphics
    ├── original
    └── random
```

Explanation of the files found in `src/` directory.

```bash
src
├── aux_math.c  # Auxiliary math operations (float comparisons)
├── aux_math.h
├── constants.h # Miscellaneous simulation constants
├── datastructures.h # Person, Coordinate, and other custom datatypes
├── dev_random.c # Functions to initialise gsl's rng.
├── dev_random.h
├── display.c # Functions to display on console (legacy)
├── display.h
├── dynamics.c # Physical displacement rules and utility functions
├── dynamics.h 
├── libraries.h # All of our headers and needed libraries.
├── main_program.c # MAIN PROGRAM
├── Makefile 
├── mersenne_twister.c # High-quality pseudo random number generation
├── mersenne_twister.h
├── parsing.c # Get parameters from the command line
├── parsing.h
├── probabilities.c  # Utility functions for random experiments
├── probabilities.h # Probabilities which are internal params
├── SDL_datastructure.h  # datastructures needed for graphics
├── simulation.c # instantiation, iteration and epochs
├── simulation.h 
├── singly_linked_list.c # Sinlgy linked list implementation
├── singly_linked_list.h
├── visualization.c # Functions used to render the simulation
└── visualization.h # via SDL
```

### Acknowledgements

We would like to thank Ouriel Grynszpan and Stéphanie Chevalier for their instruction and support during this course.

We would also like to thank Marco Heinen for his explanations and example
codes on the topics of _pseudo-random number generators_ and 
_data structures_ during his 2017 Numerical Methods Course at Universidad de Guanajuato, Mexico. This repository relies on some of the 
beforementioned codes to build the simulation.

We would also like to thank [Mathis Delehouzée](https://github.com/mathisdelehouzee) for his help finding the matrix indexation bug.
