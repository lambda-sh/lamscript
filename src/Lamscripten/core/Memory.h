#ifndef SRC_LAMSCRIPTEN_CORE_MEMORY_H_
#define SRC_LAMSCRIPTEN_CORE_MEMORY_H_

#include <algorithm>


/// @brief Reallocate memory for an array that wants to change in size. 
/// @tparam ArrayType The array type to allocate new memory for.
template<class ArrayType>
ArrayType* ReallocateArray(
    ArrayType* array, size_t old_size, size_t new_size) {
  if (new_size == 0) {
      delete[] pointer;
      return NULL;
  }

  // Allocate new array and copy contents of old array into it.
  ArrayType* new_array = new ArrayType[new_size];
  std::copy(array, array + old_size, new_array);

  if (array) {
      delete[] array;
  }

  return new_array;
}

#endif  // SRC_LAMSCRIPTEN_CORE_MEMORY_H_