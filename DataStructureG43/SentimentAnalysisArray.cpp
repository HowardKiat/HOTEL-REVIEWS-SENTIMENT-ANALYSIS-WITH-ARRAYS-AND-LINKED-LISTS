#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <chrono> 
#include "DynamicArray.hpp" // Assuming your custom DynamicArray class exists
#include "WordList.hpp"     // Assuming your custom WordList class exists

using namespace std;
using namespace std::chrono;

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

// Linear search function for checking if a word exists in the DynamicArray
//bool linearSearch(const DynamicArray& wordArray, const std::string& word) {
//    for (size_t i = 0; i < wordArray.getSize(); ++i) {
//        if (wordArray.get(i) == word) {
//            return true; // Word found
//        }
//    }
//    return false; // Word not found
//}

// Binary search for a word in a sorted DynamicArray
bool binarySearch(const DynamicArray& wordArray, const std::string& word) {
    int left = 0;
    int right = wordArray.getSize() - 1;

    while (left <= right) {
        int middle = left + (right - left) / 2;
        if (wordArray.get(middle) == word) {
            return true; // Word found
        }
        if (wordArray.get(middle) < word) {
            left = middle + 1; // Search the right half
        }
        else {
            right = middle - 1; // Search the left half
        }
    }
    return false; // Word not found
}

// Bubble Sort to sort DynamicArray
void bubbleSort(DynamicArray& wordArray) {
    bool swapped;

    do {
        swapped = false;
        for (size_t j = 0; j < wordArray.getSize() - 1; ++j) {
            if (wordArray[j] > wordArray[j + 1]) {
                // Swap the elements using operator[]
                std::string temp = wordArray[j];
                wordArray[j] = wordArray[j + 1];
                wordArray[j + 1] = temp;
                swapped = true; // A swap occurred
            }
        }
    } while (swapped); // Continue as long as swaps are made
}

void collectSentimentWords(const std::string& cleanedReview,
    DynamicArray& positiveWords,
    DynamicArray& negativeWords,
    int& positiveCount,
    int& negativeCount,
    DynamicArray& collectedPositiveWords,
    DynamicArray& collectedNegativeWords) {

    //linear search
    // ----------------------------------------------------------------------------------------

    // Collect positive words using linear search
    //for (size_t k = 0; k < positiveWords.getSize(); ++k) {
    //    if (cleanedReview.find(positiveWords.get(k)) != std::string::npos) {
    //        if (linearSearch(positiveWords, positiveWords.get(k))) {
    //            positiveCount++;
    //            collectedPositiveWords.push_back(positiveWords.get(k));
    //        }
    //    }
    //}

    //// Collect negative words using linear search
    //for (size_t k = 0; k < negativeWords.getSize(); ++k) {
    //    if (cleanedReview.find(negativeWords.get(k)) != std::string::npos) {
    //        if (linearSearch(negativeWords, negativeWords.get(k))) {
    //            negativeCount++;
    //            collectedNegativeWords.push_back(negativeWords.get(k));
    //        }
    //    }
    //}

    // binary search (without sort)
    // ----------------------------------------------------------------------------------------

    // Collect positive words using binary search
    //for (size_t k = 0; k < positiveWords.getSize(); ++k) {
    //    if (cleanedReview.find(positiveWords.get(k)) != std::string::npos) {
    //        if (binarySearch(positiveWords, positiveWords.get(k))) {
    //            positiveCount++;
    //            collectedPositiveWords.push_back(positiveWords.get(k));
    //        }
    //    }
    //}

    //// Collect negative words using binary search
    //for (size_t k = 0; k < negativeWords.getSize(); ++k) {
    //    if (cleanedReview.find(negativeWords.get(k)) != std::string::npos) {
    //        if (binarySearch(negativeWords, negativeWords.get(k))) {
    //            negativeCount++;
    //            collectedNegativeWords.push_back(negativeWords.get(k));
    //        }
    //    }
    //}

    // binary search (with sort)
    // ----------------------------------------------------------------------------------------

    // Sort both arrays before searching
    bubbleSort(positiveWords);
    bubbleSort(negativeWords);

    // Collect positive words using binary search
    for (size_t k = 0; k < positiveWords.getSize(); ++k) {
        if (cleanedReview.find(positiveWords.get(k)) != std::string::npos) {
            if (binarySearch(positiveWords, positiveWords.get(k))) {
                positiveCount++;
                collectedPositiveWords.push_back(positiveWords.get(k));
            }
        }
    }

    // Collect negative words using binary search
    for (size_t k = 0; k < negativeWords.getSize(); ++k) {
        if (cleanedReview.find(negativeWords.get(k)) != std::string::npos) {
            if (binarySearch(negativeWords, negativeWords.get(k))) {
                negativeCount++;
                collectedNegativeWords.push_back(negativeWords.get(k));
            }
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

    // Start the timer
    auto start = high_resolution_clock::now();

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

    // Stop the timer
    auto stop = high_resolution_clock::now();

    // Calculate the duration and print the execution time in seconds
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Time execution for this searching algorithm: " << duration.count() << " milliseconds." << "\n" << endl;

    return 0;
}
