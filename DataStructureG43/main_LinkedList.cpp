// mainLinkedList.cpp

#include "SentimentAnalysisLinkedList.hpp"
#include <iostream>

int main() {
    SentimentAnalysisLinkedList analysis;

    // Load positive and negative words
    analysis.loadPositiveWords("positive-words.txt");
    analysis.loadNegativeWords("negative-words.txt");

    // Load reviews from CSV
    analysis.loadCSV("tripadvisor_hotel_reviews.csv");

    // Manually add some reviews for testing purposes
    analysis.addReview("This hotel is amazing and the service is great!", 5);
    analysis.addReview("The food was terrible, and the noise was unbearable.", 2);
    analysis.addReview("The location is perfect, highly recommended.", 5);
    analysis.addReview("Not worth the money.", 1);

    // Perform sentiment analysis
    analysis.analyzeSentimentReviews();

    // Calculate overall sentiment
    analysis.calculateOverallSentiment();

    return 0;
}
