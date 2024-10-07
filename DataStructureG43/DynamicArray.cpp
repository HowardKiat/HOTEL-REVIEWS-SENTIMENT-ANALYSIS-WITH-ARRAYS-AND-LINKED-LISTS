#include "DynamicArray.hpp"
#include <iostream>
#include <sstream> // For std::istringstream

// Constructor
DynamicArray::DynamicArray() {
    initialize(2); // Start with a capacity of 2
}

// Initialize the dynamic array
void DynamicArray::initialize(size_t initialCapacity) {
    data = new std::string[initialCapacity];
    capacity = initialCapacity;
    size = 0;
}

// Copy Constructor
DynamicArray::DynamicArray(const DynamicArray& other) {
    initialize(other.capacity);
    size = other.size;
    for (size_t i = 0; i < size; ++i) {
        data[i] = other.data[i];
    }
}

// Move Constructor
DynamicArray::DynamicArray(DynamicArray&& other) noexcept : data(other.data), capacity(other.capacity), size(other.size) {
    other.data = nullptr;
    other.capacity = 0;
    other.size = 0;
}

// Assignment Operator
DynamicArray& DynamicArray::operator=(const DynamicArray& other) {
    if (this != &other) {
        delete[] data; // Free existing resource
        initialize(other.capacity);
        size = other.size;
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }
    return *this;
}

// Move Assignment Operator
DynamicArray& DynamicArray::operator=(DynamicArray&& other) noexcept {
    if (this != &other) {
        delete[] data; // Free existing resource
        data = other.data;
        capacity = other.capacity;
        size = other.size;

        other.data = nullptr;
        other.capacity = 0;
        other.size = 0;
    }
    return *this;
}

// Destructor
DynamicArray::~DynamicArray() {
    delete[] data; // Free allocated memory
}

// Resize the array to a new capacity
void DynamicArray::resize(size_t newCapacity) {
    std::string* newData = new std::string[newCapacity];
    for (size_t i = 0; i < size; ++i) {
        newData[i] = std::move(data[i]); // Use move semantics for efficiency
    }
    delete[] data;
    data = newData;
    capacity = newCapacity;
}

// Add a value to the end of the array
void DynamicArray::push_back(const std::string& value) {
    if (size >= capacity) {
        resize(capacity * 2); // Double the capacity when needed
    }
    data[size++] = value;
}

// Access an element by index
std::string& DynamicArray::operator[](size_t index) {
    if (index >= size) {
        throw std::out_of_range("Index out of bounds");
    }
    return data[index];
}

// Get the current size of the array
size_t DynamicArray::getSize() const {
    return size;
}

// Check if a word is contained in the array
bool DynamicArray::contains(const std::string& word) const {
    for (size_t i = 0; i < size; ++i) {
        if (data[i] == word) {
            return true;
        }
    }
    return false;
}

// Count positive and negative words in a review
void DynamicArray::countWords(const std::string& review, int& positiveCount, int& negativeCount) const {
    std::istringstream reviewStream(review);
    std::string word;
    while (reviewStream >> word) {
        if (contains(word)) {
            positiveCount++;
        }
        else {
            negativeCount++;
        }
    }
}

// Get method for an element at index
std::string DynamicArray::get(size_t index) const {
    if (index < size) {
        return data[index];
    }
    else {
        throw std::out_of_range("Index out of range");
    }
}
