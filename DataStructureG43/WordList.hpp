#ifndef WORDLIST_HPP
#define WORDLIST_HPP

#include <iostream>
#include <string>

class WordList {
private:
    struct WordNode {
        std::string word;
        int count;
        WordNode* next;

        WordNode(const std::string& w) : word(w), count(1), next(nullptr) {}
    };

    WordNode* head;

public:
    WordList();
    ~WordList();
    void addWord(const std::string& word);
    void displayCounts();
    void analyzeFrequencies(int& positiveCount, int& negativeCount);
    void findMaxMinWords(std::string& maxWord, int& maxCount, std::string& minWord, int& minCount);
    bool isPositive(const std::string& word);
    bool isNegative(const std::string& word);
};

#endif // WORDLIST_HPP
