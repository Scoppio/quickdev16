#ifndef NALL_UTILITY_HPP
#define NALL_UTILITY_HPP

namespace nall {
  template<typename T>
  inline void swap(T &x, T &y) {
    T temp(x);
    x = y;
    y = temp;
  }

  template<typename T>
  struct base_from_member {
    T value;
    base_from_member(T value_) : value(value_) {}
  };

  class noncopyable {
  protected:
    noncopyable() {}
    ~noncopyable() {}

  private:
    noncopyable(const noncopyable&);
    const noncopyable& operator=(const noncopyable&);
  };
}

#endif
