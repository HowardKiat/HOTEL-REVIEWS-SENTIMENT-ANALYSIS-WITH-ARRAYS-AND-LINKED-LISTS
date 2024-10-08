#include "DynamicArray.hpp"
#include <string>
#include <iostream>

DynamicArray::DynamicArray() {
    initialize(2);
}

void DynamicArray::initialize(size_t initialCapacity) {
    data = new std::string[initialCapacity];
    capacity = initialCapacity;
    size = 0;
}

DynamicArray::DynamicArray(const DynamicArray& other) {
    initialize(other.capacity);
    size = other.size;
    for (size_t i = 0; i < size; ++i) {
        data[i] = other.data[i];
    }
}

DynamicArray::DynamicArray(DynamicArray&& other) noexcept
    : data(other.data), capacity(other.capacity), size(other.size) {
    other.data = nullptr;
    other.capacity = 0;
    other.size = 0;
}

DynamicArray& DynamicArray::operator=(const DynamicArray& other) {
    if (this != &other) {
        delete[] data;
        initialize(other.capacity);
        size = other.size;
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }
    return *this;
}

DynamicArray& DynamicArray::operator=(DynamicArray&& other) noexcept {
    if (this != &other) {
        delete[] data;
        data = other.data;
        capacity = other.capacity;
        size = other.size;

        other.data = nullptr;
        other.capacity = 0;
        other.size = 0;
    }
    return *this;
}

DynamicArray::~DynamicArray() {
    delete[] data;
}

void DynamicArray::resize(size_t newCapacity) {
    if (newCapacity < size) {
        throw std::invalid_argument("New capacity cannot be less than the current size.");
    }

    std::string* newData = new std::string[newCapacity];
    for (size_t i = 0; i < size; ++i) {
        newData[i] = std::move(data[i]);  // Moving old data to new array
    }
    delete[] data;  // Clean up old memory
    data = newData;
    capacity = newCapacity;
}

void DynamicArray::push_back(const std::string& value) {
    if (size >= capacity) {
        resize(capacity * 2);
    }
    data[size++] = value;
}

std::string& DynamicArray::operator[](size_t index) {
    if (index >= size) {
        throw std::out_of_range("Index out of bounds");
    }
    return data[index];
}

size_t DynamicArray::getSize() const {
    return size;
}

bool DynamicArray::contains(const std::string& word) const {
    for (size_t i = 0; i < size; ++i) {
        if (data[i] == word) {
            return true;
        }
    }
    return false;
}

void DynamicArray::countWords(const std::string& review, int& positiveCount, int& negativeCount) const {
    size_t start = 0;
    size_t end = review.find(" ");

    while (end != std::string::npos) {
        std::string word = review.substr(start, end - start);

        // Adjust to check positive and negative counts
        if (contains(word)) {
            positiveCount++;
        }
        // Implement logic for negative count if needed

        start = end + 1;
        end = review.find(" ", start);
    }

    std::string lastWord = review.substr(start);
    if (contains(lastWord)) {
        positiveCount++;  // or negativeCount based on your logic
    }
}

std::string DynamicArray::get(size_t index) const {
    if (index >= size) {
        throw std::out_of_range("Index out of bounds");
    }
    return data[index];
}
