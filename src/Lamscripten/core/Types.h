#ifndef SRC_LAMSCRIPTEN_CORE_TYPES_H_
#define SRC_LAMSCRIPTEN_CORE_TYPES_H_

#include <optional>

#include <Lamscripten/core/Memory.h>

namespace lamscripten::core {

template<class ValueType>
class DynamicArray {
 public:
  DynamicArray()
      : count_(0),
      capacity_(0),
      elements_(nullptr) {}

  ~DynamicArray() {
    FreeArray<ValueType>(elements_, count_);
  }

  /// @brief Allocate an array double the size of the input list
  explicit DynamicArray(std::initializer_list<ValueType> values)
      : count_(0), capacity_(0), elements_(nullptr) {
    for (auto value : values) {
      PushCopy(value);
    }
  }

  /// @brief Copy the array elements. Creates new memory for the newly
  /// constructed
  DynamicArray(const DynamicArray& array)
      : count_(0), capacity_(0), elements_(nullptr) {
    ResizeTo(array.capacity_);
    for (auto value : array) {
      PushCopy(value);
    }
  }

  DynamicArray(DynamicArray&& array) :
      count_(std::move(array.count_)),
      capacity_(std::move(array.capacity_)),
      elements_(std::exchange(array.elements_, nullptr)) {}


  void operator=(const DynamicArray& array) {
    count_ = array.count_;
    capacity_ = array.capacity_;
    elements_ = array.elements_;
  }

  /// @brief Push an item into the array.
  [[nodiscard]] size_t PushCopy(ValueType val) {
    [[unlikely]] if (ShouldResize()) {
      size_t new_size = capacity_ < 8 ? 8 : capacity_ * 2;
      ResizeTo(new_size);
    }

    elements_[count_] = val;
    count_ += 1;
    return count_ - 1;
  }

  [[nodiscard]] size_t PushMemory(ValueType&& val) {
    if (ShouldResize()) {
      size_t new_size = capacity_ < 8 ? 8 : capacity_ * 2;
      ResizeTo(new_size);
    }

    elements_[count_] = val;
    count_ += 1;
    return count_ - 1;
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
    [[unlikely]] if (new_size < count_ || new_size < capacity_) {
      return;
    }
    ResizeTo(new_size);
  }

  void ShrinkTo(size_t new_size) {
    [[unlikely]] if (new_size > count_ || new_size < capacity_) {
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
    if (index < count_) {
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

/// @brief Allocate an array of data on the stack.
template<class ValueType, size_t Size>
class FixedArray {
 private:
  ValueType array_[Size];
};

}  // namespace lamscripten::core

#endif  // SRC_LAMSCRIPTEN_CORE_TYPES_H_
