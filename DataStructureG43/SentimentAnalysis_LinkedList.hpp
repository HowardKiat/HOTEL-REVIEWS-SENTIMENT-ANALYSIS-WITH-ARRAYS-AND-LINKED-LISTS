// SentimentAnalysisLinkedList.hpp

#pragma once

#ifndef SENTIMENTANALYSIS_LINKEDLIST_HPP
#define SENTIMENTANALYSIS_LINKEDLIST_HPP

#include <iostream>
#include <fstream>
#include <string>

using std::string;

// Define maximum sizes for word arrays
#define MAX_WORDS 1000

// Structure for a single review (linked list node)
struct ReviewNode {
    string reviewText;
    int rating;
    ReviewNode* next;

    ReviewNode(const string& text, int rate)
        : reviewText(text), rating(rate), next(nullptr) {}
};

// Structure for tracking word frequencies (linked list node)
struct WordFrequencyNode {
    string word;
    int count;
    WordFrequencyNode* next;

    WordFrequencyNode(const string& w)
        : word(w), count(1), next(nullptr) {}
};

class SentimentAnalysisLinkedList {
private:
    // Arrays for positive and negative words
    string positiveWords[MAX_WORDS];
    string negativeWords[MAX_WORDS];
    int positiveWordCount;
    int negativeWordCount;

    // Linked list for storing reviews
    ReviewNode* reviewHead;
    int reviewCount;

    // Linked list for tracking word frequencies
    WordFrequencyNode* frequencyHead;
    int individualWordCount;

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
    void compareSentimentWithRating(double sentimentScore, int userRating);
    void calculateOverallSentiment();

    // Destructor
    ~SentimentAnalysisLinkedList();
};

#endif
