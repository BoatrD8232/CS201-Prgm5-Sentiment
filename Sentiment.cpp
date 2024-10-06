#include "Sentiment.h"

//SUPPORT FUNCTION TO FIND THE CURRENT WORD IN THE SENTEMENT VECTOR & RETURN THE SENTIMENT VALUE
//PRE:   accepts a string (in) and the words list
//POST:  returns the sentiment of the word if found or the value 0 if not
double sentimentVal(string in, vector<wordList> &words) {
    // A standard binary search that finds a word in the list.
    // The list is presorted, no sorting logic needed.
    int low = 0, high = words.size() - 1;
    int mid;
    while (low <= high) { 
        mid = low + (high - low) / 2;
        if (in > words.at(mid).word) {
            low = mid + 1;
        }
        else if (in < words.at(mid).word) {
            high = mid - 1;
        }
        else {
            return words.at(mid).value;
        }
    }
    return 0;

}


//PRE:   accepts an empty set of 3 <wordList> vectors
//POST:  loads all three vectors with data from sentiment.txt
void loadSentiment(vector<wordList>& sentList,
    vector<wordList>& posList,
    vector<wordList>& negList) {
    ifstream inFile("sentiment.txt");
    // The string for storing the each line of the file, and the strings to store each word and value to convert.
    string line, word, val;
    // Stores the word and value, and is placed in the vector.
    wordList tempWord;
    //open sentiment.txt
    //add the word to the sentList vector
    //if positive enough add to posList
    //if negative enough add to negList
    //close sentiment.txt
    if (inFile.is_open()) {
        while (getline(inFile, line)) {
            stringstream readSS(line);
            getline(readSS, word, ',');
            getline(readSS, val);
            tempWord.word = word;
            tempWord.value = stof(val);
            sentList.push_back(tempWord);
            // Fills the pos and neg lists.
            if (tempWord.value > 1.25) {
                posList.push_back(tempWord);
            }
            else if (tempWord.value < -1.25) {
                negList.push_back(tempWord);
            }
        }
        inFile.close();
    }
    else {
        cout << "Error opening sentiment text file." << endl;
    }
    return;
}

//PRE: Accepts the input ouput file streams, the filename, the list of words, 
//the list of negative words, and the list of negative words.
//POST: Outputs the original review, and the more positive and negative reviews.
// Note- I misread the instructions and believed that processFile was only to be used for part 2,
// and that I needed to create this function for part 1. I realized my mistake but decided to finish this function instead.
// This function is functionally identical to processFile.
void processReview(ifstream& inFile, ofstream& outFile, string fileName, vector<wordList>& wordSent, 
    vector<wordList>& posList, vector<wordList>& negList) {
    // newNegWords stores the original word and the new, more negative word.
    // newPosWords stores the original word and the new, more positive word.
    // negWords stores the new, more negative review.
    // posWords stores the new, more positive review.
    vector <replaceList> newNegWords, newPosWords;
    vector<wordList> negWords, posWords;
    // Stores a word and a pair of words that will be placed in the above lists.
    wordList tempWord;
    replaceList tempPair;
    // inWord - the unedited word.
    // newWord - the word after all non letters are removed.
    string inWord, newWord;
    // lineLen keeps track of how long each line of output is.
    // randInt stores a randomly generated number that is used to choose a new word from the pos/neg lists.
    int lineLen = 0, randInt;
    // Used as a temp variable when stripping inWord.
    char newChar;
    // totslSent is the total sentiment of the original review.
    // totalNegSent is the total sentiment of the negative review.
    // totalPosSent is the total sentiment of the positive review.
    // tempSent stores the output from the sentimentVal function.
    float totalSent = 0, totalNegSent = 0, totalPosSent = 0, tempSent;

    outFile << "\n\n\nPROCESSING FILE: " << fileName << endl << endl;
    outFile << "FORMATTED REVIEW:" << endl;
    while (inFile >> inWord) {
        newWord = "";
        // This for loop strips all non letters from the words.
        // Copied from the code provided for processFile, not written by me.
        for (int i = 0, len = inWord.size(); i < len; i++) {
            if (isalpha(inWord[i])) {
                newChar = tolower(inWord[i]);
                newWord += newChar;
            }
        }
        // Outputs the original review to output.
        // The if else outputs a new line if the next word will put the line over 80 characters.
        if (lineLen + inWord.length() + 1 > 80) {
            outFile << '\n';
            outFile << inWord << " ";
            lineLen = inWord.length() + 1;
        }
        else {
            outFile << inWord << " ";
            lineLen += inWord.length() + 1;
        }
        // Finds the sentiment of new word.
        tempSent = sentimentVal(newWord, wordSent);
        totalSent += tempSent;
        // Creates  the entry to be added to the lists.
        tempWord.word = inWord;
        tempWord.value = tempSent;
        // Checks if the word needs to be made more negative.
        if (tempSent > 1) {
            // Chooses a random negative word and adds it to the negative review.
            randInt = rand() % negList.size();
            negWords.push_back(negList.at(randInt));
            // Adds the old new pair to the list.
            tempPair.newWord = negList.at(randInt).word;
            tempPair.newValue = negList.at(randInt).value;
            tempPair.origWord = newWord;
            tempPair.origValue = tempSent;
            newNegWords.push_back(tempPair);
        }
        else {
            // Adds the original word.
            negWords.push_back(tempWord);
        }
        // Checks if the word needs to be more positive.
        if (tempSent < -1) {
            // Chooses a random positive word and adds it to the positive review.
            randInt = rand() % posList.size();
            posWords.push_back(posList.at(randInt));
            // Adds the old new pair to the list.
            tempPair.newWord = posList.at(randInt).word;
            tempPair.newValue = posList.at(randInt).value;
            tempPair.origWord = newWord;
            tempPair.origValue = tempSent;
            newPosWords.push_back(tempPair);
        }
        else {
            // Adds the original word.
            posWords.push_back(tempWord);
        }
    }
    outFile << "\n\nORIGINAL SENTIMENT: " << totalSent << "\n\n" << endl;
    outFile << "WORDS UPDATED TO BE MORE NEGATIVE:" << endl;
    // If a change has been made, output the word pairs and the new negative review.
    float origTotal = 0;
    float negTotal = 0;
    if (newNegWords.size() != 0) {
        // Ouputs the word pairs.
        for (int i = 0; i < newNegWords.size(); ++i) {
            outFile << fixed << setprecision(2) << right << setw(15) << newNegWords.at(i).origWord 
                << setw(10) << newNegWords.at(i).origValue << setw(20)
                << newNegWords.at(i).newWord << setw(10) << newNegWords.at(i).newValue << endl;
            // Sums the word pair values.
            origTotal += newNegWords.at(i).origValue;
            negTotal += newNegWords.at(i).newValue;
        }
        outFile <<"TOTALS: " << setw(17) << origTotal << setw(30) << negTotal << endl;
        lineLen = 0;
        outFile << "\n\n\nUPDATED REVIEW (MORE NEGATIVE):" << endl;
        // Ouputs the negative review. The if ouputs a newline if the next word puts the line above 80 characters.
        for (int i = 0; i < negWords.size(); ++i) {
            if (lineLen + negWords.at(i).word.length() + 1 > 80) {
                outFile << '\n';
                outFile << negWords.at(i).word << " ";
                lineLen = negWords.at(i).word.length() + 1;
                totalNegSent += negWords.at(i).value;
            }
            else {
                outFile << negWords.at(i).word << " ";
                lineLen += negWords.at(i).word.length() + 1;
                totalNegSent += negWords.at(i).value;
            }
        }
        outFile << "\n\nUPDATED SENTIMENT: " << totalNegSent << endl;
    }
    else {
        outFile << "\nREVIEW NOT UPDATED. THE SENTIMENT REMAINS: " << totalSent << endl;
    }
    outFile << "\nWORDS UPDATED TO BE MORE POSITIVE:" << endl;
    // If a change has been made, output the word pairs and the new positive review.
    origTotal = 0;
    float posTotal = 0;
    if (newPosWords.size() != 0) {
        // Ouputs the word pairs.
        for (int i = 0; i < newPosWords.size(); ++i) {
            outFile << fixed << setprecision(2) << right << setw(15) << newPosWords.at(i).origWord 
                << setw(10) << newPosWords.at(i).origValue << setw(20)
                << newPosWords.at(i).newWord << setw(10) << newPosWords.at(i).newValue << endl;
            // Sums the word pair values.
            origTotal += newPosWords.at(i).origValue;
            posTotal += newPosWords.at(i).newValue;
        }
        outFile << "TOTALS: " << setw(17) << origTotal << setw(30) << posTotal << endl;
        lineLen = 0;
        outFile << "\n\n\nUPDATED REVIEW (MORE POSITIVE):" << endl;
        // Ouputs the positive review. The if ouputs a newline if the next word puts the line above 80 characters.
        for (int i = 0; i < posWords.size(); ++i) {
            if (lineLen + posWords.at(i).word.length() + 1 > 80) {
                outFile << '\n';
                outFile << posWords.at(i).word << " ";
                lineLen = posWords.at(i).word.length() + 1;
                totalPosSent += posWords.at(i).value;
            }
            else {
                outFile << posWords.at(i).word << " ";
                lineLen += posWords.at(i).word.length() + 1;
                totalPosSent += posWords.at(i).value;
            }
        }
        outFile << "\nUPDATED SENTIMENT: " << totalPosSent << endl;
    }
    else {
        outFile << "\nREVIEW NOT UPDATED. THE SENTIMENT REMAINS: " << totalSent << endl;
    }
}
