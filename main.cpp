// Copyright (C) 2015 Wyatt Childers
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>
#include <regex>
#include <string>

#include "SeamCarver.hpp"
#include "ImageLoader.hpp"
#include "Util/FlexGrid.hpp"

int main(int argc, char* argv[]) {
  // Check that the proper amount of arguments have
  // been supplied
  if (argc == 4) {
    // Remove the file name from the file extension
    std::string file = std::regex_replace(argv[1], std::regex("(\\.pgm)"), "");
    // Get the number of vertical seams to remove
    unsigned int vert = std::atoi(argv[2]);
    // Get the number of horizontal seams to remove
    unsigned int horiz = std::atoi(argv[3]);

    // Establish an ImageLoader, then load the file, with the removed
    // ".pgm" extension readded
    ImageLoader loader;
    loader.loadFile(file + ".pgm");

    // Perform the removal of the vertical seams, followed by the
    // removal of the horizontal seams
    FlexGrid<int> p = seamCarve(loader.getGrid(), CarvingMode::VERTICAL, vert);
    FlexGrid<int> f = seamCarve(p, CarvingMode::HORIZONTAL, horiz);

    // Update the pixel grid stored in the ImageLoader,
    // and export the file, as being processed
    loader.setGrid(f);
    loader.exportFile(file + "_processed.pgm");
  } else {
    throw std::runtime_error("Illegal number of arguments!");
  }
  return 0;
}
