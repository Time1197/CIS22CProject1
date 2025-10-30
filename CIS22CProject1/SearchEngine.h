#pragma once

#include <string>
#include <vector>
#include <filesystem> //C++ 17 directory iteration
#include <fstream>
#include <sstream>
#include <iostream>

//including data structures
#include "Tokenizer.h"
#include "LinkedList.h"
#include "ArrayStack.h"
#include "HashMap.h" //hashmap will be used for inverted index
#include "RBMap.h" //used for inner postings list

class SearchEngine {
private:
	//the inverted index should go like this
	// word --> (docID --> frequency)
	//use RB map for the inner map since it's good with balencing

	HashMap<std::string, RBMap<std::string, int>> invertedIndex;

	//a list of all the documents that have been indexed
	LinkedList<std::string> indexedDocuments;

	//tokenizer to use normalizeWord
	Tokenizer tokenizer;

	//history stacks for navigation
	ArrayStack<std::string> historyBack;
	ArrayStack<std::string> historyForward;
	std::string currentDocument;

public:
	//indexing

	//iterates thru all text files in a given direcotry path and builds inverted index
	void buildIndexFromDirectory(const std::string& directoryPath);

	//prints the contetns of inverted index (debugging)
	void printIndex() const;

	//querying
	//processes a user query string, tokenizes it, finds all documents
	//that have that term, returns results

	LinkedList<std::string> query(const std::string& queryString);

	//navigation

	//views a document with updates navigation history
	void viewDocument(const std::string& docID);

	//moves back in history to previous document
	std::string goBack();

	//moves forwards in history
	std::string goForward();

	//returns id of current document
	std::string getCurrentDocument() const;
};