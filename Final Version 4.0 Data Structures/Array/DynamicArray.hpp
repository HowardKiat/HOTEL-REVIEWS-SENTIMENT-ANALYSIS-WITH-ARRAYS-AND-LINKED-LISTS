#ifndef DYNAMICARRAY_HPP
#define DYNAMICARRAY_HPP

#include <stdexcept> // for std::out_of_range

template <typename T>
class DynamicArray {
public:
    DynamicArray();               // Constructor
    ~DynamicArray();              // Destructor
    void push_back(const T& item); // Add an item to the array
    T get(size_t index) const;    // Get item at index
    size_t getSize() const;        // Get current size of the array

    // Overload the [] operator
    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    // Reset the dynamic array
    void reset() {
        size = 0; // Simply set size to 0, "removing" all elements
    }

private:
    void resize();                 // Resize the array when needed
    T* data;                       // Pointer to the data
    size_t size;                  // Current size
    size_t capacity;              // Current capacity
};

// Constructor
template <typename T>
DynamicArray<T>::DynamicArray() : size(0), capacity(10) {
    data = new T[capacity];
}

// Destructor
template <typename T>
DynamicArray<T>::~DynamicArray() {
    delete[] data;
}

// Resize function
template <typename T>
void DynamicArray<T>::resize() {
    capacity *= 2;
    T* newData = new T[capacity];
    for (size_t i = 0; i < size; ++i) {
        newData[i] = data[i];
    }
    delete[] data;
    data = newData;
}

// Push back function
template <typename T>
void DynamicArray<T>::push_back(const T& item) {
    if (size >= capacity) {
        resize();
    }
    data[size++] = item;
}

// Get function
template <typename T>
T DynamicArray<T>::get(size_t index) const {
    if (index >= size) {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

// Get size function
template <typename T>
size_t DynamicArray<T>::getSize() const {
    return size;
}

// Overload the [] operator for non-const
template <typename T>
T& DynamicArray<T>::operator[](size_t index) {
    if (index >= size) {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

// Overload the [] operator for const
template <typename T>
const T& DynamicArray<T>::operator[](size_t index) const {
    if (index >= size) {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

#endif // DYNAMICARRAY_HPP