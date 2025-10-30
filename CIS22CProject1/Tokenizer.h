#pragma once

#include <string>
#include <fstream>      // For file handling (ifstream)
#include "LinkedList.h" // We will be returning one of these!

class Tokenizer {
public:
    /**
     * Reads a text file from the given filename, normalizes all words,
     * and returns a LinkedList containing the tokens.
     * Throws std::runtime_error if the file cannot be opened.
     */
    LinkedList<std::string> processFile(const std::string& filename);

    /**
     * Helper function to normalize a single word.
     * - Converts to lowercase
     * - Strips all punctuation *except* for the '+' sign
     * (This is to pass the "c++" test case)
     */
    std::string normalizeWord(const std::string& word);
};