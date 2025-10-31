#pragma once

#include <string>
#include <fstream>      //for file handling (ifstream)
#include "LinkedList.h" //returning one of these!

class Tokenizer {
public:
    
    //reads a text file from the given filename, normalizes all words,
    //returns a LinkedList containing the tokens.
    //throws runtime_error if the file cannot be opened.
    LinkedList<std::string> processFile(const std::string& filename);

    
    //helper function to normalize a single word, converts to lowercase
    //strips all punctuation *except* for the '+' sign
    //(this is to pass the "c++" test case)
    std::string normalizeWord(const std::string& word);
};