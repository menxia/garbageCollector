template<class T> 
class PtrDetails{
public:
    unsigned refcount;
    T *memPtr;
    bool isArray;
    unsigned arraySize;

    PtrDetails(T *ptr, int size=0) {
        memPtr = ptr;
        refcount = 1;
        arraySize = size;
        if(size==0) {
            isArray = false;
        } else {
            isArray = true;
        }
    }
    
};

template <class T> 
bool operator==(const PtrDetails<T> &obj1, const PtrDetails<T> &obj2) {
    return (obj1.memPtr == obj2.memPtr);
}