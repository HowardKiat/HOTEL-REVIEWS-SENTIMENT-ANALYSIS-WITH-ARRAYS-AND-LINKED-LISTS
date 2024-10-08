#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include "DynamicArray.hpp" // Assuming your custom DynamicArray class exists
#include "WordList.hpp"     // Assuming your custom WordList class exists

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
    const DynamicArray& positiveWords,
    const DynamicArray& negativeWords,
    int& positiveCount,
    int& negativeCount,
    DynamicArray& collectedPositiveWords,
    DynamicArray& collectedNegativeWords) {

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

void printCollectedWords(const DynamicArray& collectedWords, const std::string& type) {
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
void loadWords(const std::string& filename, DynamicArray& wordArray) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }
    std::string word;
    while (file >> word) {
        wordArray.push_back(word);
    }
}

int main() {
    DynamicArray positiveWords;
    DynamicArray negativeWords;

    loadWords("positive-words.txt", positiveWords);
    loadWords("negative-words.txt", negativeWords);

    std::ifstream reviewFile("tripadvisor_hotel_reviews.csv");
    if (!reviewFile.is_open()) {
        std::cerr << "Failed to open review file." << std::endl;
        return 1; // Exit if the file could not be opened
    }

    std::string line;
    while (std::getline(reviewFile, line)) {
        std::string review;
        std::string ratingStr;

        // Manually split the line without std::stringstream
        splitLine(line, review, ratingStr);

        int userRating;
        try {
            userRating = std::stoi(ratingStr); // Convert rating string to integer
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "Invalid rating format: " << ratingStr << std::endl;
            continue; // Skip this line if rating conversion fails
        }

        toLowercase(review); // Convert to lowercase for better matching
        std::cout << "Analyzing review: " << review << " | User Rating: " << userRating << std::endl;

        int positiveCount = 0, negativeCount = 0;
        DynamicArray collectedPositiveWords;
        DynamicArray collectedNegativeWords;

        collectSentimentWords(review, positiveWords, negativeWords, positiveCount, negativeCount, collectedPositiveWords, collectedNegativeWords);
        printCollectedWords(collectedPositiveWords, "Positive");
        printCollectedWords(collectedNegativeWords, "Negative");

        // Calculate the normalized sentiment score
        double sentimentScore = calculateSentimentScore(positiveCount, negativeCount);
        std::cout << "Sentiment score (1-5): " << sentimentScore << std::endl;

        // Compare the sentiment score with the user rating
        if ((sentimentScore > 3 && userRating < 3) || (sentimentScore < 3 && userRating > 3)) {
            std::cout << "User's subjective evaluation does not match the sentiment score.\n" << std::endl;
        }
        else {
            std::cout << "User's subjective evaluation matches the sentiment score.\n" << std::endl;
        }
    }

    return 0;
}
