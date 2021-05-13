#ifndef SRC_LAMSCRIPTEN_CORE_TYPES_H_
#define SRC_LAMSCRIPTEN_CORE_TYPES_H_

#include <Lamscripten/core/Error.h>
#include <Lamscripten/core/Memory.h>

namespace lamscripten::core {

template<class ValueType>
class Array {
 public:
  Array()
      : count_(0),
      capacity_(0),
      element_size_(sizeof(ValueType)),
      elements_(NULL) {}

  ~Array() {
    elements_ = ReallocateArray<ValueType>(elements_, count_, 0);
  }

  /// @brief Push an item into the array.
  void Push(const ValueType& val) {
    if (capacity_ < count_ + 2) {
      size_t old_capacity = capacity_;
      capacity_ = capacity_ < 8 ? 8 : capacity_ * 2;
      elements_ = ReallocateArray<ValueType>(
          elements_, old_capacity, capacity_);
    }

    elements_[count_] = ValueType(val);
    count_ += 1;
  }

  /// @brief Pop an item off of the array.
  std::optional<ValueType> Pop() {
    [[unlikely]] if (count_ == 0) {
      printf("here null opt pop\n");
      return std::nullopt;
    }

    // Dereference and copy.
    ValueType& val = elements_[count_ - 1];
    count_ -= 1;

    return val;
  }

 private:
  size_t count_;
  size_t capacity_;
  size_t element_size_;
  ValueType* elements_;
};

}  // namespace lamscripten::core

#endif  // SRC_LAMSCRIPTEN_CORE_TYPES_H_
