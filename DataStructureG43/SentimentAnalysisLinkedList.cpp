// SentimentAnalysisLinkedList.cpp

#include "Sentiment_Analysis_LinkedList.hpp"
#include <sstream>
#include <cctype>

using namespace std;

// Constructor
SentimentAnalysisLinkedList::SentimentAnalysisLinkedList()
    : positiveWordCount(0), negativeWordCount(0),
    reviewHead(nullptr), reviewCount(0),
    frequencyHead(nullptr), individualWordCount(0) {}

// Load positive words
void SentimentAnalysisLinkedList::loadPositiveWords(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening Positive Words File: " << filename << "!" << endl;
        return;
    }

    string word;
    while (positiveWordCount < MAX_WORDS && file >> word) {
        positiveWords[positiveWordCount++] = word;
    }
    file.close();
}

// Load negative words
void SentimentAnalysisLinkedList::loadNegativeWords(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening Negative Words File: " << filename << "!" << endl;
        return;
    }

    string word;
    while (negativeWordCount < MAX_WORDS && file >> word) {
        negativeWords[negativeWordCount++] = word;
    }
    file.close();
}

// Load reviews from CSV
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

        if (line.empty()) continue; // Skip empty lines

        if (line[0] == '"') {
            size_t endQuotePos = line.find_last_of('"');
            if (endQuotePos == string::npos || endQuotePos == 0) {
                cout << "Malformed line (quotes not closed): " << line << endl;
                continue;
            }
            reviewText = line.substr(1, endQuotePos - 1);
            size_t commaPos = line.find(',', endQuotePos);
            if (commaPos == string::npos) {
                cout << "Malformed line (comma missing after quotes): " << line << endl;
                continue;
            }
            string ratingStr = line.substr(commaPos + 1);
            try {
                rating = stoi(ratingStr);
            }
            catch (invalid_argument&) {
                cout << "Invalid rating value: " << ratingStr << " in line: " << line << endl;
                continue;
            }
        }
        else {
            stringstream ss(line);
            if (!getline(ss, reviewText, ',')) {
                cout << "Malformed line (unable to extract review text): " << line << endl;
                continue;
            }
            if (!(ss >> rating)) {
                cout << "Invalid rating in line: " << line << endl;
                continue;
            }
        }

        addReview(reviewText, rating);
    }

    file.close();
}

// Add a review
void SentimentAnalysisLinkedList::addReview(const string& reviewText, int rating) {
    ReviewNode* newReview = new ReviewNode(reviewText, rating);
    if (!reviewHead) {
        reviewHead = newReview;
    }
    else {
        ReviewNode* temp = reviewHead;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = newReview;
    }
    reviewCount++;
}

// Convert to lowercase (Manual implementation without std::transform)
void SentimentAnalysisLinkedList::toLowercase(string& word) {
    for (size_t i = 0; i < word.length(); ++i) {
        if (word[i] >= 'A' && word[i] <= 'Z') {
            word[i] = word[i] + ('a' - 'A');
        }
    }
}

// Remove punctuation (Manual implementation without std::remove_if)
void SentimentAnalysisLinkedList::removePunctuation(string& word) {
    string cleanedWord;
    for (size_t i = 0; i < word.length(); ++i) {
        if (isalpha(word[i]) || word[i] == '\'') {
            cleanedWord += word[i];
        }
    }
    word = cleanedWord;
}

// Check if word exists in list
bool SentimentAnalysisLinkedList::wordExists(const string& word, const string wordList[], int wordCount) {
    for (int i = 0; i < wordCount; ++i) {
        if (wordList[i] == word) {
            return true;
        }
    }
    return false;
}

// Update word frequency
void SentimentAnalysisLinkedList::updateWordFrequency(const string& word) {
    WordFrequencyNode* temp = frequencyHead;
    WordFrequencyNode* prev = nullptr;

    while (temp) {
        if (temp->word == word) {
            temp->count++;
            return;
        }
        prev = temp;
        temp = temp->next;
    }

    // Word not found, add new node
    WordFrequencyNode* newWord = new WordFrequencyNode(word);
    if (!frequencyHead) {
        frequencyHead = newWord;
    }
    else {
        prev->next = newWord;
    }
    individualWordCount++;
}

// Analyze sentiment
void SentimentAnalysisLinkedList::analyzeSentimentReviews() {
    ReviewNode* current = reviewHead;
    int index = 1;
    while (current) {
        int positiveCount = 0;
        int negativeCount = 0;

        stringstream ss(current->reviewText);
        string word;

        while (ss >> word) {
            toLowercase(word);
            removePunctuation(word);

            if (wordExists(word, positiveWords, positiveWordCount)) {
                positiveCount++;
                updateWordFrequency(word);
            }
            else if (wordExists(word, negativeWords, negativeWordCount)) {
                negativeCount++;
                updateWordFrequency(word);
            }
        }

        double sentimentScore = calculateSentimentScore(positiveCount, negativeCount);
        compareSentimentWithRating(sentimentScore, current->rating);

        // Display review analysis
        cout << "Review " << index++ << ":" << endl;
        cout << "Text: " << current->reviewText << endl;
        cout << "Rating: " << current->rating << endl;
        cout << "Positive words: " << positiveCount << ", Negative words: " << negativeCount << endl;
        cout << "Sentiment Score: " << sentimentScore << endl;
        cout << "-----------------------------------" << endl;

        current = current->next;
    }
}

// Calculate sentiment score
double SentimentAnalysisLinkedList::calculateSentimentScore(int positiveCount, int negativeCount) {
    int rawSentimentScore = positiveCount - negativeCount;
    int totalWords = positiveCount + negativeCount;

    if (totalWords == 0) return 3.0; // Neutral

    int minRawScore = -totalWords;
    int maxRawScore = totalWords;

    double normalizedScore = static_cast<double>(rawSentimentScore - minRawScore) / (maxRawScore - minRawScore);
    return 1 + (4 * normalizedScore); // Scale to [1,5]
}

// Compare sentiment score with user rating
void SentimentAnalysisLinkedList::compareSentimentWithRating(double sentimentScore, int userRating) {
    int roundedScore = static_cast<int>(sentimentScore + 0.5);

    if (roundedScore == userRating) {
        cout << "User's subjective evaluation matches the sentiment score." << endl;
    }
    else {
        cout << "User's subjective evaluation does not match the sentiment score. "
            << "Sentiment Score: " << roundedScore << ", User Rating: " << userRating << endl;
    }
}

// Calculate overall sentiment
void SentimentAnalysisLinkedList::calculateOverallSentiment() {
    int totalPositive = 0;
    int totalNegative = 0;

    ReviewNode* current = reviewHead;
    while (current) {
        stringstream ss(current->reviewText);
        string word;

        while (ss >> word) {
            toLowercase(word);
            removePunctuation(word);

            if (wordExists(word, positiveWords, positiveWordCount)) {
                totalPositive++;
            }
            else if (wordExists(word, negativeWords, negativeWordCount)) {
                totalNegative++;
            }
        }

        current = current->next;
    }

    cout << "Overall Sentiment: " << endl;
    cout << "Total Positive Words: " << totalPositive << endl;
    cout << "Total Negative Words: " << totalNegative << endl;

    SentimentAnalysisLinkedList()::~SentimentAnalysisLinkedList()
    {

    }
}

