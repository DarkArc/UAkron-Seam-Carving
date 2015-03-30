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

#ifndef GRID_HPP
#define GRID_HPP

#include <vector>

template <typename T>
class Grid {
  const unsigned int width;
  const unsigned int height;
  std::vector<T> grid;
public:
  Grid(unsigned int, unsigned int);

  unsigned int calcIndex(unsigned int, unsigned int) const;

  T getValAt(unsigned int, unsigned int) const;
  void setValAt(const unsigned int&, const T&);
  void setValAt(const unsigned int&, const unsigned int&, const T&);

  T operator [] (const unsigned int&) const;

  unsigned int len() const;
  unsigned int getWidth() const;
  unsigned int getHeight() const;
};

#include "Grid.ipp"

#endif
