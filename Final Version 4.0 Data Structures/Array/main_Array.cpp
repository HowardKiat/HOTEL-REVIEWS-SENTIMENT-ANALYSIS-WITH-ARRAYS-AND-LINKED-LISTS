//#include "sentiment_analysis.hpp"
//#include <iostream>
//
//int main()
//{
//    // Create an instance of the SentimentAnalysis class on the heap
//    SentimentAnalysis* analysis = new SentimentAnalysis();
//
//    // Load positive and negative words from files
//    analysis->loadPositiveWords();
//    analysis->loadNegativeWords();
//
//    //Load Reviews from CSV files
//    analysis->loadCSV("tripadvisor_hotel_reviews.csv");
//
//    // Manually add some reviews for testing purposes (FOR TESTING USE - HOWARD :))
//    analysis->addReview("This hotel is amazing and the service is great!", 5);
//    analysis->addReview("The food was terrible, and the noise was unbearable.", 2);
//    analysis->addReview("The location is perfect, highly recommended.", 5);
//    analysis->addReview("Not worth the money.", 1);
//
//    // Perform sentiment analysis on the reviews
//    analysis->analyzeSentimentReviews();
//
//    // Calculate and display overall sentiment
//    analysis->calculateOverallSentiment();
//
//    // Clean up the allocated memory
//    delete analysis;
//
//    return 0;
//}