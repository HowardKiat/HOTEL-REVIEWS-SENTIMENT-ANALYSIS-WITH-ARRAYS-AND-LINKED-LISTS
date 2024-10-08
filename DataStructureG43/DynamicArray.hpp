#pragma once

#ifndef DYNAMICARRAY_HPP
#define DYNAMICARRAY_HPP

#include <iostream>
#include <string>
#include <stdexcept>

class DynamicArray {
private:
    std::string* data;
    size_t capacity;
    size_t size;

    void initialize(size_t initialCapacity);
    void resize(size_t newCapacity);

public:
    DynamicArray();
    DynamicArray(const DynamicArray& other);
    DynamicArray(DynamicArray&& other) noexcept;
    DynamicArray& operator=(const DynamicArray& other);
    DynamicArray& operator=(DynamicArray&& other) noexcept;
    ~DynamicArray();

    void push_back(const std::string& value);
    std::string& operator[](size_t index);
    size_t getSize() const;
    bool contains(const std::string& word) const;
    void countWords(const std::string& review, int& positiveCount, int& negativeCount) const;
    std::string get(size_t index) const;
};

#endif // DYNAMICARRAY_HPP
