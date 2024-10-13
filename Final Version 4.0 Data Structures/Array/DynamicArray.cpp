//THIS FILE WILL NOT BE USED - HOWARD

//#include "DynamicArray.hpp"
//#include <stdexcept> // for std::out_of_range
//
//template <typename T>
//DynamicArray<T>::DynamicArray() : size(0), capacity(10) {
//    data = new T[capacity];
//}
//
//template <typename T>
//DynamicArray<T>::~DynamicArray() {
//    delete[] data; 
//}
//
//template <typename T>
//void DynamicArray<T>::resize() {
//    capacity *= 2;
//    T* newData = new T[capacity];
//    for (size_t i = 0; i < size; ++i) {
//        newData[i] = data[i];
//    }
//    delete[] data;
//    data = newData;
//}
//
//template <typename T>
//void DynamicArray<T>::push_back(const T& item) {
//    if (size >= capacity) {
//        resize();
//    }
//    data[size++] = item;
//}
//
//template <typename T>
//T DynamicArray<T>::get(size_t index) const {
//    if (index >= size) {
//        throw std::out_of_range("Index out of range");
//    }
//    return data[index];
//}
//
//template <typename T>
//size_t DynamicArray<T>::getSize() const {
//    return size;
//}


