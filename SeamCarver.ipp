#include "SeamCarver.hpp"

// TODO Remove
#include <iostream>

#include <algorithm>
#include <stdexcept>
#include <set>
#include <map>

#include "Util/Optional.hpp"

template <typename T>
  FlexGrid<T> seamCarve(const FlexGrid<T>& grid, const CarvingMode& mode, const unsigned int& amt) {
    std::cout << "Calcing energy" << std::endl;
    FlexGrid<T> energyGrid = calcEnergy(grid);
    std::cout << "Calcing cost" << std::endl;
    FlexGrid<T> costGrid = calcCost(energyGrid, mode);
    std::cout << "Carving!" << std::endl;
    return traceBackRem(grid, costGrid, mode, amt);
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
  FlexGrid<T> traceBackRemH(FlexGrid<T> grid, FlexGrid<T>& cost, const unsigned int& amt) {
    // Finding starting points
    std::map<T, unsigned int> startingPts;
    for (unsigned int h = 0; h < cost.getHeight(); ++h) {
      startingPts[cost.getValAt(cost.getWidth() - 1, h)] = h;
    }
    startingPts.erase(std::next(startingPts.begin(), amt), startingPts.end());

    std::set<unsigned int> indexes;
    std::set<unsigned int> nextIndexes;

    // Populate the indexes with the starting points
    for (auto& entry : startingPts) {
      indexes.insert(entry.second);
    }

    // Start removing seams
    for (unsigned int w = grid.getWidth() - 1; w >= 0; --w) {
      unsigned int offset = 0;
      for (unsigned int h = 0; h < grid.getHeight(); ++h) {
        if (indexes.find(h) != indexes.end()) {
          if (w > 0) {
            auto wPos = w - 1;

            // Constant Params
            Optional<T> center(cost.getValAt(wPos, h));

            // Optional Params
            Optional<T> above;
            Optional<T> below;

            if (h > 0) {
              above.setVal(cost.getValAt(wPos, h - 1));
            }
            if (h < cost.getHeight()) {
              below.setVal(cost.getValAt(wPos, h + 1));
            }

            Optional<T> minVal = std::min({above, center, below});

            if (minVal == above) {
              nextIndexes.insert(h + 1);
            } else if (minVal == below) {
              nextIndexes.insert(h - 1);
            } else {
              nextIndexes.insert(h);
            }
          }
          ++offset;
          continue;
        }

        if (offset > 0) {
          grid.setValAt(w - offset, h, grid.getValAt(w, h));
          cost.setValAt(w - offset, h, cost.getValAt(w, h));
        }
      }
      indexes = nextIndexes;
      nextIndexes.clear();
    }
    grid.setHeight(grid.getHeight() - amt);
    cost.setHeight(cost.getHeight() - amt);
    return grid;
  }

template <typename T>
  FlexGrid<T> traceBackRemV(FlexGrid<T> grid, FlexGrid<T>& cost, const unsigned int& amt) {
    // Finding starting points
    std::map<T, unsigned int> startingPts;
    for (unsigned int w = 0; w < cost.getWidth(); ++w) {
      startingPts[cost.getValAt(w, cost.getHeight() - 1)] = w;
    }
    startingPts.erase(std::next(startingPts.begin(), amt), startingPts.end());

    std::set<unsigned int> indexes;
    std::set<unsigned int> nextIndexes;

    // Populate the indexes with the starting points
    for (auto& entry : startingPts) {
      indexes.insert(entry.second);
    }

    // Start removing seams
    for (unsigned int h = grid.getHeight() - 1; h >= 0; --h) {
      unsigned int offset = 0;
      for (unsigned int w = 0; w < grid.getWidth(); ++w) {
        if (indexes.find(w) != indexes.end()) {
          if (h > 0) {
            auto hPos = h - 1;

            // Constant Params
            Optional<T> center(cost.getValAt(w, hPos));

            // Optional Params
            Optional<T> left;
            Optional<T> right;

            if (w > 0) {
              left.setVal(cost.getValAt(w - 1, hPos));
            }
            if (w < cost.getWidth()) {
              right.setVal(cost.getValAt(w + 1, hPos));
            }

            Optional<T> minVal = std::min({left, center, right});

            if (minVal == left) {
              nextIndexes.insert(w + 1);
            } else if (minVal == right) {
              nextIndexes.insert(w - 1);
            } else {
              nextIndexes.insert(w);
            }
          }
          ++offset;
          continue;
        }

        if (offset > 0) {
          grid.setValAt(w, h - offset, grid.getValAt(w, h));
          cost.setValAt(w, h - offset, cost.getValAt(w, h));
        }
      }
      indexes = nextIndexes;
      nextIndexes.clear();
    }
    grid.setWidth(grid.getHeight() - amt);
    cost.setWidth(cost.getHeight() - amt);
    return grid;
  }



template <typename T>
  FlexGrid<T> traceBackRem(FlexGrid<T> grid, FlexGrid<T>& cost, const CarvingMode& mode, const unsigned int& amt) {
    switch (mode) {
      case CarvingMode::HORIZONTAL:
        return traceBackRemH(grid, cost, amt);
      case CarvingMode::VERTICAL:
        return traceBackRemV(grid, cost, amt);
    }
    throw std::runtime_error("Invalid Carving Mode!");
  }
