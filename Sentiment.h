#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

using namespace std;
// struct WordList stores a word and the coresponding sentiment/
struct wordList {
    string word;
    double value;
};
// sturct replaceList stores a replaced word, the new word, and both their sentiment values.
struct replaceList {
    string origWord;
    double origValue;
    string newWord;
    double newValue;
};

//PRE: takes in a sorted list word words plus their sentiment value, and a word to look for.
//POST: outputs the sentiment value of the target word, zero if the word is not found.
double sentimentVal(string in, vector<wordList>& words);

//PRE:   accepts an empty set of 3 <wordList> vectors
//POST:  loads all three vectors with data from sentiment.txt
void   loadSentiment(vector<wordList>& sentList, vector<wordList>& posList, vector<wordList>& negList);

//PRE: Accepts the input ouput file streams, the filename, the list of words, 
//the list of negative words, and the list of negative words.
//POST: Outputs the original review, and the more positive and negative reviews.
void processReview(ifstream& inFile, ofstream& outFile, string fileName, vector<wordList>& words, 
    vector<wordList>& posList, vector<wordList>& negList);






 
