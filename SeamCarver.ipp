#include "SeamCarver.hpp"

#include <algorithm>
#include <stdexcept>

#include "Util/Optional.hpp"

template <typename T>
  FlexGrid<T> seamCarve(const FlexGrid<T>& grid, const CarvingMode& mode, const unsigned int& amt) {
    FlexGrid<T> newGrid = grid;
    for (unsigned int i = 0; i < amt; ++i) {
      FlexGrid<T> energyGrid = calcEnergy(newGrid);
      FlexGrid<T> costGrid = calcCost(energyGrid, mode);
      traceBackRem(newGrid, costGrid, mode);
    }
    return newGrid;
  }

template <typename T>
  FlexGrid<T> calcEnergy(const FlexGrid<T>& grid) {
    FlexGrid<T> r(grid.getWidth(), grid.getHeight());
    for (unsigned int h = 0; h < r.getHeight(); ++h) {
      for (unsigned int w = 0; w < r.getWidth(); ++w) {
        T cur = grid.getValAt(w, h);

        // Gen diffs
        T left   =  w > 0                 ? cur - grid.getValAt(w - 1, h) : 0;
        T right  =  w < r.getWidth() - 1  ? cur - grid.getValAt(w + 1, h) : 0;
        T top    =  h > 0                 ? cur - grid.getValAt(w, h - 1) : 0;
        T bottom =  h < r.getHeight() - 1 ? cur - grid.getValAt(w, h + 1) : 0;

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
  FlexGrid<T> calcCostH(const FlexGrid<T>& grid) {
    FlexGrid<T> r(grid.getWidth(), grid.getHeight());

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
        if (h < r.getHeight() - 1) {
          below.setVal(r.getValAt(wPos, h + 1));
        }

        T minVal = std::min({above, center, below}).getVal();

        r.setValAt(w, h, curVal + minVal);
      }
    }
    return r;
  }

template <typename T>
  FlexGrid<T> calcCostV(const FlexGrid<T>& grid) {
    FlexGrid<T> r(grid.getWidth(), grid.getHeight());

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
        if (w < r.getWidth() - 1) {
          right.setVal(r.getValAt(w + 1, hPos));
        }

        T minVal = std::min({left, center, right}).getVal();

        r.setValAt(w, h, curVal + minVal);
      }
    }
    return r;
  }

template <typename T>
  FlexGrid<T> calcCost(const FlexGrid<T>& grid, const CarvingMode& mode) {
    switch (mode) {
      case CarvingMode::HORIZONTAL:
        return calcCostH(grid);
      case CarvingMode::VERTICAL:
        return calcCostV(grid);
    }
    throw std::runtime_error("Invalid Carving Mode!");
  }

template <typename T>
  void traceBackRemH(FlexGrid<T>& grid, const FlexGrid<T>& cost) {
    // Finding starting points
    unsigned int next = 0;

    for (unsigned int h = 0; h < cost.getHeight(); ++h) {
      auto val = cost.getValAt(cost.getWidth() - 1, h);
      auto curVal = cost.getValAt(cost.getWidth() - 1, next);

      if (val < curVal) {
        next = h;
      }
    }

    // Start removing seams
    for (unsigned int w = grid.getWidth() - 1; w + 1 > 0; --w) {

      unsigned int newNext = 0;

      if (w > 0) {
        auto wPos = w - 1;

        // Constant Params
        Optional<T> center(cost.getValAt(wPos, next));

        // Optional Params
        Optional<T> above;
        Optional<T> below;

        if (next > 0) {
          above.setVal(cost.getValAt(wPos, next - 1));
        }
        if (next < cost.getHeight() - 1) {
          below.setVal(cost.getValAt(wPos, next + 1));
        }

        Optional<T> minVal = std::min({above, center, below});

        if (minVal == above) {
          newNext = next - 1;
        } else if (minVal == below) {
          newNext = next + 1;
        } else {
          newNext = next;
        }
      }

      for (unsigned int h = next + 1; h < grid.getHeight(); ++h) {
        grid.setValAt(w, h - 1, grid.getValAt(w, h));
      }
      next = newNext;
    }
    grid.setHeight(grid.getHeight() - 1);
  }

template <typename T>
  void traceBackRemV(FlexGrid<T>& grid, const FlexGrid<T>& cost) {
    // Finding starting points
    unsigned int next = 0;

    for (unsigned int w = 0; w < cost.getWidth(); ++w) {
      auto val = cost.getValAt(w, cost.getHeight() - 1);
      auto curVal = cost.getValAt(next, cost.getHeight() - 1);

      if (val < curVal) {
        next = w;
      }
    }

    // Start removing seams
    for (unsigned int h = grid.getHeight() - 1; h + 1 > 0; --h) {

      unsigned int newNext = 0;

      if (h > 0) {
        auto hPos = h - 1;

        // Constant Params
        Optional<T> center(cost.getValAt(next, hPos));

        // Optional Params
        Optional<T> left;
        Optional<T> right;

        if (next > 0) {
          left.setVal(cost.getValAt(next - 1, hPos));
        }
        if (next < cost.getWidth() - 1) {
          right.setVal(cost.getValAt(next + 1, hPos));
        }

        Optional<T> minVal = std::min({left, center, right});

        if (minVal == left) {
          newNext = next - 1;
        } else if (minVal == right) {
          newNext = next + 1;
        } else {
          newNext = next;
        }
      }

      for (unsigned int w = next + 1; w < grid.getWidth(); ++w) {
        grid.setValAt(w - 1, h, grid.getValAt(w, h));
      }
      next = newNext;
    }
    grid.setWidth(grid.getWidth() - 1);
  }



template <typename T>
  void traceBackRem(FlexGrid<T>& grid, const FlexGrid<T>& cost, const CarvingMode& mode) {
    switch (mode) {
      case CarvingMode::HORIZONTAL:
        traceBackRemH(grid, cost);
        return;
      case CarvingMode::VERTICAL:
        traceBackRemV(grid, cost);
        return;
    }
    throw std::runtime_error("Invalid Carving Mode!");
  }
