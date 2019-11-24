#include <iostream>
#include <list>
#include <typeinfo>
#include <cstdlib>
#include "detail.h"


template <class  T, int size=0>
class Pointer{
private:
    // refContainer maintains the garbage collection list.
    static std::list<PtrDetails<T>> refContainer;
    // addr points to the allocated memory to which this Pointer pointer currently points
    T *addr;
    bool isArray;
    unsigned arraySize;
    static bool first;
    // Return an iterator to pointer details in refContainer.
    typename std::list<PtrDetails<T>>::iterator findPtrInfo(T *ptr);
public:
    typedef Iter<T> GCiterator;
    Pointer() {
        Pointer(NULL);
    }
    Pointer(T*);
    Pointer(const Pointer &);
    ~Pointer();

    static bool collect();
    T *operator=(T *t);
    Pointer &operator=(Pointer &rv);
    T &operator*(){
        return *addr;
    }
    T *operator->(){return addr;}
    T &operator[](int i){return addr[i];}
    operator T *() {return addr;}
    Iter<T> begin(){
        int _size;
        if (isArray) {_size = arraySize;}
        else {_size=1;}
        return Iter<T>(addr, addr, addr+_size);
    }
    Iter<T> end() {
        int _size;
        if (isArray)
            _size = arraySize;
        else
            _size = 1;
        return Iter<T>(addr + _size, addr, addr + _size);
    }
    static int refContainerSize() {return refContainer.size();}
    static void showlist();
    static void shutdown();
};

template <class T, int size>
std::list<PtrDetails<T>> Pointer(T, size)::refContainer;
template <class T, int size>
bool Pointer<T,size>::first = true;

template <class T, int size>
Pointer<T, size>::Pointer(T *t) {
    if (first) atexit(shutdown);
    first = false;
    typename std::list<PtrDetails<T>>::iterator p;
  p = findPtrInfo(t);

  if (p != refContainer.end()) {
    p->refcount++;
  }
  else {
    PtrDetails<T> obj(t, size);
    refContainer.push_front(obj);
  }

  addr = t; 
  arraySize = size;

  if (size >0) {
    isArray = true;
  }
  else { 
    isArray = false;
  }
}

// Copy constructor.
template< class T, int size>
Pointer<T,size>::Pointer(const Pointer &ob) {
  typename std::list<PtrDetails<T>>::iterator p;
  p = findPtrInfo(ob.addr);
  p->refcount++;  // increment ref count
  addr = ob.addr;
  arraySize = ob.arraySize;

  if (arraySize > 0) {
    isArray = true;
  } 
  else {
    isArray = false;
  }
}

// Destructor for Pointer.
template <class T, int size>
Pointer<T, size>::~Pointer() {
  typename std::list <PtrDetails<T>>::iterator p;
  p = findPtrInfo(addr);

  if (p->refcount) {
    p->refcount--;
  }
  collect();
}

// Collect garbage. Returns true if at least
// one object was freed.
template <class T, int size>
bool Pointer<T, size>::collect() {
  // collect() will be called in the destructor
  bool memfreed = false;
  typename std::list<PtrDetails<T>>::iterator p;

  do {
    for (p = refContainer.begin(); p != refContainer.end(); p++) {
      if (p->refcount > 0) {
        continue;
      }
      memfreed = true;
      refContainer.remove(*p);

      if (p->memPtr) {
        if (p->isArray) {
          delete[] p->memPtr;
        } 
        else {
          delete p->memPtr;
        }
      }
      break;
    }
  } while (p != refContainer.end());

  return memfreed;
}

// Overload assignment of pointer to Pointer.
template <class T, int size>
T *Pointer<T, size>::operator=(T *t) {
  typename std::list<PtrDetails<T>>::iterator p;
  p = findPtrInfo(addr);
  p->refcount--;
  p = findPtrInfo(t);

  if (p != refContainer.end()) {
    p->refcount++;
  }
  else {
    PtrDetails<T> obj(t,size);
    refContainer.push_front(obj);
  }
  addr = t;
  return t;
}

// Overload assignment of Pointer to Pointer.
template <class T, int size>
Pointer<T, size> &Pointer<T, size>::operator=(Pointer &rv) {
  typename std::list<PtrDetails<T>>::iterator p;
  p = findPtrInfo(addr);
  p->refcount--;
  p = findPtrInfo(rv.addr);
  p->refcount++;
  addr = rv.addr;
  return rv;
}

// A utility function that displays refContainer.
template <class T, int size>
void Pointer<T, size>::showlist() {
  typename std::list<PtrDetails<T> >::iterator p;
  std::cout << "refContainer<" << typeid(T).name() << ", " << size << ">:\n";
  std::cout << "memPtr refcount value\n ";

  if (refContainer.begin() == refContainer.end()) {
    std::cout << " Container is empty!\n\n ";
  }

  for (p = refContainer.begin(); p != refContainer.end(); p++) {
    std::cout << "[" << (void *)p->memPtr << "]"
              << " " << p->refcount << " ";
    if (p->memPtr) {
      std::cout << " " << *p->memPtr; 
    }
    else {
      std::cout << "---";
      std::cout << std::endl;
    }
  }
  std::cout << std::endl;
}

// Find a pointer in refContainer.
template <class T, int size>
typename std::list<PtrDetails<T> >::iterator
Pointer<T, size>::findPtrInfo(T *ptr) {
  typename std::list<PtrDetails<T> >::iterator p;
  // Find ptr in refContainer.
  for (p = refContainer.begin(); p != refContainer.end(); p++) {
    if (p->memPtr == ptr) {
      return p; 
    }
  }
  return p;
}

// Clear refContainer when program exits.
template <class T, int size>
void Pointer<T, size>::shutdown() {
  if (refContainerSize() == 0) {
    return; // list is empty 
  }
  typename std::list<PtrDetails<T> >::iterator p;
  for (p = refContainer.begin(); p != refContainer.end(); p++) {
    // Set all reference counts to zero
    p->refcount = 0;
  }
  collect();
}