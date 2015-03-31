
#include <iostream>

#include "Util/FlexGrid.hpp"
#include "SeamCarver.hpp"

int main(int argc, char* argv[]) {
  FlexGrid<int> k(10, 10);
  FlexGrid<int> p = seamCarve(k, CarvingMode::VERTICAL, 4);
  // FlexGrid<int> f = seamCarve(p, CarvingMode::VERTICAL, 4);
  return 0;
}
