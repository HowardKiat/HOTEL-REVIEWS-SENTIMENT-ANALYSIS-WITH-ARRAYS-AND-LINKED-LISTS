#include <iostream>
#include <iomanip>
#include <fstream> //get file library
#include <string> //get content from csv

using namespace std;
// develop struct to group the Hotel Review attributes
struct HotelReviewItem
{
	string Review;
	int Rating; //NOTE TO SELF: CAN CHANGE TO DOUBLE if needed according to calculations
};

// develop class for Hotel Review
class HotelReviewList // linked list action call
{
	HotelReviewItem* hotelReviewList;
	int hotelReviewSize;

public:
	HotelReviewList(int size)
	{
		hotelReviewList = new HotelReviewItem[size]; //create the dynamic array list
		hotelReviewSize = size;
	}
	void setHotelReviewInfo(int index, string Review, int Rating)//setter
	{
		hotelReviewList[index].Review = Review;
		hotelReviewList[index].Rating = Rating;
	}
	void displayHotelReviewInfo()
	{
		for (int index = 0; index < hotelReviewSize; index++)
		{
			cout << "Review\t: " << hotelReviewList[index].Review << endl;
			cout << fixed << setprecision(2);
			cout << "Rating\t: " << hotelReviewList[index].Rating << endl;
			cout << endl;
		}
	}
};

int main()
{
    HotelReviewList list(20493); // Create a list with 20493 entries (adjust based on your dataset size)
    ifstream file("tripadvisor_hotel_reviews.csv"); // Open the CSV file

    // Check if the file opened successfully
    if (!file.is_open())
    {
        cout << "Error opening file!" << endl;
        return 1;
    }

    string Review, Rating; // Variables to store data read from the file
    int index = 0; // Index to track where to store the data in the array

    // Loop to read content from the file
    while (getline(file, Review, ',') && getline(file, Rating))
    {
        if (Review.find("Review") != string::npos) // Skip header row
        {
            continue;
        }
        else if (Review.empty() || Rating.empty()) // If either Review or Rating is empty, stop reading
        {
            break;
        }

        // Convert the Rating from string to integer
        int ratingValue = stoi(Rating);

        // Store the review and rating in the array
        list.setHotelReviewInfo(index, Review, ratingValue);
        index++;
    }

    // Close the file after reading
    file.close();

    // Display all the reviews and ratings
    list.displayHotelReviewInfo();

    return 0;
}
