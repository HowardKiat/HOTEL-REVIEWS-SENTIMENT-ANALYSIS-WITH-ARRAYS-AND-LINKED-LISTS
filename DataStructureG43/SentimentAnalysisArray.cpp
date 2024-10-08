#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <stdexcept>
#include <chrono> // For measuring time
#include "DynamicArray.hpp" // Assuming your custom DynamicArray class exists
#include "WordList.hpp"     // Assuming your custom WordList class exists
#include "WordFrequency.hpp" // Include the custom WordFrequency class

using namespace std;

void toLowercase(std::string& str) {
    for (char& c : str) {
        c = tolower(c);
    }
}

// Manual splitting of CSV without using std::stringstream
void splitLine(const std::string& line, std::string& review, std::string& ratingStr) {
    size_t pos = line.find_last_of(',');
    if (pos != std::string::npos) {
        review = line.substr(0, pos);
        ratingStr = line.substr(pos + 1);
    }
}

void collectSentimentWords(const std::string& cleanedReview,
    const DynamicArray<std::string>& positiveWords, // Correct template usage
    const DynamicArray<std::string>& negativeWords, // Correct template usage
    int& positiveCount,
    int& negativeCount,
    DynamicArray<std::string>& collectedPositiveWords, // Correct template usage
    DynamicArray<std::string>& collectedNegativeWords) { // Correct template usage

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

void printCollectedWords(const DynamicArray<std::string>& collectedWords, const std::string& type) {
    std::cout << type << " Words = " << collectedWords.getSize() << ":\n";
    for (size_t i = 0; i < collectedWords.getSize(); ++i) {
        std::cout << "-" << collectedWords.get(i) << "\n";
    }
    std::cout << std::endl;
}

// Function to calculate the normalized sentiment score (1-5 scale)
double calculateSentimentScore(int positiveCount, int negativeCount) {
    int rawScore = positiveCount - negativeCount;
    int N = positiveCount + negativeCount; // Total number of positive and negative words

    if (N == 0) {
        return 3.0; // Neutral score if no words are found
    }

    int minRawScore = -N;
    int maxRawScore = N;

    // Normalized score formula
    double normalizedScore = (rawScore - minRawScore) / static_cast<double>(maxRawScore - minRawScore);
    double sentimentScore = 1 + (4 * normalizedScore);

    return sentimentScore;
}

// Function to load words from a file
void loadWords(const std::string& filename, DynamicArray<std::string>& wordArray) { // Correct template usage
    auto start = std::chrono::high_resolution_clock::now();

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string word;
    while (file >> word) {
        wordArray.push_back(word);
        std::cout << "Loaded word: " << word << std::endl; // Print loaded words
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Time taken to load words from " << filename << ": " << elapsed.count() << " seconds.\n";
}

int main() {
    DynamicArray<std::string> positiveWords; // Correct template usage
    DynamicArray<std::string> negativeWords; // Correct template usage

    std::cout << "Loading positive words...\n";
    loadWords("positive-words.txt", positiveWords);

    std::cout << "Loading negative words...\n";
    loadWords("negative-words.txt", negativeWords);

    std::ifstream reviewFile("tripadvisor_hotel_reviews.csv");
    if (!reviewFile.is_open()) {
        std::cerr << "Failed to open review file." << std::endl;
        return 1; // Exit if the file could not be opened
    }

    std::string line;
    int totalPositiveWords = 0, totalNegativeWords = 0, totalReviews = 0;
    WordFrequency wordFrequency; // Use the custom word frequency class

    while (std::getline(reviewFile, line)) {
        std::string review;
        std::string ratingStr;

        // Manually split the line without std::stringstream
        splitLine(line, review, ratingStr);

        int userRating;
        try {
            userRating = std::stoi(ratingStr); // Convert rating string to integer
        }
        catch (const std::invalid_argument&) {
            std::cerr << "Invalid rating format: " << ratingStr << std::endl;
            continue; // Skip this line if rating conversion fails
        }

        toLowercase(review); // Convert to lowercase for better matching
        std::cout << "Analyzing review: " << review << " | User Rating: " << userRating << std::endl;

        int positiveCount = 0, negativeCount = 0;
        DynamicArray<std::string> collectedPositiveWords; // Correct template usage
        DynamicArray<std::string> collectedNegativeWords; // Correct template usage

        collectSentimentWords(review, positiveWords, negativeWords, positiveCount, negativeCount, collectedPositiveWords, collectedNegativeWords);

        printCollectedWords(collectedPositiveWords, "Positive");
        printCollectedWords(collectedNegativeWords, "Negative");

        // Add counts to the WordFrequency object
        for (size_t k = 0; k < collectedPositiveWords.getSize(); ++k) {
            wordFrequency.addWord(collectedPositiveWords.get(k));
        }

        for (size_t k = 0; k < collectedNegativeWords.getSize(); ++k) {
            wordFrequency.addWord(collectedNegativeWords.get(k));
        }

        // Calculate the normalized sentiment score
        double sentimentScore = calculateSentimentScore(positiveCount, negativeCount);
        std::cout << "Sentiment score (1 - 5): " << sentimentScore << std::endl;

        // Compare the sentiment score with the user rating
        if ((sentimentScore > 3 && userRating < 3) || (sentimentScore < 3 && userRating > 3)) {
            std::cout << "User's subjective evaluation does not match the sentiment score.\n" << std::endl;
        }
        else {
            std::cout << "User's subjective evaluation matches the sentiment score.\n" << std::endl;
        }

        // Update overall counts
        totalPositiveWords += positiveCount;
        totalNegativeWords += negativeCount;
        totalReviews++;
    }

    // Print overall sentiment statistics
    std::cout << "-----------------------------------\n";
    std::cout << "Review Analysis Summary:\n";
    std::cout << "-----------------------------------\n";
    std::cout << "Total Reviews: " << totalReviews << "\n";
    std::cout << "Total Positive Words: " << totalPositiveWords << "\n";
    std::cout << "Total Negative Words: " << totalNegativeWords << "\n";
    std::cout << "-----------------------------------\n";

    // Frequency of each word
    std::cout << "Frequency of each word used in overall reviews:\n";
    for (int i = 0; i < wordFrequency.getSize(); ++i) {
        std::cout << wordFrequency.getWord(i) << ": " << wordFrequency.getCount(wordFrequency.getWord(i)) << std::endl;
    }

    // Find and print maximum frequency
    int maxFrequency = wordFrequency.findMaxFrequency();
    std::cout << "Maximum Frequency: " << maxFrequency << std::endl;

    // Find and print minimum frequency
    int minFrequency = wordFrequency.findMinFrequency();
    std::cout << "Minimum Frequency: " << minFrequency << std::endl;

    return 0; // Return success
}