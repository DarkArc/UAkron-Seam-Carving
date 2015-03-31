#ifndef SEAMCARVER_HPP
#define SEAMCARVER_HPP

#include "Util/Grid.hpp"

enum class CarvingMode {
  HORIZONTAL,
  VERTICAL
};

template <typename T>
  Grid<T> seamCarve(const Grid<T>&, const int&, const int&);

template <typename T>
  Grid<T> calcEnergy(const Grid<T>&);

template <typename T>
  Grid<T> calcCost(const Grid<T>&, const CarvingMode&);

template <typename T>
  Grid<T> traceBackRem(const Grid<T>&, const CarvingMode&);


#include "SeamCarver.ipp"
#endif
