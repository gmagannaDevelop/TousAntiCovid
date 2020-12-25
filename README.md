# TousAntiCovid![](images/c-vid-logo.png)

### Faculté des Sciences de l'Université Paris-Saclay

### Master en BioInformatique

**FR** : Projet Final pour l'UE Mise à Niveau en Informatique I

**EN** : Final project for *Mise à Niveau en Informatique I* course.

### Étudiants

* Gustavo Magaña López
* Alexandre Abhay

### Preview

This is a preview of the type of graphic output we expect to be able to produce (This is currently a preview of Conway's Game of Life, coded by Marco Heinen)  :

![](images/simgif.gif)

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

### Acknowledgements

We would like to thank Ouriel Grynszpan and Stéphanie Chevalier for their instruction and support during this course.

We would also like to thank Marco Heinen for his explanations and example
codes on the topics of _pseudo-random number generators_ and 
_data structures_ during his 2017 Numerical Methods Course at Universidad de Guanajuato, Mexico. This repository relies on some of the 
beforementioned codes to build the simulation.

We would also like to thank [Mathis Delehouzée](https://github.com/mathisdelehouzee) for his help finding the matrix indexation bug.
