#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include "DynamicArray.hpp" // Assuming your custom DynamicArray class exists
#include "WordList.hpp"     // Assuming your custom WordList class exists

using namespace std;

// Load words from a file into the DynamicArray
void loadWords(const std::string& filename, DynamicArray& wordArray) {
    auto start = std::chrono::high_resolution_clock::now(); // Start timing
    std::ifstream file(filename);
    if (!file.is_open()) { // Check if the file opens successfully
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string word;
    while (file >> word) {
        wordArray.push_back(word); // Ensure push_back is defined in DynamicArray
        std::cout << "Loaded word: " << word << std::endl; // Debug output
    }

    // Manual splitting of CSV without using std::stringstream
    void splitLine(const std::string & line, std::string & review, std::string & ratingStr) {
        size_t pos = line.find_last_of(',');
        if (pos != std::string::npos) {
            review = line.substr(0, pos);
            ratingStr = line.substr(pos + 1);
        }
    }

    void collectSentimentWords(const std::string & cleanedReview,
        const DynamicArray & positiveWords,
        const DynamicArray & negativeWords,
        int& positiveCount,
        int& negativeCount,
        DynamicArray & collectedPositiveWords,
        DynamicArray & collectedNegativeWords) {

        for (size_t k = 0; k < positiveWords.getSize(); ++k) {
            if (cleanedReview.find(positiveWords.get(k)) != std::string::npos) {
                positiveCount++;
                collectedPositiveWords.push_back(positiveWords.get(k));
            }
        }

        for (size_t k = 0; k < negativeWords.getSize(); ++k) {
            if (cleanedReview.find(negativeWords.get(k)) != std::string::npos) {
                negativeCount++;
                collectedNegativeWords.push_back(negativeWords.get(k));
            }
        }
    }

    // Return the raw sentiment score (positive count - negative count)
    return positiveCount - negativeCount;
}

// Calculate sentiment score based on raw sentiment score
double calculateSentimentScore(double rawScore, int totalCount) {
    if (totalCount == 0) return 0.0; // Prevent division by zero
    double minRawScore = -totalCount;
    double maxRawScore = totalCount;
    double normalizedScore = (rawScore - minRawScore) / (maxRawScore - minRawScore);
    return 1 + (4 * normalizedScore); // Score from 1 to 5
}

// Analyze the sentiment of a review and compare it with the user's rating
void analyzeReview(const std::string& review, const std::string& userRatingStr,
    const DynamicArray& positiveWords, const DynamicArray& negativeWords, WordList& wordList) {

    // Analyze the raw sentiment score
    double rawSentimentScore = analyzeSentiment(review, positiveWords, negativeWords, wordList);
    double sentimentScore = calculateSentimentScore(rawSentimentScore, positiveWords.getSize() + negativeWords.getSize());

    std::string sentimentCategory; // Declare sentimentCategory here

    // Categorize the sentiment score
    if (sentimentScore >= 4.0) {
        sentimentCategory = "Positive"; // 4.0 to 5.0
    }
    else if (sentimentScore >= 3.0) {
        sentimentCategory = "Neutral"; // 3.0 to 3.9
    }
    else {
        sentimentCategory = "Negative"; // 1.0 to 2.9
    }

    try {
        int userRating = std::stoi(userRatingStr); // Convert user rating to int
        std::cout << "Analyzing review: " << review << " | User Rating: " << userRatingStr << std::endl; // Debug output

        std::cout << "Sentiment score (1 - 5) is " << sentimentScore << ", thus the rating should be equal to "
            << static_cast<int>(sentimentScore) << " (" << sentimentCategory << ")." << std::endl;

        std::cout << "Sentiment category: " << sentimentCategory << std::endl;

        // Compare sentiment score with user rating
        if (static_cast<int>(sentimentScore) == userRating) {
            std::cout << "User's subjective evaluation matches the sentiment score provided by the analysis." << std::endl;
        }
        else {
            std::cout << "User's subjective evaluation does not match the sentiment score provided by the analysis." << std::endl;
        }
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Invalid user rating: " << userRatingStr << std::endl;
    }
    catch (const std::out_of_range& e) {
        std::cerr << "User rating out of range: " << userRatingStr << std::endl;
    }

    std::cout << std::endl; // For better readability
}

// Update word frequencies from a review
void updateWordFrequencies(const std::string& review, const DynamicArray& positiveWords, const DynamicArray& negativeWords, WordList& wordList) {
    std::istringstream reviewStream(review);
    std::string word;

    while (reviewStream >> word) {
        // Check if the word is in the positive or negative word lists
        bool isPositive = positiveWords.contains(word); // Assuming contains() is implemented in DynamicArray
        bool isNegative = negativeWords.contains(word); // Assuming contains() is implemented in DynamicArray

        // Only add to wordList if the word is positive or negative
        if (isPositive || isNegative) {
            wordList.addWord(word); // Ensure addWord is implemented in WordList
        }
    }
}

// Extra Features
// Convert Characters to lowercase
void toLowercase(std::string& word)
{
    for (char& c : word) {
        if (c >= 'A' && c <= 'Z') { // Check if the character is uppercase
            c = c + 32; // Convert uppercase to lowercase by adding 32 (ASCII value difference)
        }
    }
}

// Removing Punctuations
void removePunctuation(std::string& word) {
    std::string cleanWord; // Temporary string to store the cleaned word

    for (char c : word) {
        if (isalpha(c)) { // Check if the character is alphabetic
            cleanWord += c; // Only add alphabetic characters to cleanWord
        }
    }

    word = cleanWord; // Replace the original word with the cleaned version
}

// Main function
int main() {
    DynamicArray positiveWords;
    DynamicArray negativeWords;

    // Load positive and negative words
    loadWords("positive-words.txt", positiveWords);
    loadWords("negative-words.txt", negativeWords);

    auto startReviews = std::chrono::high_resolution_clock::now(); // Start timing reviews
    std::ifstream reviewFile("tripadvisor_hotel_reviews.csv");
    std::string line;
    WordList wordList; // For tracking word frequencies

    std::string line;
    while (std::getline(reviewFile, line)) {
        // Check for empty lines
        if (line.empty()) {
            std::cerr << "Empty line in review file." << std::endl; // Debug output
            continue; // Skip empty lines
        }

        // Split by the last comma to separate review and rating
        size_t lastCommaPos = line.rfind(',');
        if (lastCommaPos != std::string::npos) {
            // Extract user rating (after the last comma) and review text
            std::string userRatingStr = line.substr(lastCommaPos + 1); // Extract user rating
            std::string review = line.substr(0, lastCommaPos); // Extract review text

            // Trim leading and trailing whitespace from both review and user rating
            review.erase(0, review.find_first_not_of(" \t")); // Ltrim
            userRatingStr.erase(0, userRatingStr.find_first_not_of(" \t")); // Ltrim

            // Analyze the review
            analyzeReview(review, userRatingStr, positiveWords, negativeWords, wordList);
        }
    }

    return 0;
}
