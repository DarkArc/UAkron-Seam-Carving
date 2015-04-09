Implementation
  The program is composed of 3 classes, and 4 header files, as well as a main:
    3 classes:
      * FlexGrid    - A wrapper for a 2D vector array which allows it to be easily
                      managed and resized.
      * Optional    - A data structure which stores an value which may or may not exists
                      used to simply some variable calculations in the
                      seam carving functions.
      * ImageLoader - Provides the tools for loading and saving PGM files for
                      the seam carving algorithm.
    General headers:
      * SeamCarver  - Functions for performing the seam carving algorithm
  The main:
    Coordinates interaction between the different elements, and handles user input

Compilation
  Requirements:
    * C++11 (Definitely works with GCC 4.9.2, may work with older versions)
    * CMake 2.8+

  Compilation Steps:
    1) Go to the project directory via command line
    2) mkdir build
    3) cd build/
    4) cmake ..
    5) make

Usage
  Steps:
    1) ./SeamCarving (command arguments here)
