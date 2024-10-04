#include "SentimentAnalysisLinkedList.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>

using namespace std;

// Constructor to initialize member variables
SentimentAnalysisLinkedList::SentimentAnalysisLinkedList()
    : reviewHead(nullptr), reviewCount(0), positiveWordCount(0), negativeWordCount(0), individualWordCount(0) {
    // Initialize frequency counts to zero
    for (int i = 0; i < MAX_FREQUENCIES; ++i) {
        trackWordFrequencyCounts[i] = 0; // Initialize all counts to zero
        trackWordsFrequency[i] = "";  // Initialize all words frequency to an empty string
    }

    // Dynamically allocate arrays for positive and negative words
    positiveWords = new string[MAX_WORDS];
    negativeWords = new string[MAX_WORDS];
}

// Load positive words from a text file
void SentimentAnalysisLinkedList::loadPositiveWords(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening Positive Words File: " << filename << endl;
        return;
    }

    while (positiveWordCount < MAX_WORDS && file >> positiveWords[positiveWordCount]) {
        positiveWordCount++;
    }
    file.close();
}

void SentimentAnalysisLinkedList::loadNegativeWords(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening Negative Words File: " << filename << endl;
        return;
    }

    while (negativeWordCount < MAX_WORDS && file >> negativeWords[negativeWordCount]) {
        negativeWordCount++;
    }
    file.close();
}


// Load CSV File
void SentimentAnalysisLinkedList::loadCSV(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file '" << filename << "'" << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        string reviewText;
        int rating;

        if (line[0] == '"') {
            size_t endQuotePos = line.find_last_of('"');
            reviewText = line.substr(1, endQuotePos - 1);
            string ratingStr = line.substr(endQuotePos + 2);
            rating = stoi(ratingStr);
        }
        else {
            stringstream ss(line);
            getline(ss, reviewText, ',');
            ss >> rating;
        }

        addReview(reviewText, rating);
    }

    file.close();
}

// Add a review to the review list (Linked List)
void SentimentAnalysisLinkedList::addReview(const string& reviewText, int rating) {
    ReviewNode* newNode = new ReviewNode(reviewText, rating);
    if (reviewHead == nullptr) {
        reviewHead = newNode; // First review
    }
    else {
        ReviewNode* current = reviewHead;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode; // Add at the end
    }
    reviewCount++;
}

// Convert each character of the word to lowercase
void SentimentAnalysisLinkedList::toLowercase(string& word) {
    for (char& character : word) {
        character = tolower(character);
    }
}

// Removing Punctuations
void SentimentAnalysisLinkedList::removePunctuation(string& word) {
    word.erase(remove_if(word.begin(), word.end(), [](char character) {
        // ASCII range
        if (character < 0 || character > 255) {
            return true;
        }
        return !(isalpha(character) || character == '\'');
        }), word.end());
}

// Check if a word exists in a list
bool SentimentAnalysisLinkedList::wordExists(const string& word, const string wordList[], int wordCount) {
    for (int i = 0; i < wordCount; ++i) {
        if (wordList[i] == word) {
            return true;
        }
    }
    return false;
}

// Update the count of the word frequency
void SentimentAnalysisLinkedList::updateWordFrequency(const string& word) {
    for (int i = 0; i < individualWordCount; i++) {
        if (trackWordsFrequency[i] == word) {
            trackWordFrequencyCounts[i]++;
            return;
        }
    }

    if (individualWordCount < MAX_FREQUENCIES) {
        trackWordsFrequency[individualWordCount] = word;
        trackWordFrequencyCounts[individualWordCount] = 1;
        individualWordCount++;
    }
}

// Analyze all reviews and calculate the sentiment score for each review
void SentimentAnalysisLinkedList::analyzeSentimentReviews() {
    ReviewNode* current = reviewHead;
    while (current != nullptr) {
        int positiveCount = 0;
        int negativeCount = 0;

        // Split text into individual words
        stringstream ss(current->reviewText);
        string word;

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

        // Output of the review sentiment analysis
        cout << "Review: " << current->reviewText << endl;
        cout << "Positive words: " << positiveCount << ", Negative words: " << negativeCount << endl;
        cout << "Sentiment Score: " << sentimentScore << endl;
        cout << "-----------------------------------" << endl;

        current = current->next; // Move to the next review
    }
}

// Calculate sentiment score for a review
double SentimentAnalysisLinkedList::calculateSentimentScore(int positiveCount, int negativeCount) {
    int rawSentimentScore = positiveCount - negativeCount;
    int totalWords = positiveCount + negativeCount;

    if (totalWords == 0) return 3.0;

    int minRawScore = -totalWords;
    int maxRawScore = totalWords;

    double normalizedScore = static_cast<double>(rawSentimentScore - minRawScore) / (maxRawScore - minRawScore);
    return 1 + (4 * normalizedScore);
}

// Overall analysis across all of the reviews
void SentimentAnalysisLinkedList::calculateOverallSentiment() {
    int totalPositiveWords = 0;
    int totalNegativeWords = 0;

    ReviewNode* current = reviewHead;
    while (current != nullptr) {
        stringstream ss(current->reviewText);
        string word;

        while (ss >> word) {
            if (wordExists(word, positiveWords, positiveWordCount)) {
                totalPositiveWords++;
            }
            else if (wordExists(word, negativeWords, negativeWordCount)) {
                totalNegativeWords++;
            }
        }
        current = current->next; // Move to the next review
    }

    cout << "Total Reviews: " << reviewCount << endl;
    cout << "Total Positive Words: " << totalPositiveWords << endl;
    cout << "Total Negative Words: " << totalNegativeWords << endl;

    cout << "Word Frequency:\n";
    for (int i = 0; i < individualWordCount; i++) {
        cout << trackWordsFrequency[i] << " = " << trackWordFrequencyCounts[i] << " times\n";
    }
}

// Destructor
SentimentAnalysisLinkedList::~SentimentAnalysisLinkedList() {
    // Free review nodes
    while (reviewHead) {
        ReviewNode* temp = reviewHead;
        reviewHead = reviewHead->next;
        delete temp;
    }

    // Free dynamically allocated word arrays
    delete[] positiveWords;
    delete[] negativeWords;
}
