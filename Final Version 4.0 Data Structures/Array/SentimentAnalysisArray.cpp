#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <stdexcept>
#include <chrono> // For measuring time
#include <cmath> // for sqrt()
#include "DynamicArray.hpp" // Assuming your custom DynamicArray class exists
#include "WordList.hpp"     // Assuming your custom WordList class exists
#include "WordFrequency.hpp" // Include the custom WordFrequency class

using namespace std;
using namespace std::chrono;

void toLowercase(string& str) {
    for (char& c : str) {
        c = tolower(c);
    }
}

// Manual splitting of CSV
void splitLine(const string& line, string& review, string& ratingStr) {
    size_t pos = line.find_last_of(',');
    if (pos != string::npos) {
        review = line.substr(0, pos);
        ratingStr = line.substr(pos + 1);
    }
}

// Jump search function for checking if a word exists in the DynamicArray
bool jumpSearch(const DynamicArray<string>& wordArray, const string& word) {
    size_t n = wordArray.getSize();

    if (n == 0) {
        return false; // Empty array, word not found
    }

    // Calculate the optimal jump step (sqrt of the array size)
    size_t step = sqrt(n);
    size_t prev = 0;

    // Jump ahead by step size until we find an interval where the element may exist
    while (wordArray.get(min(step, n) - 1) < word) {
        prev = step;
        step += sqrt(n);
        if (prev >= n) {
            return false; // Word not found
        }
    }

    // Perform a linear search in the block defined by [prev, step)
    for (size_t i = prev; i < min(step, n); ++i) {
        if (wordArray.get(i) == word) {
            return true; // Word found
        }
    }

    return false; // Word not found
}

// Binary search for a word in a sorted DynamicArray
bool binarySearch(const DynamicArray<string>& wordArray, const string& word) {
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

// Function to collect sentiment words using the chosen search method
void collectSentimentWords(const string& cleanedReview,
    const DynamicArray<string>& positiveWords,
    const DynamicArray<string>& negativeWords,
    int& positiveCount,
    int& negativeCount,
    DynamicArray<string>& collectedPositiveWords,
    DynamicArray<string>& collectedNegativeWords,
    int searchChoice) {

    string word;
    size_t start = 0;

    // Tokenize the cleanedReview string to separate words
    while (start < cleanedReview.length()) {
        // Find the next space or punctuation
        size_t end = cleanedReview.find_first_of(" ,.!?;:", start);

        // If no more spaces, take the rest of the string
        if (end == string::npos) {
            end = cleanedReview.length();
        }

        // Extract the word
        word = cleanedReview.substr(start, end - start);

        // Check for hyphenated words (e.g., "non-existent")
        if (word.find('-') != string::npos) {
            // Add the hyphenated word directly
            if ((searchChoice == 1 && binarySearch(negativeWords, word)) ||
                (searchChoice == 2 && jumpSearch(negativeWords, word))) {
                bool alreadyExists = false;
                for (size_t i = 0; i < collectedNegativeWords.getSize(); ++i) {
                    if (collectedNegativeWords.get(i) == word) {
                        alreadyExists = true;
                        break;
                    }
                }
                if (!alreadyExists) {
                    negativeCount++;
                    collectedNegativeWords.push_back(word);
                }
            }
        }
        else {
            // Normal word processing
            bool isPositive = (searchChoice == 1 && binarySearch(positiveWords, word)) ||
                (searchChoice == 2 && jumpSearch(positiveWords, word));

            bool isNegative = (searchChoice == 1 && binarySearch(negativeWords, word)) ||
                (searchChoice == 2 && jumpSearch(negativeWords, word));

            // Only add unique words to the collected arrays
            if (isPositive) {
                bool alreadyExists = false;
                for (size_t i = 0; i < collectedPositiveWords.getSize(); ++i) {
                    if (collectedPositiveWords.get(i) == word) {
                        alreadyExists = true;
                        break;
                    }
                }
                if (!alreadyExists) {
                    positiveCount++;
                    collectedPositiveWords.push_back(word);
                }
            }

            if (isNegative) {
                bool alreadyExists = false;
                for (size_t i = 0; i < collectedNegativeWords.getSize(); ++i) {
                    if (collectedNegativeWords.get(i) == word) {
                        alreadyExists = true;
                        break;
                    }
                }
                if (!alreadyExists) {
                    negativeCount++;
                    collectedNegativeWords.push_back(word);
                }
            }
        }

        start = end + 1; // Move to the next word
    }
}

void printCollectedWords(const DynamicArray<string>& collectedWords, const string& type) {
    std::cout << type << " Words = " << collectedWords.getSize() << ":\n";
    for (size_t i = 0; i < collectedWords.getSize(); ++i) {
        std::cout << "-" << collectedWords.get(i) << "\n";
    }
    cout << endl;
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
void loadWords(const string& filename, DynamicArray<string>& wordArray) { // Correct template usage
    auto start = chrono::high_resolution_clock::now();

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        return;
    }

    string word;
    while (file >> word) {
        wordArray.push_back(word);
        cout << "Loaded word: " << word << endl; // Print loaded words
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Time taken to load words from " << filename << ": " << elapsed.count() << " seconds.\n";
}

void searchForWordFrequency(const WordFrequency& wordFrequency) {
    string searchWord;
    cout << "Enter the word you want to search for: ";
    cin >> searchWord;
    toLowercase(searchWord);

    int frequency = wordFrequency.getCount(searchWord);
    if (frequency > 0) {
        cout << "The word '" << searchWord << "' appears " << frequency << " times in the reviews.\n";
    }
    else {
        cout << "The word '" << searchWord << "' does not appear in the reviews.\n";
    }
}

int getSearchChoice() {
    int choice;
    do {
        cout << "Choose searching method:\n1. Binary Search\n2. Jump Search\nEnter your choice (1 or 2): ";
        cin >> choice;
        if (choice != 1 && choice != 2) {
            cerr << "Invalid choice! Please enter 1 or 2." << endl;
        }
    } while (choice != 1 && choice != 2);
    return choice;
}

int getSortChoice() {
    int choice;
    do {
        cout << "Choose sorting method:\n1. Merge Sort\n2. Quick Sort\n3. Exit\nEnter your choice (1, 2, or 3): ";
        cin >> choice;
        if (choice < 1 || choice > 3) {
            cerr << "Invalid choice! Please enter 1, 2, or 3." << endl;
        }
    } while (choice < 1 || choice > 3);
    return choice;
}

void performSortAndPrintFrequencies(WordFrequency& wordFrequency, int sortChoice) {
    auto start = high_resolution_clock::now();

    if (sortChoice == 1) {
        wordFrequency.mergeSortFrequencies();
        cout << "Frequency of each word used in overall reviews (after Merge Sort):\n";
    }
    else {
        wordFrequency.quickSortFrequencies();
        cout << "Frequency of each word used in overall reviews (after Quick Sort):\n";
    }

    auto end = high_resolution_clock::now();
    duration<double> sortDuration = end - start;

    for (size_t i = 0; i < wordFrequency.getSize(); ++i) {
        cout << wordFrequency.getWord(i) << ": " << wordFrequency.getCount(wordFrequency.getWord(i)) << endl;
    }

    cout << (sortChoice == 1 ? "Merge" : "Quick") << " sort execution time: "
        << sortDuration.count() << " seconds" << endl;
}

bool getUserChoice(const string& prompt) {
    char choice;
    do {
        cout << prompt;
        cin >> choice;
        choice = tolower(choice);
        if (choice != 'y' && choice != 'n') {
            cerr << "Invalid input. Please enter 'y' or 'n'." << endl;
        }
    } while (choice != 'y' && choice != 'n');
    return choice == 'y';
}


int main() {
    auto mainStart = high_resolution_clock::now();

    DynamicArray<string> positiveWords, negativeWords;
    const int rCount = 25001;
    bool continueAnalysis = true;

    try {
        cout << "Loading positive words...\n";
        loadWords("positive-words.txt", positiveWords);
        cout << "Loading negative words...\n";
        loadWords("negative-words.txt", negativeWords);
    }
    catch (const runtime_error& e) {
        cerr << e.what() << endl;
        return 1;
    }

    while (continueAnalysis) {
        ifstream reviewFile("tripadvisor_hotel_reviews.csv");
        if (!reviewFile.is_open()) {
            cerr << "Failed to open review file." << endl;
            return 1;
        }

        int totalPositiveWords = 0, totalNegativeWords = 0, totalReviews = 0;
        int totalMatchingReviews = 0, totalUnmatchingReviews = 0;
        WordFrequency wordFrequency;

        int searchChoice = getSearchChoice();

        auto start = high_resolution_clock::now();
        string line;
        while (totalReviews < rCount && getline(reviewFile, line)) {
            string review, ratingStr;
            splitLine(line, review, ratingStr);

            int userRating;
            try {
                userRating = stoi(ratingStr);
            }
            catch (const invalid_argument&) {
                cerr << "Invalid rating format: " << ratingStr << endl;
                continue;
            }

            toLowercase(review);
            cout << "Analyzing review: " << review << " | User Rating: " << userRating << endl;

            int positiveCount = 0, negativeCount = 0;
            DynamicArray<string> collectedPositiveWords, collectedNegativeWords;


            collectSentimentWords(review, positiveWords, negativeWords,
                positiveCount, negativeCount,
                collectedPositiveWords, collectedNegativeWords,
                searchChoice);

            printCollectedWords(collectedPositiveWords, "Positive");
            printCollectedWords(collectedNegativeWords, "Negative");

            for (size_t k = 0; k < collectedPositiveWords.getSize(); ++k) {
                wordFrequency.addWord(collectedPositiveWords.get(k));
            }
            for (size_t k = 0; k < collectedNegativeWords.getSize(); ++k) {
                wordFrequency.addWord(collectedNegativeWords.get(k));
            }

            double sentimentScore = calculateSentimentScore(positiveCount, negativeCount);
            cout << "Sentiment score (1 - 5): " << sentimentScore << endl;

            if ((sentimentScore > 3 && userRating < 3) || (sentimentScore < 3 && userRating > 3)) {
                cout << "User's subjective evaluation does not match the sentiment score.\n" << endl;
                totalUnmatchingReviews++;
            }
            else {
                cout << "User's subjective evaluation matches the sentiment score.\n" << endl;
                totalMatchingReviews++;
            }

            // Stop the timer
            auto stop = high_resolution_clock::now();

            // Calculate the duration and print the execution time in seconds
            auto duration = duration_cast<milliseconds>(stop - start);
            cout << "Time execution for this searching algorithm: " << duration.count() << " milliseconds." << "\n" << endl;

            totalPositiveWords += positiveCount;
            totalNegativeWords += negativeCount;
            totalReviews++;
        }

        auto end = high_resolution_clock::now();
        duration<double> elapsed = end - start;

        double accuracy = 0;
        if (totalMatchingReviews + totalUnmatchingReviews > 0) {
            accuracy = (static_cast<double>(totalMatchingReviews) /
                (totalMatchingReviews + totalUnmatchingReviews)) * 100;
        }

        cout << "-----------------------------------\n";
        cout << "Review Analysis Summary:\n";
        cout << "-----------------------------------\n";
        cout << "Total Reviews: " << totalReviews << "\n";
        cout << "Total Positive Words: " << totalPositiveWords << "\n";
        cout << "Total Negative Words: " << totalNegativeWords << "\n";
        cout << "Total Matching Reviews: " << totalMatchingReviews << "\n";
        cout << "Total Unmatching Reviews: " << totalUnmatchingReviews << "\n";
        cout << "Overall Evaluation Accuracy: " << accuracy << "%\n";
        cout << "Analysis execution time: " << elapsed.count() << " seconds\n";
        cout << "-----------------------------------\n";

        bool keepSorting = true;
        while (keepSorting) {
            int sortChoice = getSortChoice();
            if (sortChoice == 3) {
                keepSorting = false;
            }
            else {
                performSortAndPrintFrequencies(wordFrequency, sortChoice);
            }
        }

        cout << "Maximum Frequency: " << wordFrequency.findMaxFrequency() << endl;
        cout << "Minimum Frequency: " << wordFrequency.findMinFrequency() << endl;

        if (getUserChoice("Do you want to search for specific words? (y/n): ")) {
            do {
                searchForWordFrequency(wordFrequency);
            } while (getUserChoice("Search for another word? (y/n): "));
        }

        continueAnalysis = getUserChoice("Do you want to analyze another set of reviews? (y/n): ");

        if (!continueAnalysis) {
            cout << "Thank you for using the review analysis program. Goodbye!" << endl;
        }
        else {
            wordFrequency.resetFrequencies();
        }
    }

    auto mainEnd = high_resolution_clock::now();
    duration<double> overallElapsed = mainEnd - mainStart;
    cout << "Total time taken for overall analysis: " << overallElapsed.count() << " seconds.\n";

    return 0;
}