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

#ifndef IMAGELOADER_HPP
#define IMAGELOADER_HPP

#include <string>

#include "Util/FlexGrid.hpp"

class ImageLoader {
    std::string header;
    int greyScale;

    int rowCount;
    int colCount;
    FlexGrid<int> iData;
public:
    ImageLoader();

    FlexGrid<int> getGrid() const;
    void setGrid(const FlexGrid<int>&);

    void loadFile(const std::string&);
    void exportFile(const std::string&) const;
private:
    bool isComment(const std::string&);

    void parseHeader(std::ifstream&);
    void parseBody(std::ifstream&);

    void exportHeader(std::ofstream&, const std::string&) const;
    void exportBody(std::ofstream&) const;
};
#endif
