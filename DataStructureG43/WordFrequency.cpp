#include "WordFrequency.hpp"
#include "DynamicArray.hpp"
#include <iostream>

WordFrequency::WordFrequency() {}

void WordFrequency::addWord(const std::string& word) {
    for (int i = 0; i < words.getSize(); ++i) {
        if (words[i] == word) { // Use operator[] instead of get
            counts[i]++;
            return; // Word found, increment count
        }
    }
    // If word not found, add it to the list
    words.push_back(word);
    counts.push_back(1);
}

int WordFrequency::getCount(const std::string& word) const {
    for (int i = 0; i < words.getSize(); ++i) {
        if (words[i] == word) { // Use operator[] instead of get
            return counts[i];
        }
    }
    return 0; // Return 0 if word not found
}

// Changed return type to size_t
size_t WordFrequency::getSize() const { // Change return type to size_t
    return words.getSize(); // Return size of words
}

std::string WordFrequency::getWord(int index) const {
    return words[index]; // Use operator[] instead of get
}

int WordFrequency::findMaxFrequency() const {
    int maxFrequency = 0;
    for (int i = 0; i < counts.getSize(); ++i) {
        if (counts[i] > maxFrequency) { // Use operator[] instead of get
            maxFrequency = counts[i]; // Update max frequency
        }
    }
    return maxFrequency;
}

int WordFrequency::findMinFrequency() const {
    if (counts.getSize() == 0) return 0; // Handle empty case

    int minFrequency = counts[0]; // Use operator[] instead of get
    for (int i = 1; i < counts.getSize(); ++i) {
        if (counts[i] < minFrequency) { // Use operator[] instead of get
            minFrequency = counts[i]; // Update min frequency
        }
    }
    return minFrequency;
}

