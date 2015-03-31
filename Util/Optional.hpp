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

#ifndef OPTIONAL_HPP
#define OPTIONAL_HPP

template <typename T>
class Optional {
  bool present;
  T val;
public:
  Optional();
  Optional(const T& val);

  bool hasVal() const;
  T getVal() const;

  void clearVal();
  void setVal(const T& val);
};

template <typename K>
  bool operator == (const Optional<K>&, const Optional<K>&);
template <typename K>
  bool operator != (const Optional<K>&, const Optional<K>&);

#include "Optional.ipp"

#endif
