#pragma once

#ifndef DYNAMICARRAY_HPP
#define DYNAMICARRAY_HPP

#include <iostream>
#include <string>

class DynamicArray {
private:
    std::string* data; // Pointer to dynamically allocated array
    size_t capacity;   // Current capacity of the array
    size_t size;       // Current number of elements

    void resize(size_t newCapacity); // Function to resize array

public:
    // Constructors
    DynamicArray();
    DynamicArray(const DynamicArray& other);
    DynamicArray(DynamicArray&& other) noexcept;

    // Assignment operators
    DynamicArray& operator=(const DynamicArray& other);
    DynamicArray& operator=(DynamicArray&& other) noexcept;

    // Destructor
    ~DynamicArray();

    // Member functions
    void push_back(const std::string& value);
    std::string& operator[](size_t index);
    size_t getSize() const;
    bool contains(const std::string& word) const;
    void countWords(const std::string& review, int& positiveCount, int& negativeCount) const;
};

#endif // DYNAMICARRAY_HPP
