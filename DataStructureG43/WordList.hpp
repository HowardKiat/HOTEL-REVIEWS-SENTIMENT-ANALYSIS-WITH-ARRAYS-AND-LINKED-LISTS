#ifndef WORDLIST_HPP
#define WORDLIST_HPP

#include <iostream>
#include <string>
#include <stdexcept>

class WordList {
private:
    struct WordNode {
        std::string word;
        int count;
        WordNode* next;

        WordNode(const std::string& w) : word(w), count(1), next(nullptr) {}
    };

    WordNode* head;
    std::string* positiveWords;
    std::string* negativeWords;
    int positiveCount;
    int negativeCount;
    int positiveCapacity;
    int negativeCapacity;

    void loadWords(const std::string& filename, std::string*& wordArray, int& wordCount, int& wordCapacity);
    void resizeWords(std::string*& wordArray, int& wordCount, int& wordCapacity);

public:
    WordList();
    ~WordList();

    void addWord(const std::string& word);
    void displayCounts() const;
    void findMaxMinWords(std::string& maxWord, int& maxCount, std::string& minWord, int& minCount) const;
    bool isPositive(const std::string& word) const;
    bool isNegative(const std::string& word) const;
    void loadPositiveWords(const std::string& filename);
    void loadNegativeWords(const std::string& filename);
    void analyzeFrequencies(int& posCount, int& negCount) const;
    bool containsInList(const std::string& word) const;
};

#endif // WORDLIST_HPP

