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

    // Declarations for sorting functions
    void mergeSortFrequencies(); // Declaration for merge sort
    void quickSortFrequencies(); // Declaration for quick sort

    // Function to reset/clear all word frequencies (for re-sorting)
    void resetFrequencies();

private:
    DynamicArray<std::string> words; // Assuming this is your dynamic array for words
    DynamicArray<int> counts; // Assuming this is your dynamic array for counts
    void mergeSort(DynamicArray<std::string>& words, DynamicArray<int>& counts, size_t left, size_t right);
    void merge(DynamicArray<std::string>& words, DynamicArray<int>& counts, size_t left, size_t mid, size_t right);

    // Quick sort related functions
    void quickSort(DynamicArray<std::string>& words, DynamicArray<int>& counts, int low, int high);
    int partition(DynamicArray<std::string>& words, DynamicArray<int>& counts, int low, int high);
};

#endif // WORDFREQUENCY_HPP