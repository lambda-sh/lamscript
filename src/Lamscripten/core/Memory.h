#ifndef SRC_LAMSCRIPTEN_CORE_MEMORY_H_
#define SRC_LAMSCRIPTEN_CORE_MEMORY_H_

#include <algorithm>

/// @brief Reallocate memory for an array that wants to change in size.
/// @tparam ArrayType The array type to allocate new memory for.
template<class ArrayType>
ArrayType* ReallocateArray(
    ArrayType* array, size_t old_count, size_t new_count) {
  if (new_count == 0) {
    delete[] array;
    return nullptr;
  }

  ArrayType* new_array = new ArrayType[new_count];
  std::copy_n(array, old_count, new_array);

  // If the old array still exists, delete it.
  [[likely]] if (array) {
    delete[] array;
  }

  // If allocation fails, exit.
  [[unlikely]] if (new_array == nullptr) {
    exit(1);
  }

  return new_array;
}

template<class ArrayType>
void FreeArray(ArrayType* array, size_t current_count) {
  ReallocateArray<ArrayType>(array, current_count, 0);
}

#endif  // SRC_LAMSCRIPTEN_CORE_MEMORY_H_
