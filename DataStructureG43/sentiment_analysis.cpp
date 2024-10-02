#include "sentiment_analysis.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>

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

// Load CSV File
void SentimentAnalysis::loadCSV(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file '" << filename << "'" << endl;
        return;
    }

    string line;
    while (getline(file, line) && reviewCount < MAX_REVIEWS) {
        stringstream ss(line);
        string reviewText;
        int rating;

        if (line[0] == '"') {
            size_t endQuotePos = line.find_last_of('"');
            reviewText = line.substr(1, endQuotePos - 1);
            string ratingStr = line.substr(endQuotePos + 2);
            rating = stoi(ratingStr);
        }
        else {
            getline(ss, reviewText, ',');
            ss >> rating;
        }

        addReview(reviewText, rating);
    }

    file.close();
}

// Add a review to the review list
void SentimentAnalysis::addReview(const string& reviewText, int rating) {
    if (reviewCount < MAX_REVIEWS) {
        reviewList[reviewCount].review = reviewText;
        reviewList[reviewCount].rating = rating;
        reviewCount++;
    }
    else {
        cout << "Maximum number of reviews reached!" << endl;
    }
}

// Manually convert each character of the word to lowercase
void SentimentAnalysis::toLowercase(string& word) {
    for (char& ch : word) {
        ch = tolower(ch);
    }
}

// Removing Punctuations/Symbols
void SentimentAnalysis::removePunctuation(string& word) {
    word.erase(remove_if(word.begin(), word.end(), [](char ch) {
        // Ensure the character is in the valid ASCII range
        if (ch < 0 || ch > 255) {
            return true; // Remove invalid characters
        }
        return !(isalpha(ch) || ch == '\''); // Keep only alphabetic characters and apostrophes
        }), word.end());
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
            frequencyCounts[i]++;
            return;
        }
    }

    if (uniqueWordCount < MAX_FREQUENCIES) {
        frequencyWords[uniqueWordCount] = word;
        frequencyCounts[uniqueWordCount] = 1;
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

        // Process each word in the review
        while (ss >> word) {
            toLowercase(word);         // Convert to lowercase
            removePunctuation(word);   // Remove punctuation

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

    if (totalWords == 0) return 3.0;

    int minRawScore = -totalWords;
    int maxRawScore = totalWords;

    double normalizedScore = static_cast<double>(rawSentimentScore - minRawScore) / (maxRawScore - minRawScore);
    return 1 + (4 * normalizedScore);
}

// Compare sentiment score with user rating
void SentimentAnalysis::compareSentimentWithRating(double sentimentScore, int userRating) {
    int roundedSentimentScore = static_cast<int>(sentimentScore + 0.5);

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

    cout << "Total Reviews: " << reviewCount << endl;
    cout << "Total Positive Words: " << totalPositiveWords << endl;
    cout << "Total Negative Words: " << totalNegativeWords << endl;

    cout << "Word Frequency:\n";
    for (int i = 0; i < uniqueWordCount; i++) {
        cout << frequencyWords[i] << " = " << frequencyCounts[i] << " times\n";
    }
}

// Destructor to clean up allocated memory
SentimentAnalysis::~SentimentAnalysis() {
    delete[] reviewList; // Free allocated memory for reviews
}
