#include <iostream>
#include <string>
#include <sstream>
#include <vector>

// --- Project Headers ---
#include "SearchEngine.h"
#include "LinkedList.h" //query results

// --- Helper Function Prototypes ---
void printWelcome();
void printHelp();
void printQueryResults(LinkedList<std::string>& results);
void handleQuery(SearchEngine& engine, const std::string& query);
void handleView(SearchEngine& engine, const std::string& docID);
void handleBack(SearchEngine& engine);
void handleForward(SearchEngine& engine);

// --- Main Function ---
int main() {

    try {
        std::cout << "DEBUG: Current Working Directory is: "
            << std::filesystem::current_path() << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "DEBUG: Error getting current path: " << e.what() << std::endl;
    }

    //pass the documents directory to the engine's constructor
    SearchEngine engine("documents");

    //build index from all .txt files in that directory
    engine.buildIndexFromDirectory();

    printWelcome();

    std::string line;
    while (true) {
        std::cout << "\n> ";
        if (!std::getline(std::cin, line)) {
            break; //end of input
        }

        if (line.empty()) {
            continue;
        }

        //use istringstream to split command and its arguments
        std::istringstream iss(line);
        std::string command;
        iss >> command; //read the first word

        if (command == "quit" || command == "exit") {
            std::cout << "Goodbye!" << std::endl;
            break;
        }
        else if (command == "query" || command == "q") {
            std::string restOfLine;
            std::getline(iss, restOfLine); //read the rest of the line
            handleQuery(engine, restOfLine);
        }
        else if (command == "view" || command == "v") {
            std::string docID;
            iss >> docID;
            handleView(engine, docID);
        }
        else if (command == "back" || command == "b") {
            handleBack(engine);
        }
        else if (command == "forward" || command == "f") {
            handleForward(engine);
        }
        else if (command == "help") {
            printHelp();
        }
        else {
            std::cout << "Unknown command. Type 'help' for a list of commands." << std::endl;
        }
    }

    return 0;
}

// --- Helper Function Implementations ---

void printWelcome() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "  Welcome to the C++ Search Engine!" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Type 'help' for commands, 'quit' to exit." << std::endl;
}

void printHelp() {
    std::cout << "Available commands:" << std::endl;
    std::cout << "  query <term> [term2]..." << " : Search for docs with all terms." << std::endl;
    std::cout << "  view <docID>" << "             : View a document by its ID (e.g., doc1.txt)." << std::endl;
    std::cout << "  back" << "                     : Go to the previous document." << std::endl;
    std::cout << "  forward" << "                  : Go to the next document." << std::endl;
    std::cout << "  help" << "                     : Show this help message." << std::endl;
    std::cout << "  quit" << "                     : Exit the program." << std::endl;
}

//prints resuts of query
void printQueryResults(LinkedList<std::string>& results) {
    if (results.isEmpty()) {
        std::cout << "  No documents found." << std::endl;
        return;
    }
    std::cout << "  Documents found: ";

    // We must copy the list to print it
    LinkedList<std::string> temp = results;
    temp.print();
}

//handles query command
void handleQuery(SearchEngine& engine, const std::string& queryString) {
    if (queryString.empty() || queryString.find_first_not_of(' ') == std::string::npos) {
        std::cout << "  Usage: query <term1> [term2]..." << std::endl;
        return;
    }
    LinkedList<std::string> results = engine.query(queryString);
    printQueryResults(results);
}

//handles view command
void handleView(SearchEngine& engine, const std::string& docID) {
    if (docID.empty()) {
        std::cout << "  Usage: view <docID> (e.g., view doc1.txt)" << std::endl;
        return;
    }
    engine.viewDocument(docID);
}

//back command
void handleBack(SearchEngine& engine) {
    // --- UPDATED: No more cout here ---
    engine.goBack();
}

//forwards command
void handleForward(SearchEngine& engine) {
    engine.goForward();
}