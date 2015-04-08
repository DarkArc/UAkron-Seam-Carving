#include "ImageLoader.hpp"

#include <algorithm>
#include <fstream>
#include <regex>
#include <ctime>
#include <cassert>
#include <stdexcept>

ImageLoader::ImageLoader() : iData(0, 0) { }

FlexGrid<int> ImageLoader::getGrid() const {
  return iData;
}

void ImageLoader::setGrid(const FlexGrid<int>& iData) {
  this->iData = iData;
  this->colCount = iData.getWidth();
  this->rowCount = iData.getHeight();
}

bool ImageLoader::loadFile(const std::string& fileName) {
  std::ifstream ifs(fileName);
  return parseHeader(ifs) && parseBody(ifs);
}

bool ImageLoader::isComment(const std::string& str) {
  return std::regex_match(str, std::regex("(#)(.*)"));
}

bool ImageLoader::parseHeader(std::ifstream& ifs) {
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
  return true;
}

bool ImageLoader::parseBody(std::ifstream& ifs) {

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
  return true;
}

bool ImageLoader::exportFile(const std::string& fileName) const {
  std::ofstream ofs(fileName);
  return exportHeader(ofs, fileName) && exportBody(ofs);
}

bool ImageLoader::exportHeader(std::ofstream& ofs, const std::string& nFileName) const {
  ofs << header << std::endl;
  ofs << "# " << nFileName << std::endl;
  ofs << colCount << ' ' << rowCount << std::endl;
  ofs << greyScale << std::endl;
  return true;
}

bool ImageLoader::exportBody(std::ofstream& ofs) const {
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
  return true;
}
