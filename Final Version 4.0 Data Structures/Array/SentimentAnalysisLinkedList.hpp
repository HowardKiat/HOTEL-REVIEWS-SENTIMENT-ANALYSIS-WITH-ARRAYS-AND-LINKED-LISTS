// SentimentAnalysisLinkedList.hpp

#pragma once

#ifndef SENTIMENTANALYSIS_LINKEDLIST_HPP
#define SENTIMENTANALYSIS_LINKEDLIST_HPP

#include <iostream>
#include <fstream>
#include <string>

using std::string;

const int MAX_WORDS = 5000;         // Maximum number of positive/negative words
const int MAX_FREQUENCIES = 1000;   // Maximum number of unique words to track frequency

// Structure for a single review (linked list node)
struct ReviewNode {
    string reviewText;
    int rating;
    ReviewNode* next;

    ReviewNode(const string& text, int rate)
        : reviewText(text), rating(rate), next(nullptr) {}
};

class SentimentAnalysisLinkedList {
private:
    // Pointers to arrays for positive and negative words (dynamically allocated)
    string* positiveWords;
    string* negativeWords;
    int positiveWordCount;
    int negativeWordCount;

    // Arrays to track word frequency
    string trackWordsFrequency[MAX_FREQUENCIES];    // Array to store the words
    int trackWordFrequencyCounts[MAX_FREQUENCIES];  // Array to store the counts
    int individualWordCount;

    // Linked list for storing reviews
    ReviewNode* reviewHead;
    int reviewCount;

public:
    // Constructor
    SentimentAnalysisLinkedList();

    // Load words
    void loadPositiveWords(const char* filename);
    void loadNegativeWords(const char* filename);

    // Load reviews from CSV
    void loadCSV(const char* filename);

    // Add a review
    void addReview(const string& reviewText, int rating);

    // Utility functions
    void toLowercase(string& word);
    void removePunctuation(string& word);
    bool wordExists(const string& word, const string wordList[], int wordCount);
    void updateWordFrequency(const string& word);

    // Sentiment analysis
    void analyzeSentimentReviews();
    double calculateSentimentScore(int positiveCount, int negativeCount);
    void calculateOverallSentiment();

    // Destructor
    ~SentimentAnalysisLinkedList();
};

#endif
