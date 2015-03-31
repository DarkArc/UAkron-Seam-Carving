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

#include <stdexcept>

/** Construct a new FlexGrid object.

    @param width
    The width of the grid

    @param height
    The height of the grid
 */
template <typename T>
  FlexGrid<T>::FlexGrid(unsigned int width, unsigned int height) :
    width(width), height(height) {
    grid.resize(width);
    for (auto& entry : grid) {
      entry.resize(height);
    }
  }

/** Retrieves the value at the given 2D quards.

    Takes the 2D quards, then calculates the grid index,
    to retrive the element from the internal 1D representation.

    @param x
    The x quardinate.

    @param y
    The y quardinate.

    @return the given value held by the grid at the
    calulated 1D index.
 */
template <typename T>
  T FlexGrid<T>::getValAt(unsigned int x, unsigned int y) const {
    if (x >= width || x < 0) {
      throw std::runtime_error("Given x is not within bounds!");
    }
    if (y >= height || y < 0) {
      throw std::runtime_error("Given y is not within bounds!");
    }

    return grid[x][y];
  }

/** Sets the value at the given 2D quards.

    Takes the 2D quards, then calculates the grid index.
    Then sets the value at the calculated index to be equal
    to the provided new value.

    @param x
    The x quardinate.

    @param y
    The y quardinate.

    @param val
    The new value to be set at the specified index.
 */
template <typename T>
  void FlexGrid<T>::setValAt(const unsigned int& x, const unsigned int& y, const T& val) {
    if (x >= width || x < 0) {
      throw std::runtime_error("Given x is not within bounds!");
    }
    if (y >= height || y < 0) {
      throw std::runtime_error("Given y is not within bounds!");
    }

    grid[x][y] = val;
  }

template <typename T>
  void FlexGrid<T>::setWidth(const unsigned int& width) {
    this->width = width;
    grid.resize(width);
  }

template <typename T>
  void FlexGrid<T>::setHeight(const unsigned int& height) {
    this->height = height;
    for (auto& entry : grid) {
      entry.resize(height);
    }
  }

/** Gets the length of the grid.

    Gets the length of the interal 1D representation
    of the grid.

    @returns the length of the 1D representation.
 */
template <typename T>
  unsigned int FlexGrid<T>::len() const {
    return width * height;
  }

/** Gets the width of the grids rows.

    Gets the width of the grid row, effectively providing
    the column count.

    @returns the width of grid.
 */
template <typename T>
  unsigned int FlexGrid<T>::getWidth() const {
    return width;
  }

/** Gets the length of the grids columns.

    Gets the length of the grid columns, effectively providing
    the row count.

    @returns the length of grid.
 */
template <typename T>
  unsigned int FlexGrid<T>::getHeight() const {
    return height;
  }
