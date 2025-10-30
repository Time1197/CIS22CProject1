#include "Tokenizer.h"
#include <sstream> //for istringstream
#include <cctype> //for isalnum and tolower
#include <stdexcept> //fpr runtime_errror

//public methods

LinkedList<std::string> Tokenizer::processFile(const std::string& fileName) {
	std::ifstream file(fileName);
	if (!file.is_open()) {
		//check if file is open or not
		//if not:
		throw std::runtime_error("Could not open file:" + fileName);
	}

	LinkedList<std::string> tokens;
	std::string line;

	//read file line by line
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string word;

		//split each line into an individual word
		while (iss >> word) { //seperates words by whitespace
			//normalize each word
			std::string normalized = normalizeWord(word);
			
			//add to our list if it is not empty
			if (!normalized.empty()) {
				tokens.insert(normalized);
			}
		}
	}

	file.close();
	return tokens;
}

//public normalizer helper method

std::string Tokenizer::normalizeWord(const std::string& word) {
	std::string normalizedWord = "";

	//iterate over each character in the input
	for (char c : word) {
		//Keep character if it's alphanumeric or a + sign
		if (std::isalnum(static_cast<unsigned char>(c)) || c == '+') {
			//convert to lowercase and add to new string
			normalizedWord += std::tolower(static_cast<unsigned char>(c));
		}
	}
	return normalizedWord;
}