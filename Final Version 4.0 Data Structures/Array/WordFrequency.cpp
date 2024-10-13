#pragma comment(linker, "/STACK:100000000") // Set the stack size to 100 MB
#include "WordFrequency.hpp"
#include "DynamicArray.hpp"
#include <iostream>
#include <chrono>

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

void WordFrequency::mergeSort(DynamicArray<std::string>& words, DynamicArray<int>& counts, size_t left, size_t right) {
    size_t n = right - left + 1; // Get the number of elements to sort

    // Outer loop for merging subarrays in bottom-up manner
    for (size_t curr_size = 1; curr_size <= n - 1; curr_size = 2 * curr_size) {
        // Pick starting point of different subarrays of current size
        for (size_t left_start = left; left_start < right; left_start += 2 * curr_size) {
            // Find mid point and right end point for merging
            size_t mid = std::min(left_start + curr_size - 1, right);
            size_t right_end = std::min(left_start + 2 * curr_size - 1, right);

            // Merge the subarrays
            merge(words, counts, left_start, mid, right_end);
        }
    }
}


void WordFrequency::merge(DynamicArray<std::string>& words, DynamicArray<int>& counts, size_t left, size_t mid, size_t right) {
    size_t leftSize = mid - left + 1;
    size_t rightSize = right - mid;

    // Create temporary arrays
    DynamicArray<std::string> leftWords;
    DynamicArray<int> leftCounts;
    DynamicArray<std::string> rightWords;
    DynamicArray<int> rightCounts;

    // Copy data to temporary arrays
    for (size_t i = 0; i < leftSize; ++i) {
        leftWords.push_back(words[left + i]);
        leftCounts.push_back(counts[left + i]);
    }
    for (size_t j = 0; j < rightSize; ++j) {
        rightWords.push_back(words[mid + 1 + j]);
        rightCounts.push_back(counts[mid + 1 + j]);
    }

    // Merge the temporary arrays back into words and counts
    size_t i = 0, j = 0, k = left;

    while (i < leftSize && j < rightSize) {
        if (leftCounts[i] <= rightCounts[j]) {
            words[k] = leftWords[i];
            counts[k] = leftCounts[i];
            i++;
        }
        else {
            words[k] = rightWords[j];
            counts[k] = rightCounts[j];
            j++;
        }
        k++;
    }

    // Copy remaining elements of leftWords, if any
    while (i < leftSize) {
        words[k] = leftWords[i];
        counts[k] = leftCounts[i];
        i++;
        k++;
    }

    // Copy remaining elements of rightWords, if any
    while (j < rightSize) {
        words[k] = rightWords[j];
        counts[k] = rightCounts[j];
        j++;
        k++;
    }
}

void WordFrequency::mergeSortFrequencies() {
    mergeSort(words, counts, 0, words.getSize() - 1);
}


void WordFrequency::quickSort(DynamicArray<std::string>& words, DynamicArray<int>& counts, int low, int high) {
    if (low < high) {
        int pivotIndex = partition(words, counts, low, high); // Get pivot index

        // Recursively sort the elements before and after partition
        quickSort(words, counts, low, pivotIndex - 1); // Sort left part
        quickSort(words, counts, pivotIndex + 1, high); // Sort right part
    }
}

int WordFrequency::partition(DynamicArray<std::string>& words, DynamicArray<int>& counts, int low, int high) {
    std::string pivot = words[high]; // Choose the last element as pivot
    int i = low - 1; // Index of smaller element

    for (int j = low; j < high; j++) {
        if (counts[j] <= counts[high]) { // Compare with pivot
            i++; // Increment index of smaller element
            std::swap(words[i], words[j]); // Swap
            std::swap(counts[i], counts[j]); // Swap counts as well
        }
    }
    std::swap(words[i + 1], words[high]); // Swap pivot to correct position
    std::swap(counts[i + 1], counts[high]); // Swap counts for pivot
    return i + 1; // Return the partition index
}

void WordFrequency::quickSortFrequencies() {
    quickSort(words, counts, 0, getSize() - 1); // Perform quick sort
}

void WordFrequency::resetFrequencies() {
    words.reset();  // Clear the words dynamic array
    counts.reset(); // Clear the counts dynamic array
}