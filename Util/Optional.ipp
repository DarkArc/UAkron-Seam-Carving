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

/** Construct a new Optional object.

    Constructs a new Optional object, which has
    no value present.
 */
template <typename T>
  Optional<T>::Optional() : present(false) { }

/** Construct a new Optional object.

    Constructs a new Optional object, which has
    the specified value present.

    @param val
    The value which this Optional object stores.
 */
template <typename T>
  Optional<T>::Optional(const T& val) : present(true), val(val) { }

/** Tells whether there is a stored value.

    @returns whether there is a stored value.
 */
template <typename T>
  bool Optional<T>::hasVal() const {
    return present;
  }

/** Retrives the stored value.

    Retrives the stored value, throwing a runtime error
    if there is not current a stored value in this
    object.

    @returns the stored object if successful.
*/
template <typename T>
  T Optional<T>::getVal() const {
    if (!hasVal()) {
      throw std::runtime_error("There is currently no held value!");
    }
    return val;
  }

/** Clears the stored value.
 */
template <typename T>
  void Optional<T>::clearVal() {
    present = false;
  }

/** Sets the stored value.

    Sets the stored value of this optional
    to the provided value. If an item
    already exists it will be overriden.

    @param val
    The new value to store.
 */
template <typename T>
  void Optional<T>::setVal(const T& val) {
    this->val = val;
    this->present = true;
  }

/** Checks equivalence between the two Optional objects.

    Checks to see if both Optionals contain the same value.

    @return true if both Optionals contain the same value,
    false otherwise.
 */
template <typename T, typename K>
  bool operator == (const Optional<K>& a, const Optional<K>& b) {
    return a.hasVal() == b.hasVal() && a.getVal() == b.getVal();
  }

/** Checks equivalence between the two Optional objects.

    Checks to see if both Optionals contain the same value.

    @return false if both Optionals contain the same value,
    true otherwise.
 */
template <typename T, typename K>
  bool operator != (const Optional<K>& a, const Optional<K>& b) {
    return !(a == b);
  }
