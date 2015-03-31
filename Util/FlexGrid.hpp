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

#ifndef FLEXGRID_HPP
#define FLEXGRID_HPP

#include <vector>

template <typename T>
class FlexGrid {
  unsigned int width;
  unsigned int height;
  std::vector<std::vector<T>> grid;
public:
  FlexGrid(unsigned int, unsigned int);

  T getValAt(unsigned int, unsigned int) const;
  void setValAt(const unsigned int&, const unsigned int&, const T&);

  void setWidth(const unsigned int&);
  void setHeight(const unsigned int&);

  unsigned int len() const;
  unsigned int getWidth() const;
  unsigned int getHeight() const;
};

#include "FlexGrid.ipp"

#endif
