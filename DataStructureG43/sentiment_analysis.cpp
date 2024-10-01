#include "sentiment_analysis.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm> // For std::transform

using namespace std;

// Constructor to initialize member variables
SentimentAnalysis::SentimentAnalysis()
    : reviewCount(0), positiveWordCount(0), negativeWordCount(0), uniqueWordCount(0) {
    reviewList = new Review[MAX_REVIEWS]; // Allocate memory for reviews

    // Initialize frequency counts to zero
    for (int i = 0; i < MAX_FREQUENCIES; ++i) {
        frequencyCounts[i] = 0; // Initialize all counts to zero
        frequencyWords[i] = "";  // Initialize all frequency words to an empty string
    }
}

// Load positive words from a text file
void SentimentAnalysis::loadPositiveWords() {
    ifstream file("positive-words.txt");
    if (!file.is_open()) {
        cout << "Error opening Positive Words File!" << endl;
        return;
    }

    while (positiveWordCount < MAX_WORDS && file >> positiveWords[positiveWordCount]) {
        positiveWordCount++;
    }
    file.close();
}

// Load negative words from a text file
void SentimentAnalysis::loadNegativeWords() {
    ifstream file("negative-words.txt");
    if (!file.is_open()) {
        cout << "Error opening Negative Words File!" << endl;
        return;
    }

    while (negativeWordCount < MAX_WORDS && file >> negativeWords[negativeWordCount]) {
        negativeWordCount++;
    }
    file.close();
}

// Add a review to the review list
void SentimentAnalysis::addReview(const string& reviewText, int rating) {
    if (reviewCount < MAX_REVIEWS) {
        reviewList[reviewCount].review = reviewText; // Safe assignment
        reviewList[reviewCount].rating = rating;
        reviewCount++;
    }
    else {
        cout << "Maximum number of reviews reached!" << endl; // Avoid overflow
    }
}

// Check if a word exists in a list
bool SentimentAnalysis::wordExists(const string& word, const string wordList[], int wordCount) {
    for (int i = 0; i < wordCount; ++i) {
        if (wordList[i] == word) {
            return true;
        }
    }
    return false;
}

// Update word frequency counts
void SentimentAnalysis::updateWordFrequency(const string& word) {
    for (int i = 0; i < uniqueWordCount; i++) {
        if (frequencyWords[i] == word) {
            frequencyCounts[i]++; // Increment count if the word exists
            return;
        }
    }

    // If the word is not found, add it to the frequency lists
    if (uniqueWordCount < MAX_FREQUENCIES) {
        frequencyWords[uniqueWordCount] = word;
        frequencyCounts[uniqueWordCount] = 1; // First occurrence
        uniqueWordCount++;
    }
}

// Analyze all reviews and calculate the sentiment score for each
void SentimentAnalysis::analyzeSentimentReviews() {
    for (int i = 0; i < reviewCount; i++) {
        int positiveCount = 0;
        int negativeCount = 0;

        // Split the review text into words
        stringstream ss(reviewList[i].review);
        string word;

        // Convert words to lowercase for case-insensitive comparison
        while (ss >> word) {
            std::transform(word.begin(), word.end(), word.begin(), ::tolower); // Lowercase transformation

            if (wordExists(word, positiveWords, positiveWordCount)) {
                positiveCount++;
                updateWordFrequency(word); // Track frequency of positive words
            }
            else if (wordExists(word, negativeWords, negativeWordCount)) {
                negativeCount++;
                updateWordFrequency(word); // Track frequency of negative words
            }
        }

        // Calculate the sentiment score
        double sentimentScore = calculateSentimentScore(positiveCount, negativeCount);

        // Output the review sentiment analysis
        cout << "Review: " << reviewList[i].review << endl;
        cout << "Positive words: " << positiveCount << ", Negative words: " << negativeCount << endl;
        cout << "Sentiment Score: " << sentimentScore << endl;
        cout << "-----------------------------------" << endl;
    }
}

// Calculate normalized sentiment score for a review
double SentimentAnalysis::calculateSentimentScore(int positiveCount, int negativeCount) {
    int rawSentimentScore = positiveCount - negativeCount;
    int totalWords = positiveCount + negativeCount;

    // Avoid division by zero if no words are found
    if (totalWords == 0) return 3.0; // Neutral if no words found

    int minRawScore = -totalWords;
    int maxRawScore = totalWords;

    // Normalize the score between 1 and 5
    double normalizedScore = static_cast<double>(rawSentimentScore - minRawScore) / (maxRawScore - minRawScore);
    return 1 + (4 * normalizedScore); // Scale to 1 - 5
}

// Compare sentiment score with user rating
void SentimentAnalysis::compareSentimentWithRating(double sentimentScore, int userRating) {
    int roundedSentimentScore = static_cast<int>(sentimentScore + 0.5); // Round to nearest integer

    if (roundedSentimentScore == userRating) {
        cout << "User's subjective evaluation matches the sentiment score." << endl;
    }
    else {
        cout << "Inconsistency between sentiment score and user rating." << endl;
    }
}

// Overall analysis across all reviews
void SentimentAnalysis::calculateOverallSentiment() {
    int totalPositiveWords = 0;
    int totalNegativeWords = 0;

    for (int i = 0; i < reviewCount; i++) {
        stringstream ss(reviewList[i].review);
        string word;

        while (ss >> word) {
            if (wordExists(word, positiveWords, positiveWordCount)) {
                totalPositiveWords++;
            }
            else if (wordExists(word, negativeWords, negativeWordCount)) {
                totalNegativeWords++;
            }
        }
    }

    // Output overall sentiment statistics
    cout << "Total Reviews: " << reviewCount << endl;
    cout << "Total Positive Words: " << totalPositiveWords << endl;
    cout << "Total Negative Words: " << totalNegativeWords << endl;

    // Output word frequency
    cout << "Word Frequency:\n";
    for (int i = 0; i < uniqueWordCount; i++) {
        cout << frequencyWords[i] << " = " << frequencyCounts[i] << " times\n";
    }
}

// Destructor to clean up dynamically allocated memory
SentimentAnalysis::~SentimentAnalysis() {
    delete[] reviewList; // Free allocated memory for reviews
    cout << "All reviews deleted." << endl;
}
