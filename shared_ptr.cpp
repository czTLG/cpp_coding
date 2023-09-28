#include <iostream>

using uint = unsigned int;

template <typename T> class shared_ptr {
public:
  shared_ptr() : ptr_(nullptr), count_(new uint(0)) {}
  shared_ptr(T *ptr) : ptr_(ptr) {
    if (nullptr != ptr_) {
      count_ = new uint(1);
    }
  }

  ~shared_ptr() { reset(); }

  shared_ptr(const shared_ptr<T> &obj)
      : ptr_(obj.get()), count_(obj.get_count_ptr()) {
    if (count_) {
      (*count_)++;
    }
  }

  shared_ptr &operator=(const shared_ptr<T> &obj) {
    if (this != &this) {
      reset();
      ptr_ = obj.get();
      count_ = obj.get_count();
      if (count_) {
        (*count_)++;
      }
    }
    return *this;
  }

  shared_ptr(const shared_ptr<T> &&obj) {
    reset();
    ptr_ = obj.get();
    count_ = obj.get_count();
    if (count_) {
      (*count_)++;
    }
  }

  shared_ptr &operator=(const shared_ptr<T> &&obj) {
    if (this != &this) {
      reset();
      ptr_ = obj.ptr_;
      count_ = obj.get_count_ptr();

      obj.get() = nullptr;
      obj.get_count() = nullptr;
    }

    return *this;
  }

  T &operator*() const { return *ptr_; }

  T &operator->() const { return ptr_; }

  void reset() {
    if (count_) {
      (*count_)--;
      if (0 == *count_) {
        if (nullptr != ptr_) {
          delete ptr_;
        }
        delete count_;
      }

      ptr_ = nullptr;
      count_ = nullptr;
    }
  }

  T *get() const { return ptr_; }

  uint get_count() const { return count_ ? *count_ : 0U; }
  uint *get_count_ptr() const { return count_; }

private:
  T *ptr_;
  uint *count_;
};

using namespace std;
class Foo {
public:
  int val;
  Foo() : val(0) {}
};

int main() {
  shared_ptr<Foo> obj;
  std::cout << obj.get_count() << std::endl;

  shared_ptr<Foo> box1(new Foo());
  std::cout << box1.get_count() << std::endl;

  shared_ptr<Foo> box2(box1);
  std::cout << box1.get_count() << std::endl;
  std::cout << box2.get_count() << std::endl;

  box2.reset();
  std::cout << box1.get_count() << std::endl;
  std::cout << box2.get_count() << std::endl;

  return 0;
}