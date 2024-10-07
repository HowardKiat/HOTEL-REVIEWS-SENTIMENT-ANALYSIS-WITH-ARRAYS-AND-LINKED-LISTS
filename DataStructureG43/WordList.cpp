#include "WordList.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

// Constructor
WordList::WordList() : head(nullptr) {}

// Destructor
WordList::~WordList() {
    WordNode* current = head;
    while (current) {
        WordNode* next = current->next; // Store next node
        delete current;                  // Delete current node
        current = next;                  // Move to the next node
    }
}

// Add a word to the linked list
void WordList::addWord(const std::string& word) {
    WordNode* current = head;
    WordNode* prev = nullptr;

    // Traverse the list to find the word
    while (current) {
        if (current->word == word) {
            current->count++; // Increment count if found
            return;          // Exit if found
        }
        prev = current;       // Keep track of previous node
        current = current->next; // Move to next node
    }

    // Create a new node if the word wasn't found
    WordNode* newNode = new WordNode(word);
    if (prev) {
        prev->next = newNode; // Link new node to the list
    }
    else {
        head = newNode; // If this is the first node
    }
}

// Display counts of each word
void WordList::displayCounts() {
    WordNode* current = head;
    while (current) {
        std::cout << current->word << " = " << current->count << " times" << std::endl;
        current = current->next; // Move to the next node
    }
}

// Analyze word frequencies
void WordList::analyzeFrequencies(int& positiveCount, int& negativeCount) {
    positiveCount = 0; // Reset counts
    negativeCount = 0;
    WordNode* current = head;

    // Traverse the list to analyze frequencies
    while (current) {
        if (isPositive(current->word)) {
            positiveCount += current->count; // Increment positive count
        }
        else if (isNegative(current->word)) {
            negativeCount += current->count; // Increment negative count
        }
        current = current->next; // Move to the next node
    }
}

// Find maximum and minimum words
void WordList::findMaxMinWords(std::string& maxWord, int& maxCount, std::string& minWord, int& minCount) {
    WordNode* current = head;
    if (!current) return; // Return if the list is empty

    maxWord = current->word;
    maxCount = current->count;
    minWord = current->word;
    minCount = current->count;

    // Traverse the list to find max/min counts
    while (current) {
        if (current->count > maxCount) {
            maxWord = current->word;
            maxCount = current->count;
        }
        if (current->count < minCount) {
            minWord = current->word;
            minCount = current->count;
        }
        current = current->next; // Move to the next node
    }
}

// Implement the isPositive and isNegative functions based on your positive/negative words list
bool WordList::isPositive(const std::string& word) {
    // Implement your logic here
    return false; // Placeholder
}

bool WordList::isNegative(const std::string& word) {
    // Implement your logic here
    return false; // Placeholder
}
