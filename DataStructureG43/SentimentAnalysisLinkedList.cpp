//// SentimentAnalysisLinkedList.cpp
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include "DoublyLinkedList.hpp"
//#include <chrono>
//
//void loadWords(const std::string& filename, LinkedList& wordList) {
//    std::ifstream file(filename);
//    std::string word;
//    while (file >> word) {
//        wordList.append(word);
//    }
//}
//
//double analyzeSentiment(const std::string& review, const LinkedList& positiveWords, const LinkedList& negativeWords) {
//    int positiveCount = 0;
//    int negativeCount = 0;
//    std::istringstream iss(review);
//    std::string word;
//    while (iss >> word) {
//        if (positiveWords.contains(word)) {
//            positiveCount++;
//        }
//        else if (negativeWords.contains(word)) {
//            negativeCount++;
//        }
//    }
//    return positiveCount - negativeCount; // Simplified score
//}
//
//void processCSV(const std::string& csvFilename, const LinkedList& positiveWords, const LinkedList& negativeWords) {
//    std::ifstream infile(csvFilename);
//    std::string line;
//    while (std::getline(infile, line)) {
//        double sentiment = analyzeSentiment(line, positiveWords, negativeWords);
//        // Store or display sentiment result as needed
//    }
//}
//
//int main() {
//    LinkedList positiveWords;
//    LinkedList negativeWords;
//
//    auto start = std::chrono::high_resolution_clock::now();
//
//    loadWords("positive-words.txt", positiveWords);
//    loadWords("negative-words.txt", negativeWords);
//
//    processCSV("tripadvisor_hotel_reviews.csv", positiveWords, negativeWords);
//
//    auto end = std::chrono::high_resolution_clock::now();
//    std::chrono::duration<double> elapsed = end - start;
//    std::cout << "Processing time: " << elapsed.count() << " seconds\n";
//
//    return 0;
//}
//
//
