#include "SeamCarver.hpp"

#include <algorithm>
#include <stdexcept>

#include "Util/Optional.hpp"

template <typename T>
  FlexGrid<T> seamCarve(const FlexGrid<T>& grid, const CarvingMode& mode, const unsigned int& amt) {
    // Create a copy of the grid, so that we do not modify the original
    FlexGrid<T> newGrid = grid;
    // Repeat the seam carving process amt times
    for (unsigned int i = 0; i < amt; ++i) {
      // Begin by caclulating an energy grid
      FlexGrid<T> energyGrid = calcEnergy(newGrid);
      // Use the energy grid to obtain a cost grid for the given mode
      FlexGrid<T> costGrid = calcCost(energyGrid, mode);
      // Finally trace back removing the least significant seam
      traceBackRem(newGrid, costGrid, mode);
    }
    return newGrid;
  }

template <typename T>
  FlexGrid<T> calcEnergy(const FlexGrid<T>& grid) {
    // Create a new grid of equal deminsions to serve as the
    // energy grid
    FlexGrid<T> r(grid.getWidth(), grid.getHeight());
    for (unsigned int h = 0; h < r.getHeight(); ++h) {
      for (unsigned int w = 0; w < r.getWidth(); ++w) {
        T cur = grid.getValAt(w, h);

        // Gen diffs for each adjacent pixel
        T left   =  w > 0                 ? cur - grid.getValAt(w - 1, h) : 0;
        T right  =  w < r.getWidth() - 1  ? cur - grid.getValAt(w + 1, h) : 0;
        T top    =  h > 0                 ? cur - grid.getValAt(w, h - 1) : 0;
        T bottom =  h < r.getHeight() - 1 ? cur - grid.getValAt(w, h + 1) : 0;

        // Make diffs absolute
        left   = std::abs(left);
        right  = std::abs(right);
        top    = std::abs(top);
        bottom = std::abs(bottom);

        // Update the energy value for the current pixel
        // based as calculated by the sum of the absolute
        // value of the difference in value between the
        // adjacent pixels
        r.setValAt(w, h, left + right + top + bottom);
      }
    }
    return r;
  }

template <typename T>
  FlexGrid<T> calcCostH(const FlexGrid<T>& energy) {
    // Create a new grid of equal deminisions to serve as the
    // cost grid
    FlexGrid<T> r(energy.getWidth(), energy.getHeight());
    for (unsigned int w = 0; w < r.getWidth(); ++w) {
      for (unsigned int h = 0; h < r.getHeight(); ++h) {
        // Initialize first column to match the first
        // column on the energy grid, as these two
        // start out the same
        if (w < 1) {
          r.setValAt(0, h, energy.getValAt(0, h));
          continue;
        }

        // Create a relative offset, for examining
        // the previous column
        auto wPos = w - 1;

        // Establish constant variables for the current pixel's value
        // and the neighboring pixel of the previous column
        const T curVal = energy.getValAt(w, h);
        const Optional<T> neighbor(r.getValAt(wPos, h));

        // Establish optional variables for the relative pixel
        // above and below the neighboring pixel, these values
        // must be optional to account for values which
        // go past the grid's bounds
        Optional<T> above;
        Optional<T> below;

        // Enable the respective optional(s) values if they
        // are within the bounds of the grid
        if (h > 0) {
          above.setVal(r.getValAt(wPos, h - 1));
        }
        if (h < r.getHeight() - 1) {
          below.setVal(r.getValAt(wPos, h + 1));
        }

        // Find the minimum value of the previous iterations
        // relative pixels
        T minVal = std::min({above, neighbor, below}).getVal();

        // Update the value for this cost grid to be equal to
        // the min, plus the value held at the current
        // position of the energy grid
        r.setValAt(w, h, curVal + minVal);
      }
    }
    return r;
  }

template <typename T>
  FlexGrid<T> calcCostV(const FlexGrid<T>& energy) {
    // Create a new grid of equal deminisions to serve as the
    // cost grid
    FlexGrid<T> r(energy.getWidth(), energy.getHeight());
    for (unsigned int h = 0; h < r.getHeight(); ++h) {
      for (unsigned int w = 0; w < r.getWidth(); ++w) {
        // Initialize first row to match the first
        // column on the energy grid, as these two
        // start out the same
        if (h < 1) {
          r.setValAt(w, 0, energy.getValAt(w, 0));
          continue;
        }

        // Create a relative offset, for examining
        // the previous row
        auto hPos = h - 1;

        // Establish constant variables for the current pixel's value
        // and the neighboring pixel of the previous row
        const T curVal = energy.getValAt(w, h);
        const Optional<T> neighbor(r.getValAt(w, hPos));

        // Establish optional variables for the relative pixel
        // left and right of the neighboring pixel, these values
        // must be optional to account for values which
        // go past the grid's bounds
        Optional<T> left;
        Optional<T> right;

        // Enable the respective optional(s) values if they
        // are within the bounds of the grid
        if (w > 0) {
          left.setVal(r.getValAt(w - 1, hPos));
        }
        if (w < r.getWidth() - 1) {
          right.setVal(r.getValAt(w + 1, hPos));
        }

        // Find the minimum value of the previous iterations
        // relative pixels
        T minVal = std::min({left, neighbor, right}).getVal();

        // Update the value for this cost grid to be equal to
        // the min, plus the value held at the current
        // position of the energy grid
        r.setValAt(w, h, curVal + minVal);
      }
    }
    return r;
  }

template <typename T>
  FlexGrid<T> calcCost(const FlexGrid<T>& grid, const CarvingMode& mode) {
    // Pick the proper cost matrix generation function
    // based on the carving mode
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
    // Create a variable to keep track of the position
    // to check for the next iteration
    unsigned int next = 0;

    // Finding starting point by locating the smallest
    // cost value in the last column
    for (unsigned int h = 0; h < cost.getHeight(); ++h) {
      auto val = cost.getValAt(cost.getWidth() - 1, h);
      auto curVal = cost.getValAt(cost.getWidth() - 1, next);

      if (val < curVal) {
        next = h;
      }
    }

    // Remove the respective pixel in the seam, row by row
    for (unsigned int w = grid.getWidth() - 1; w + 1 > 0; --w) {
      // Create a temporary value to hold
      // the position to check for the next iteration
      unsigned int newNext = 0;

      // Ensure we're not outside of the bounds of the grid
      if (w > 0) {
        // Create a relative offset, for examining
        // the previous row
        auto wPos = w - 1;

        // Establish a constant variable for the current
        // the neighboring pixel of the previous row
        const Optional<T> center(cost.getValAt(wPos, next));

        // Establish optional variables for the relative pixel
        // above and below the neighboring pixel, these values
        // must be optional to account for values which
        // go past the cost grid's bounds
        Optional<T> above;
        Optional<T> below;

        // Enable the respective optional(s) values if they
        // are within the bounds of the cost grid
        if (next > 0) {
          above.setVal(cost.getValAt(wPos, next - 1));
        }
        if (next < cost.getHeight() - 1) {
          below.setVal(cost.getValAt(wPos, next + 1));
        }

        // Find the minimum value of the next iterations
        // relative pixels, discorving the path
        // created during the cost grid's calculation
        Optional<T> minVal = std::min({above, center, below});

        // Pick the next positon to look at based
        // on the discovered path
        if (minVal == above) {
          newNext = next - 1;
        } else if (minVal == below) {
          newNext = next + 1;
        } else {
          newNext = next;
        }
      }

      // Collapse the row by shifting all values in the column
      // up after the currently targeted position in such a
      // way that the removed seam pixel is overriden
      for (unsigned int h = next + 1; h < grid.getHeight(); ++h) {
        grid.setValAt(w, h - 1, grid.getValAt(w, h));
      }
      // Update the next position based off of the temporary
      // value
      next = newNext;
    }
    // Resize the grid so that the last row which now contains
    // duplicate data is dropped
    grid.setHeight(grid.getHeight() - 1);
  }

template <typename T>
  void traceBackRemV(FlexGrid<T>& grid, const FlexGrid<T>& cost) {
    // Create a variable to keep track of the position
    // to check for the next iteration
    unsigned int next = 0;

    // Finding starting point by locating the smallest
    // cost value in the last row
    for (unsigned int w = 0; w < cost.getWidth(); ++w) {
      auto val = cost.getValAt(w, cost.getHeight() - 1);
      auto curVal = cost.getValAt(next, cost.getHeight() - 1);

      if (val < curVal) {
        next = w;
      }
    }

    // Remove the respective pixel in the seam, column by column
    for (unsigned int h = grid.getHeight() - 1; h + 1 > 0; --h) {
      // Create a temporary value to hold
      // the position to check for the next iteration
      unsigned int newNext = 0;

      // Ensure we're not outside of the bounds of the grid
      if (h > 0) {
        // Create a relative offset, for examining
        // the previous column
        auto hPos = h - 1;

        // Establish a constant variable for the current
        // the neighboring pixel of the previous column
        const Optional<T> center(cost.getValAt(next, hPos));

        // Establish optional variables for the relative pixel
        // left and right of the neighboring pixel, these values
        // must be optional to account for values which
        // go past the cost grid's bounds
        Optional<T> left;
        Optional<T> right;

        // Enable the respective optional(s) values if they
        // are within the bounds of the cost grid
        if (next > 0) {
          left.setVal(cost.getValAt(next - 1, hPos));
        }
        if (next < cost.getWidth() - 1) {
          right.setVal(cost.getValAt(next + 1, hPos));
        }

        // Find the minimum value of the next iterations
        // relative pixels, discorving the path
        // created during the cost grid's calculation
        Optional<T> minVal = std::min({left, center, right});

        // Pick the next positon to look at based
        // on the discovered path
        if (minVal == left) {
          newNext = next - 1;
        } else if (minVal == right) {
          newNext = next + 1;
        } else {
          newNext = next;
        }
      }

      // Collapse the column by shifting all values in the row
      // to the left after the currently targeted position
      // in such a way that the removed seam pixel is overriden
      for (unsigned int w = next + 1; w < grid.getWidth(); ++w) {
        grid.setValAt(w - 1, h, grid.getValAt(w, h));
      }
      // Update the next position based off of the temporary
      // value
      next = newNext;
    }
    // Resize the grid so that the last column which now contains
    // duplicate data is dropped
    grid.setWidth(grid.getWidth() - 1);
  }



template <typename T>
  void traceBackRem(FlexGrid<T>& grid, const FlexGrid<T>& cost, const CarvingMode& mode) {
    // Pick the proper removal function based on the carving mode
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
