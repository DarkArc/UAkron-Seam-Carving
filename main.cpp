
#include <iostream>

#include "Util/FlexGrid.hpp"
#include "SeamCarver.hpp"

int main(int argc, char* argv[]) {
  FlexGrid<int> k(10, 10);
  FlexGrid<int> p = seamCarve(k, 4, 4);
  return 0;
}
