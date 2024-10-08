#ifndef WORDFREQUENCY_HPP
#define WORDFREQUENCY_HPP

#include <string>
#include "DynamicArray.hpp"

class WordFrequency {
public:
    WordFrequency();
    void addWord(const std::string& word);
    int getCount(const std::string& word) const;
    // Change return type to size_t
    size_t getSize() const; // Change return type to size_t
    std::string getWord(int index) const;
    int findMaxFrequency() const;
    int findMinFrequency() const;

private:
    DynamicArray<std::string> words; // Assuming this is your dynamic array for words
    DynamicArray<int> counts; // Assuming this is your dynamic array for counts
};

#endif // WORDFREQUENCY_HPP
