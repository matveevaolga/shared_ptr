#pragma once

template <class T>
class SharedPtr {
  int* strong_counter_ = nullptr;
  T* ptr_ = nullptr;

 public:
  explicit SharedPtr(T* ptr = nullptr) : strong_counter_(ptr ? new int(1) : nullptr), ptr_(ptr) {
  }

  SharedPtr(const SharedPtr& other) : strong_counter_(other.strong_counter_), ptr_(other.ptr_) {
    if (strong_counter_) {
      (*strong_counter_)++;
    }
  }

  SharedPtr& operator=(const SharedPtr& other) {
    if (strong_counter_ && other.ptr_ != ptr_) {
      if (*strong_counter_ - 1 == 0) {
        Reset();
      } else {
        (*strong_counter_)--;
      }
    }
    *this = SharedPtr(other);
    return *this;
  }

  SharedPtr(SharedPtr&& other) noexcept : strong_counter_(other.strong_counter_), ptr_(other.ptr_) {
    other.ptr_ = nullptr;
    other.strong_counter_ = nullptr;
  }

  SharedPtr& operator=(SharedPtr&& other) noexcept {
    if (strong_counter_ && other.ptr_ != ptr_) {
      if (*strong_counter_ - 1 == 0) {
        Reset();
      } else {
        (*strong_counter_)--;
      }
    }
    ptr_ = other.ptr_;
    strong_counter_ = other.strong_counter_;
    other.ptr_ = nullptr;
    other.strong_counter_ = nullptr;
    return *this;
  }

  void Reset(T* ptr = nullptr) {
    if (strong_counter_) {
      (*strong_counter_)--;
      if (*strong_counter_ == 0) {
        delete strong_counter_;
        delete ptr_;
      }
    }
    ptr_ = ptr;
    strong_counter_ = (ptr_ == nullptr ? nullptr : new int(1));
  }

  void Swap(SharedPtr& other) noexcept {
    T* tmp_ptr = ptr_;
    int* tmp_count = strong_counter_;
    ptr_ = other.ptr_;
    strong_counter_ = other.strong_counter_;
    other.ptr_ = tmp_ptr;
    other.strong_counter_ = tmp_count;
  }

  T* Get() const {
    return ptr_;
  }

  int UseCount() const {
    if (strong_counter_) {
      return *strong_counter_;
    }
    return 0;
  }

  T& operator*() const {
    return *ptr_;
  }

  T* operator->() const {
    return ptr_;
  }

  explicit operator bool() const {
    return ptr_ != nullptr;
  }

  ~SharedPtr() {
    Reset();
  }
};