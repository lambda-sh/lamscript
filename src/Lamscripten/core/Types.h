#ifndef SRC_LAMSCRIPTEN_CORE_TYPES_H_
#define SRC_LAMSCRIPTEN_CORE_TYPES_H_

#include <Lamscripten/core/Memory.h>

namespace lamscripten::core {

template<class ValueType>
class Array {
 public:
  Array()
      : count_(0),
      capacity_(0),
      elements_(nullptr) {}

  /// @brief Push an item into the array.
  void Push(ValueType val) {
    if (ShouldResize()) {
      size_t old_capacity = capacity_;
      capacity_ = capacity_ < 8 ? 8 : capacity_ * 2;
      elements_ = ReallocateArray<ValueType>(
          elements_, old_capacity, capacity_);
    }

    elements_[count_] = val;
    count_ += 1;
  }

  /// @brief Pop an item off of the array.
  std::optional<ValueType> Pop() {
    [[unlikely]] if (count_ == 0) {
      printf("here null opt pop\n");
      return std::nullopt;
    }

    // Dereference and copy.
    ValueType val = elements_[count_ - 1];
    count_ -= 1;

    return val;
  }

  size_t GetElementSize() const {
    return sizeof(ValueType);
  }

 private:
  size_t count_;
  size_t capacity_;
  ValueType* elements_;

  bool ShouldResize() const {
    return capacity_ < count_ + 1;
  }
};

}  // namespace lamscripten::core

#endif  // SRC_LAMSCRIPTEN_CORE_TYPES_H_
