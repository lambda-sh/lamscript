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
  [[nodiscard]] size_t Push(ValueType val) {
    if (ShouldResize()) {
      size_t new_size = capacity_ < 8 ? 8 : capacity_ * 2;
      ResizeTo(new_size);
    }

    elements_[count_] = val;
    count_ += 1;
  }

  /// @brief Pop an item off of the array.
  std::optional<ValueType> Pop() {
    [[unlikely]] if (count_ == 0) {
      return std::nullopt;
    }

    // Dereference and copy.
    ValueType val = elements_[count_ - 1];
    count_ -= 1;

    return val;
  }

  void GrowTo(size_t new_size) {
    [[unlikely]] if (new_size < count_) {
      return;
    }
    ResizeTo(new_size);
  }

  void ShrinkTo(size_t new_size) {
    [[unlikely]] if (new_size > count_) {
      return;
    }
    ResizeTo(new_size);
  }

  void ResizeTo(size_t new_size) {
    size_t old_capacity = capacity_;
    capacity_ = new_size;
    elements_ = ReallocateArray<ValueType>(elements_, old_capacity, capacity_);
  }

  size_t GetElementSize() const {
    return sizeof(ValueType);
  }

  size_t GetCount() const {
    return count_;
  }

  [[nodiscard]] auto begin() const {
    return elements_;
  }

  [[nodiscard]] auto end() const {
    return elements_ + count_;
  }

  [[nodiscard]] std::optional<ValueType> GetAtIndex(size_t index) const {
    if (index <= count_) {
      return elements_[index];
    }
    return std::nullopt;
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
