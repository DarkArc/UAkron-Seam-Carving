
#include <iostream>

#include "Util/FlexGrid.hpp"
#include "SeamCarver.hpp"
#include "ImageLoader.hpp"

int main(int argc, char* argv[]) {
  ImageLoader loader;
  loader.loadFile("test.pgm");
  FlexGrid<int> p = seamCarve(loader.getGrid(), CarvingMode::VERTICAL, 1);
  FlexGrid<int> f = seamCarve(p, CarvingMode::HORIZONTAL, 5);
  loader.setGrid(f);
  loader.exportFile("test-res.pgm");
  std::cout << "exited" << std::endl;
  return 0;
}
