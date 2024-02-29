#include "csv.hpp"

void uploadFile() {
    // Create an output filestream object and append to it
    std::ofstream myFile("resources/test.csv", std::ios::app);

    // Send data to the stream
    myFile << "1\n";
    myFile << "2\n";
    myFile << "2\n";
    myFile << "3\n";

    // Close the file
    myFile.close();
}