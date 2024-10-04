//#pragma once
//
//#ifndef SENTIMENT_ANALYSIS_HPP
//#define SENTIMENT_ANALYSIS_HPP
//
//#include <iostream>
//#include <fstream>
//#include <string>
//
//using std::string;
//
//#define MAX_WORDS 1000
//#define MAX_REVIEWS 100
//#define MAX_FREQUENCIES 1000  // Maximum unique words tracked
//
//struct Review 
//{
//    string review;  
//    int rating;    
//
//    Review() : review(""), rating(0) {} 
//
//};
//
//class SentimentAnalysis
//{
//private:
//    Review* reviewList;                             // Dynamically allocated array for the reviews in the list
//    int reviewCount;                                // Number of reviews
//    string positiveWords[MAX_WORDS];                // Array for positive words
//    string negativeWords[MAX_WORDS];                // Array for negative words
//    int positiveWordCount;                          // Number of positive words
//    int negativeWordCount;                          // Number of negative words
//
//    // Arrays to track word frequencies
//    string trackWordsFrequency[MAX_FREQUENCIES];    // Tracks the Word Appearance Frequency
//    int trackWordFrequencyCounts[MAX_FREQUENCIES];  // Corresponding counts for the words
//    int individualWordCount;                        // Current number of individual words
//
//public:
//    // Constructor
//    SentimentAnalysis();
//
//    // Load positive and negative words from files
//    void loadPositiveWords();
//    void loadNegativeWords();
//
//    // Change To Lower Case Words
//    void toLowercase(string& word);
//
//    // Remove Puntuations 
//    void removePunctuation(string& word);
//
//    //Loaf CSV file
//    void loadCSV(const char* filename);
//
//    // Add reviews one by one
//    void addReview(const string& reviewText, int rating);
//
//    // Analyze sentiment of all reviews
//    void analyzeSentimentReviews();
//
//    // Calculate normalized sentiment score for a review
//    double calculateSentimentScore(int positiveCount, int negativeCount);
//
//    // Compare sentiment score with user rating
//    void compareSentimentWithRating(double sentimentScore, int userRating);
//
//    // Overall analysis across all reviews
//    void calculateOverallSentiment();
//
//    bool wordExists(const string& word, const string wordList[], int wordCount);
//    void updateWordFrequency(const string& word);
//
//    // Destructor
//    ~SentimentAnalysis();
//};
//
//#endif