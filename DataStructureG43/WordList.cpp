#include <iostream>
#include <fstream>
#include <stdexcept>
#include "WordList.hpp"

using namespace std;

void WordList::resizeWords(std::string*& wordArray, int& wordCount, int& wordCapacity) {
    wordCapacity *= 2;  // Doubling capacity
    std::string* newArray = new std::string[wordCapacity];
    for (int i = 0; i < wordCount; ++i) {
        newArray[i] = wordArray[i];
    }
    delete[] wordArray;
    wordArray = newArray;
}

void WordList::loadWords(const std::string& filename, std::string*& wordArray, int& wordCount, int& wordCapacity) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Could not open file: " + filename);
    }

    string line;
    wordCount = 0;

    while (getline(file, line)) {
        if (line.empty()) continue;

        if (wordCount >= wordCapacity) {
            resizeWords(wordArray, wordCount, wordCapacity);
        }

        wordArray[wordCount++] = line;
    }
    file.close();
}

WordList::WordList() : head(nullptr), positiveWords(new std::string[100]), negativeWords(new std::string[100]), positiveCount(0), negativeCount(0), positiveCapacity(100), negativeCapacity(100) {}

WordList::~WordList() {
    WordNode* current = head;
    while (current) {
        WordNode* next = current->next;
        delete current;
        current = next;
    }
    delete[] positiveWords;
    delete[] negativeWords;
}

void WordList::loadPositiveWords(const std::string& filename) {
    loadWords(filename, positiveWords, positiveCount, positiveCapacity);
}

void WordList::loadNegativeWords(const std::string& filename) {
    loadWords(filename, negativeWords, negativeCount, negativeCapacity);
}

void WordList::addWord(const std::string& word) {
    WordNode* current = head;
    while (current) {
        if (current->word == word) {
            current->count++;
            return; // Increment count if word exists
        }
        current = current->next;
    }

    WordNode* newNode = new WordNode(word); // Create new node
    newNode->next = head; // Insert at head
    head = newNode;
}

void WordList::displayCounts() const {
    cout << "Positive Words Count: " << positiveCount << endl;
    cout << "Negative Words Count: " << negativeCount << endl;
}

void WordList::analyzeFrequencies(int& posCount, int& negCount) const {
    posCount = positiveCount;
    negCount = negativeCount;
}

void WordList::findMaxMinWords(std::string& maxWord, int& maxCount, std::string& minWord, int& minCount) const {
    maxCount = 0;
    minCount = INT_MAX;

    WordNode* current = head;
    while (current) {
        if (current->count > maxCount) {
            maxCount = current->count;
            maxWord = current->word;
        }
        if (current->count < minCount) {
            minCount = current->count;
            minWord = current->word;
        }
        current = current->next;
    }
}

bool WordList::isPositive(const std::string& word) const {
    for (int i = 0; i < positiveCount; ++i) {
        if (positiveWords[i] == word) {
            return true; // Word found in positive list
        }
    }
    return false; // Word not found
}

bool WordList::isNegative(const std::string& word) const {
    for (int i = 0; i < negativeCount; ++i) {
        if (negativeWords[i] == word) {
            return true; // Word found in negative list
        }
    }
    return false; // Word not found
}

bool WordList::containsInList(const std::string& word) const {
    WordNode* current = head;
    while (current) {
        if (current->word == word) {
            return true; // Word found in linked list
        }
        current = current->next;
    }
    return false; // Word not found
}

