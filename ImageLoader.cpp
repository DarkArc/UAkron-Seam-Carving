#include "ImageLoader.hpp"

#include <algorithm>
#include <fstream>
#include <regex>
#include <ctime>
#include <cassert>
#include <stdexcept>

/** Construct a new ImageLoader object.

    Constructs a new ImageLoader object, which
    no relationship to any image.
 */
ImageLoader::ImageLoader() : iData(0, 0) { }

/** Retrives the stored pixel grid.

    Retrieves the stored pixel grid,
    or an empty pixel grid if there is not
    an established relationship to any image.

    @returns the stored pixel grid.
 */
FlexGrid<int> ImageLoader::getGrid() const {
  return iData;
}

/** Replaces the stored pixel grid.

    Sets the stored pixel grid, to the provided grid,
    and updates all related demensional measures.

    @param iData
    The pixel grid which shall replace the current
    pixel grid.
 */
void ImageLoader::setGrid(const FlexGrid<int>& iData) {
  this->iData = iData;
  this->colCount = iData.getWidth();
  this->rowCount = iData.getHeight();
}

/** Loads a PGM image file.

    Processes the PGM file at the provided path,
    and updates the ImageLoader's data to match
    that of the new image, establishing a relationship
    between the ImageLoader, and the image.

    If the image loading process fails, and throws
    an exception, the ImageLoader's current
    state -- and therefor data -- must be considered
    invalid.

    @param path
    The PGM file which shall be processed.
 */
void ImageLoader::loadFile(const std::string& path) {
  std::ifstream ifs(path);
  parseHeader(ifs);
  parseBody(ifs);
}

/** Checks to see if a line is a comment line.

    Uses regex to check to see if a given string
    matches the format of a PGM comment.

    @param str
    The string to check

    @returns true if the provided string indeed
    matches the formate of a PGM comment line.
 */
bool ImageLoader::isComment(const std::string& str) {
  return std::regex_match(str, std::regex("(#)(.*)"));
}

/** Parses the header portion of a PGM file.

    Given an input file stream, this function
    removes lines from the file stream until all header
    information is gathered, or the file is deemed
    invalid. Header information will be updated
    in the ImageLoader as it is obtained.

    If the header processing fails, and throws
    an exception, the ImageLoader's current
    state -- and therefor data -- must be considered
    invalid.

    @param ifs
    The input file stream to pull lines from.
 */
void ImageLoader::parseHeader(std::ifstream& ifs) {
  // Regex constants
  const std::regex_token_iterator<std::string::iterator> rend;

  // Get PGM Header
  do {
    std::getline(ifs, header);
  } while (isComment(header));

  std::string temp;

  // Get size line
  do {
    std::getline(ifs, temp);
  } while (isComment(temp));

  std::regex_token_iterator<std::string::iterator> sizeMatch(
    temp.begin(), temp.end(), std::regex("[0-9]+")
  );

  if (std::distance(sizeMatch, rend) != 2) {
    throw std::runtime_error("PGM file deminisions invalid!");
  }

  colCount = std::atoi((*(sizeMatch++)).str().c_str());
  rowCount = std::atoi((*(sizeMatch++)).str().c_str());

  // Get grey scale line
  do {
    std::getline(ifs, temp);
  } while (isComment(temp));

  std::regex_token_iterator<std::string::iterator> greyScaleMatch(
    temp.begin(), temp.end(), std::regex("[0-9]+")
  );

  if (std::distance(greyScaleMatch, rend) != 1) {
    throw std::runtime_error("PGM file grey scale value invalid!");
  }

  // Grey scale
  greyScale = std::atoi((*(greyScaleMatch++)).str().c_str());
}

/** Parses the body portion of a PGM file.

    Given an input file stream, this function
    removes lines from the file stream until all cells
    of the pixel grid have been filled,
    or the file is deemed invalid.

    If the body processing fails, and throws
    an exception, the ImageLoader's current
    state -- and therefor data -- must be considered
    invalid.

    @param ifs
    The input file stream to pull lines from.
 */
void ImageLoader::parseBody(std::ifstream& ifs) {

  const std::regex_token_iterator<std::string::iterator> rend;

  iData = FlexGrid<int>(colCount, rowCount);

  std::vector<int> rowData;

  while (!ifs.eof()) {
    std::string line;
    std::getline(ifs, line);

    if (isComment(line)) continue;

    std::regex_token_iterator<std::string::iterator> match(
      line.begin(), line.end(), std::regex("[0-9]+")
    );

    while (match != rend) {
      rowData.push_back(std::atoi((*(match++)).str().c_str()));
    }
  }

  try {
    for (int row = 0; row < rowCount; ++row) {
      for (int col = 0; col < colCount; ++col) {
        iData.setValAt(col, row, rowData.at((row * colCount) + col));
      }
    }
  } catch (...) {
    throw std::runtime_error("Invalid PGM data (not enough data to fill all columns and rows)!");
  }
}

/** Exports the current stored pixel grid as a PGM file.

    Given an output file path, takes the current
    stored pixel grid, and exports it as a PGM file.

    @param path
    The file which shall be exported to.
 */
void ImageLoader::exportFile(const std::string& path) const {
  std::ofstream ofs(path);
  exportHeader(ofs, path);
  exportBody(ofs);
}

/** Exports the header of the pixel grid into a PGM file.

    Exports the PGM file header data using the
    provided ofs, and resulting new file name.

    @param ofs
    The output file stream to add data to.

    @param nFileName
    The name of the new file.
 */
void ImageLoader::exportHeader(std::ofstream& ofs, const std::string& nFileName) const {
  ofs << header << std::endl;
  ofs << "# " << nFileName << std::endl;
  ofs << colCount << ' ' << rowCount << std::endl;
  ofs << greyScale << std::endl;
}

/** Exports the body of the pixel grid into a PGM file.

    Exports the PGM file pixel data using the
    provided ofs.

    @param ofs
    The output file stream to add data to.
 */
void ImageLoader::exportBody(std::ofstream& ofs) const {
  int entries = 0;
  for (int line = 0; line < rowCount; ++line) {
    for (int col = 0; col < colCount; ++col) {
      ofs << iData.getValAt(col, line);
      if (++entries == 15) {
        entries = 0;
        ofs << " " << std::endl;
        continue;
      }
      ofs << ' ';
    }
  }
}
