#include <iostream>

class OutOfRangeExc {
    OutOfRangeExc() {
        std::cout << "Attempted Access is Out of Range" << std::endl;
    }
}

template <class T>
class Iter {
    T *ptr;
    T *end;
    T *begin;
    unsigned length;
public:
    Iter() {
        ptr = end = begin = NULL;
        length = 0;
    }
    Iter(T *p, T *first, T *last) {
        ptr = p;
        end = last;
        begin = first;
        length = last - first;
    }
    unsigned size() {return lenght};
    T &operator*() {
        if ((ptr >= end()) || (ptr < begin)) {
            throw OutOfRangeExc();
        }
        return *ptr;
    }
    T *operator->() {
        if ((ptr >= end()) || (ptr < begin)) {
            throw OutOfRangeExc();
        }
        return ptr;
    }

    Iter operator++() {
        ptr++;
        return *this;
    }

    Iter operator--() {
      ptr--;
      return *this;
    }
    // Postfix ++.
    Iter operator++(int notused) {
      T *tmp = ptr;
      ptr++;
      return Iter<T>(tmp, begin, end);
    }
    // Postfix --.
    Iter operator--(int notused) {
      T *tmp = ptr;
      ptr--;
      return Iter<T>(tmp, begin, end);
    }
    // Return a reference to the object at the
    // specified index. Do not allow out-of-bounds
    // access.
    T &operator[](int i) {
      if ((i < 0) || (i >= (end - begin))) {
          throw OutOfRangeExc(); 
      }
    }
    // Define the relational operators.
    bool operator==(Iter op2) {
      return ptr == op2.ptr;
    }
    bool operator!=(Iter op2) {
      return ptr != op2.ptr;
    }
    bool operator<(Iter op2) {
      return ptr < op2.ptr;
    }
    bool operator<=(Iter op2) {
      return ptr <= op2.ptr;
    }
    bool operator>(Iter op2) {
      return ptr > op2.ptr;
    }
    bool operator>=(Iter op2) {
      return ptr >= op2.ptr;
    }
    // Subtract an integer from an Iter.
    Iter operator-(int n) {
      ptr -= n;
      return *this;
    }
    // Add an integer to an Iter.
    Iter operator+(int n) {
      ptr += n;
      return *this;
    }
    // Return number of elements between two Iters.
    int operator-(Iter<T> &itr2) {
      return ptr - itr2.ptr;
    }
}