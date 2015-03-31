#include "SeamCarver.hpp"

#include <algorithm>
#include <stdexcept>

#include "Util/Optional.hpp"

template <typename T>
  Grid<T> seamCarve(const Grid<T>& grid, const int& horz, const int& vert) {
    Grid<T> energyGrid = calcEnergy(grid);
    Grid<T> costGrid = calcCost(energyGrid, CarvingMode::HORIZONTAL);
    return costGrid;
  }

template <typename T>
  Grid<T> calcEnergy(const Grid<T>& grid) {
    Grid<T> r(grid.getWidth(), grid.getHeight());
    for (unsigned int h = 0; h < r.getHeight(); ++h) {
      for (unsigned int w = 0; w < r.getWidth(); ++w) {
        T cur = grid.getValAt(w, h);

        // Gen diffs
        T left   =  w > 0              ? cur - grid.getValAt(w - 1, h) : 0;
        T right  =  w < r.getWidth()   ? cur - grid.getValAt(w + 1, h) : 0;
        T top    =  h > 0              ? cur - grid.getValAt(w, h - 1) : 0;
        T bottom =  h < r.getHeight()  ? cur - grid.getValAt(w, h + 1) : 0;

        // Make diffs abs
        left   = std::abs(left);
        right  = std::abs(right);
        top    = std::abs(top);
        bottom = std::abs(bottom);

        r.setValAt(w, h, left + right + top + bottom);
      }
    }
    return r;
  }

template <typename T>
  Grid<T> calcCostH(const Grid<T>& grid) {
    Grid<T> r(grid.getWidth(), grid.getHeight());

    for (unsigned int w = 0; w < r.getWidth(); ++w) {
      for (unsigned int h = 0; h < r.getHeight(); ++h) {
        // Initialize first row
        if (w < 1) {
          r.setValAt(0, h, grid.getValAt(0, h));
          continue;
        }

        auto wPos = w - 1;

        // Constant Params
        T curVal = grid.getValAt(w, h);
        Optional<T> center(r.getValAt(wPos, h));

        // Optional Params
        Optional<T> above;
        Optional<T> below;

        if (h > 0) {
          above.setVal(r.getValAt(wPos, h - 1));
        }
        if (h < r.getHeight()) {
          below.setVal(r.getValAt(wPos, h + 1));
        }

        T minVal = std::min({above, center, below}, [] (auto a, auto b) {
            return a.hasVal() && b.hasVal() && a.getVal() < b.getVal();
        }).getVal();

        r.setValAt(w, h, curVal + minVal);
      }
    }
    return r;
  }

template <typename T>
  Grid<T> calcCostV(const Grid<T>& grid) {
    Grid<T> r(grid.getWidth(), grid.getHeight());

    for (unsigned int h = 0; h < r.getHeight(); ++h) {
      for (unsigned int w = 0; w < r.getWidth(); ++w) {
        // Initialize first row
        if (h < 1) {
          r.setValAt(w, 0, grid.getValAt(w, 0));
          continue;
        }

        auto hPos = h - 1;

        // Constant Params
        T curVal = grid.getValAt(w, h);
        Optional<T> center(r.getValAt(w, hPos));

        // Optional Params
        Optional<T> left;
        Optional<T> right;

        if (w > 0) {
          left.setVal(r.getValAt(w - 1, hPos));
        }
        if (w < r.getWidth()) {
          right.setVal(r.getValAt(w + 1, hPos));
        }

        T minVal = std::min({left, center, right}, [] (auto a, auto b) {
            return a.hasVal() && b.hasVal() && a.getVal() < b.getVal();
        }).getVal();

        r.setValAt(w, h, curVal + minVal);
      }
    }
    return r;
  }

template <typename T>
  Grid<T> calcCost(const Grid<T>& grid, const CarvingMode& mode) {
    switch (mode) {
      case CarvingMode::HORIZONTAL:
        return calcCostH(grid);
      case CarvingMode::VERTICAL:
        return calcCostV(grid);
    }
    throw std::runtime_error("Invalid Carving Mode!");
  }
