#ifndef SEAMCARVER_HPP
#define SEAMCARVER_HPP

#include "Util/FlexGrid.hpp"

enum class CarvingMode {
  HORIZONTAL,
  VERTICAL
};

template <typename T>
  FlexGrid<T> seamCarve(const FlexGrid<T>&, const unsigned int&, const unsigned int&);

template <typename T>
  FlexGrid<T> calcEnergy(const FlexGrid<T>&);

template <typename T>
  FlexGrid<T> calcCost(const FlexGrid<T>&, const CarvingMode&);

template <typename T>
  FlexGrid<T> traceBackRem(FlexGrid<T>, FlexGrid<T>&, const CarvingMode&, const unsigned int&);


#include "SeamCarver.ipp"
#endif
