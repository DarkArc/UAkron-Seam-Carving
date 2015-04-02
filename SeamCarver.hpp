#ifndef SEAMCARVER_HPP
#define SEAMCARVER_HPP

#include "Util/FlexGrid.hpp"

enum class CarvingMode {
  HORIZONTAL,
  VERTICAL
};

template <typename T>
  FlexGrid<T> seamCarve(const FlexGrid<T>&, const CarvingMode&, const unsigned int&);

template <typename T>
  FlexGrid<T> calcEnergy(const FlexGrid<T>&);

template <typename T>
  FlexGrid<T> calcCost(const FlexGrid<T>&, const CarvingMode&);

template <typename T>
  void traceBackRem(FlexGrid<T>&, const FlexGrid<T>&, const CarvingMode&);


#include "SeamCarver.ipp"
#endif
