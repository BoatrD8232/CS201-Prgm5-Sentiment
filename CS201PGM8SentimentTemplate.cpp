// CS201PGM8Sentiment 
// NAME:  David Boatright
// 10/5/2024

#include "Sentiment.h"

int main()
{
    vector<wordList> sentList, posList, negList;
    ifstream inFile;
    ofstream outFile;
    outFile.open("output.txt");

    // load vectors 
    loadSentiment(sentList, posList, negList);

    // open output file 
    // open review files

    string fileName;
    for (unsigned int i = 1; i < 9; i++) {
        // open input file adding to_string(i) + ".txt" to review
        // if not able to open, print a message and continue
        // else process the file & close it
        fileName = "review" + to_string(i) + ".txt";
        inFile.open(fileName);
        if (!inFile.is_open()) {
            cout << "Error opening " << fileName << endl;
        }
        else {
            processReview(inFile, outFile, fileName, sentList, posList, negList);
        }
        inFile.close();
    }

    //close the output file
    outFile.close();
}
 
