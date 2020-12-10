M. Heinen, October 2017
Numerical Methods course
Universidad de Guanajuato
marco@marco-heinen.de



INSTALLATION DEPENDS ON:
libsdl1.2-dev



This program that runs a little social dynamics simulation
of N persons in an 'unrequited love' situation:

Person 1 is in love with person 2 and walks towards him/her,
person 2 is in love with person 3 and walks towards him/her,
person 3 is in love with person 4 and walks towards him/her,
.
.
.
person N-1 is in love with person N and walks towards him/her,
person N is in love with person 1 and walks towards him/her.

Graphical output uses two methods:
----------------------------------
First, there is an on-the-fly graphics output using the free
Simple DirectMedia Layer (SDL) library
http://www.libsdl.org/

Second, the program writes uncompressed ppm pictures and
converts them on-the-fly into compressed gif pictures via
a system call. The gif pictures can later be arranged to
give a movie, for example with the OpenShot editor
http://www.openshot.org/




Instructions:
=============

1.) Compiling and running:
--------------------------

cd src

make

cd ../bin/

./SDL_ppm_graphics


2.) Cleaning up:
----------------

cd src

make clean
