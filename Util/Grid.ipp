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

/** Construct a new Grid object.

    @param width
    The width of the grid

    @param height
    The height of the grid
 */
template <typename T>
  Grid<T>::Grid(unsigned int width, unsigned int height) :
    width(width), height(height),
    grid(std::vector<T>(width * height)) { }

/** Calculates the 1D index given 2D quards.

    @param x
    The x quardinate.

    @param y
    The y quardinate.

    @return the grid 1D index.
 */
template <typename T>
  unsigned int Grid<T>::calcIndex(unsigned int x, unsigned int y) const {
    return y * width + x;
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
  T Grid<T>::getValAt(unsigned int x, unsigned int y) const {
    if (x >= width || x < 0) {
      throw std::runtime_error("Given x is not within bounds!");
    }
    if (y >= height || y < 0) {
      throw std::runtime_error("Given y is not within bounds!");
    }

    return grid[calcIndex(x, y)];
  }

/** Sets the vaue at the given index.

    Sets the value at the provided index to be equal to the
    provided new value.

    @param index
    The 1D index where the new value is to be placed.

    @param val
    The new value to be set at the specified index.
 */
template <typename T>
  void Grid<T>::setValAt(const unsigned int& index, const T& val) {
    if (index < 0 || index >= len()) {
      throw std::runtime_error("Given index is not within bounds!");
    }

    grid[index] = val;
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
  void Grid<T>::setValAt(const unsigned int& x, const unsigned int& y, const T& val) {
    if (x >= width || x < 0) {
      throw std::runtime_error("Given x is not within bounds!");
    }
    if (y >= height || y < 0) {
      throw std::runtime_error("Given y is not within bounds!");
    }

    grid[calcIndex(x, y)] = val;
  }

/** Retrieves the value at the given index.

    Retrieves the value held at the given index.

    @param index
    The 1D index where the new value is to be placed.

    @returns the given value held by the grid at the
    given index.
 */
template <typename T>
  T Grid<T>::operator [] (const unsigned int& index) const {
    if (index < 0 || index >= len()) {
      throw std::runtime_error("Given index is not within bounds!");
    }
    return grid[index];
  }

/** Gets the length of the grid.

    Gets the length of the interal 1D representation
    of the grid.

    @returns the length of the 1D representation.
 */
template <typename T>
  unsigned int Grid<T>::len() const {
    return grid.size();
  }

/** Gets the width of the grids rows.

    Gets the width of the grid row, effectively providing
    the column count.

    @returns the width of grid.
 */
template <typename T>
  unsigned int Grid<T>::getWidth() const {
    return width;
  }

/** Gets the length of the grids columns.

    Gets the length of the grid columns, effectively providing
    the row count.

    @returns the length of grid.
 */
template <typename T>
  unsigned int Grid<T>::getHeight() const {
    return height;
  }
