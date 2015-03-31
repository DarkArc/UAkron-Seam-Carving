
#include <iostream>

#include "SeamCarver.hpp"

int main(int argc, char* argv[]) {
  Grid<int> k(10, 10);
  Grid<int> p = seamCarve(k, 4, 4);
  return 0;
}
