#ifndef WORDLIST_HPP
#define WORDLIST_HPP

#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

class WordList {
private:
    struct WordNode {
        std::string word;
        int count;
        WordNode* next;

        WordNode(const std::string& w) : word(w), count(1), next(nullptr) {}
    };

    WordNode* head;               // Linked list head
    std::string* positiveWords;   // Dynamic array for positive words
    std::string* negativeWords;   // Dynamic array for negative words
    int positiveCount;            // Count of positive words
    int negativeCount;            // Count of negative words
    int positiveCapacity;         // Dynamic capacity for positive words
    int negativeCapacity;         // Dynamic capacity for negative words

    void loadWords(const std::string& filename, std::string*& wordArray, int& wordCount, int& wordCapacity);

public:
    WordList();
    ~WordList();

    // Linked list operations
    void addWord(const std::string& word);
    void displayCounts() const;
    void findMaxMinWords(std::string& maxWord, int& maxCount, std::string& minWord, int& minCount);

    // Dynamic array operations for sentiment analysis
    bool isPositive(const std::string& word) const;
    bool isNegative(const std::string& word) const;
    void loadPositiveWords(const std::string& filename);
    void loadNegativeWords(const std::string& filename);

    // Additional methods
    void analyzeFrequencies(int& posCount, int& negCount) const;
    bool containsInList(const std::string& word) const; // Linked list search
};

#endif // WORDLIST_HPP
