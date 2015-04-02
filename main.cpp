
#include <iostream>
#include <regex>
#include <string>

#include "SeamCarver.hpp"
#include "ImageLoader.hpp"
#include "Util/FlexGrid.hpp"

int main(int argc, char* argv[]) {
  if (argc == 4) {
    const std::regex e("(\\.pgm)");

    // Trigger a simulation based on arguments
    std::string file = std::regex_replace(argv[1], e, "");
    unsigned int vert = std::atoi(argv[2]);
    unsigned int horiz = std::atoi(argv[3]);

    ImageLoader loader;
    loader.loadFile(file + ".pgm");
    FlexGrid<int> p = seamCarve(loader.getGrid(), CarvingMode::VERTICAL, vert);
    FlexGrid<int> f = seamCarve(p, CarvingMode::HORIZONTAL, horiz);
    loader.setGrid(f);
    loader.exportFile(file + "_processed.pgm");
  } else {
    throw std::runtime_error("Illegal number of arguments!");
  }
  return 0;
}
