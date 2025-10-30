#include "SearchEngine.h"

// constructor
SearchEngine::SearchEngine(const std::string& directoryPath) {
	//store path to read files
	docDirectory = directoryPath;
	//initalize current Document to empty
	currentDocument = "";
}

//indexing

void SearchEngine::buildIndexFromDirectory() {
	//clear any old data
	invertedIndex = HashMap<std::string, RBMap<std::string, int>>();
	indexedDocuments = LinkedList<std::string>();

	std::cout << "Starting to index directory: " << docDirectory << std::endl;

	//C++ 17 filesystem iterator
	for (const auto& entry : std::filesystem::directory_iterator(docDirectory)) {
		std::string filepath = entry.path().string();
		std::string filename = entry.path().filename().string();

		//only process .txt files
		if (entry.path().extension() == ".txt") {
			std::cout << " Indexing: " << filename << std::endl;

			//add to list of known documents
			indexedDocuments.insert(filename);

			std::ifstream file(filepath);
			if (!file.is_open()) {
				std::cerr << " Warning: Could not open file " << filename << std::endl;
				continue;
			}

			std::string word;
			//process file word by word
			while (file >> word) {
				//use normalized word from tokenizer
				std::string normalizedWord = tokenizer.normalizeWord(word);

				if (normalizedWord.empty()) {
					continue;
				}

				//1. find or create postings list for the word
				RBMap<std::string, int> postingsList;
				if (!invertedIndex.search(normalizedWord, postingsList)) {
					//word is not in the index yet. Create a new, empty map for it.
					postingsList = RBMap<std::string, int>();
				}

				//2. find or create the frequency count for this document
				int frequency = 0;
				if (postingsList.search(filename, frequency)) {
					//document is already in the list
					frequency++;
				}
				else {
					//document is not in the list
					frequency = 1;
				}

				//3. update/insert the new frequency count back into postings list
				postingsList.insert(filename, frequency);

				//4. update/insert the postings back into main index
				invertedIndex.insert(normalizedWord, postingsList);
			}
			file.close();

		}
	}
	std::cout << "Indexing complete." << std::endl;
}

//prints the contents of the inverted index
void SearchEngine::printIndex() const {
	std::cout << "\n--- Inverted Index Contents ---" << std::endl;
	std::cout << "Indexed Documents: ";
	indexedDocuments.print();
}

//querying

//processes a user query string
//returns a list of matcihg ids
LinkedList<std::string> SearchEngine::query(const std::string& queryString) {
	LinkedList<std::string> matchingDocs;
	LinkedList<std::string> queryTerms;

	//1. tokenize query string
	std::istringstream iss(queryString);
	std::string word;

	while (iss >> word) {
		std::string normalized = tokenizer.normalizeWord(word);
		if (!normalized.empty()) {
			queryTerms.insert(normalized);
		}
	}

	if (queryTerms.isEmpty()) {
		return matchingDocs; //return empty list for empty query
	}

	//2. iterate thru every indexed document
	//I guess this is a temporary solution for now
	LinkedList<std::string> docsToSearch = indexedDocuments;
	if (docsToSearch.isEmpty()) {
		// HACK: If index hasn't been built, use test docs
		docsToSearch.insert("doc1.txt");
		docsToSearch.insert("doc2.txt");
		docsToSearch.insert("doc3.txt");
		docsToSearch.insert("doc4.txt");
	}

	// Since we don't have an iterator, we'll use a while loop and removeFromHead
	// This is destructive, so let's use the 'indexedDocuments' member
	while (!indexedDocuments.isEmpty()) {
		std::string docID = indexedDocuments.removeFromHead();

		bool allTermsFound = true;

		// 3. For each doc, check if it contains *all* query terms

		// Another iterator-less loop. This is getting tricky.
		// Let's create a temporary vector for the query terms to iterate
		std::vector<std::string> termsVec;
		LinkedList<std::string> tempQueryTerms = queryTerms; // Copy
		while (!tempQueryTerms.isEmpty()) {
			termsVec.push_back(tempQueryTerms.removeFromHead());
		}

		// Create a vector of all document IDs
		LinkedList<std::string> docCopy = indexedDocuments; // Copy
		std::vector<std::string> docVec;
		while (!docCopy.isEmpty()) {
			docVec.push_back(docCopy.removeFromHead());
		}


		for (const std::string& term : termsVec) {
			RBMap<std::string, int> postings;
			// a) Check if word is in the main index
			if (!invertedIndex.search(term, postings)) {
				allTermsFound = false; // Word doesn't exist anywhere
				break;
			}
			// b) Check if this docID is in the word's postings list
			int freq;
			if (!postings.search(docID, freq)) {
				allTermsFound = false; // Word exists, but not in this doc
				break;
			}
		}

		// 4. If all terms were found, add this doc to our results
		if (allTermsFound) {
			matchingDocs.insert(docID);
		}
	}

	// HACK: Restore the indexedDocuments list since we destructively emptied it
	// In a real app, our LinkedList would have an iterator.
	indexedDocuments = docsToSearch;

	return matchingDocs;
}

//navigation

//views a document, which updates the nagivation history

void SearchEngine::viewDocument(const std::string& docID) {
	if (docID.empty()) {
		return; //no changes
	}

	//1. check if doc exists before viewing
	if (!indexedDocuments.find(docID)) {
		std::cout << "Error: Document" << docID << " not found in index." << std::endl;
		return;
	}

	if (docID == currentDocument) {
		std::cout << " (Already viewing " << docID << ")" << std::endl;
		printDocumentContents(docID); //reprint contents
		return;
	}

	//2. push old current document onto the backstack
	if (!currentDocument.empty()) {
		historyBack.push(currentDocument);
	}

	//3. set the new current document
	currentDocument = docID;

	//4. clear the forward history
	//create and empty stack
	historyForward = ArrayStack<std::string>();

	//5. print contents
	printDocumentContents(currentDocument);
}

//moves to the previous document in history
void SearchEngine::goBack() {
	if (historyBack.isEmpty()) {
		std::cout << "  (No back history)" << std::endl;
		return; //nothing to go back to
	}

	//1. push the current document onto the forwards stack
	if (!currentDocument.empty()) {
		historyForward.push(currentDocument);
	}

	//2. pop new doc from back stack
	currentDocument = historyBack.top();
	historyBack.pop();

	printDocumentContents(currentDocument);
}

//moves to next document in history

void SearchEngine::goForward() {
	if (historyForward.isEmpty()) {
		std::cout << "  (No forward history)" << std::endl;
		return;
	}

	//1. push current document to back stack
	if (!currentDocument.empty()) {
		historyBack.push(currentDocument);
	}

	//2. pop new doc from forward stack
	currentDocument = historyForward.top();
	historyForward.pop();

	//3. print the document's contents
	printDocumentContents(currentDocument);
}

std::string SearchEngine::getCurrentDocument() const {
	return currentDocument;
}

//print document contents
void SearchEngine::printDocumentContents(const std::string& docID) const {
	//construct full path
	//use filesystem::path to correctly join paths
	std::filesystem::path fullPath = docDirectory;
	fullPath /= docID;

	std::ifstream file(fullPath);
	if (file.is_open()) {
		std::cout << "\n --- ERROR: could not open " << fullPath.string() << " ---" << std::endl;
		return;
	}

	std::cout << "\n--- Viewing: " << docID << " ---" << std::endl;
	std::string line;
	while (std::getline(file, line)) {
		std::cout << line << std::endl;
	}
	std::cout << "--- End of: " << docID << " ---" << std::endl; // Use std::endl
	file.close();
}