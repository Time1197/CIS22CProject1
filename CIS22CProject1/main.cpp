#include <iostream>
#include <string>
#include <stdexcept>
#include <iomanip> // For std::setw

// We only need the "top-level" class
#include "SearchEngine.h"

// Helper function to print a welcome message
void printWelcomeMessage() {
    std::cout << "=========================================" << std::endl;
    std::cout << "  C++ Simplified Search Engine           " << std::endl;
    std::cout << "=========================================" << std::endl;
    std::cout << "  (Data Structures by [Your Name Here])  " << std::endl;
    std::cout << "\nBuilding index from 'documents/' directory..." << std::endl;
}

// Helper function to print the list of commands
void printHelp() {
    std::cout << "\n--- Commands ---" << std::endl;
    std::cout << "  query [term(s)]  : Search for documents containing all terms." << std::endl;
    std::cout << "                     (e.g., query quick fox)" << std::endl;
    std::cout << "  view [docID]     : View a document to add to history." << std::endl;
    std::cout << "                     (e.g., view doc1.txt)" << std::endl;
    std::cout << "  back             : Go to the previous document." << std::endl;
    std::cout << "  forward          : Go to the next document." << std::endl;
    std::cout << "  current          : Show the currently viewed document." << std::endl;
    std::cout << "  help             : Show this help message." << std::endl;
    std::cout << "  quit             : Exit the program." << std::endl;
    std::cout << "------------------" << std::endl;
}

// Helper function to print query results
void printResults(const LinkedList<std::string>& results, const std::string& query) {
    if (results.isEmpty()) {
        std::cout << "  No documents found containing all terms: \"" << query << "\"" << std::endl;
    }
    else {
        std::cout << "  Documents found for \"" << query << "\":" << std::endl;
        // This is a small hack since our list doesn't have an iterator.
        // We will make a copy and then print by destructively
        // removing from the copy.
        LinkedList<std::string> copy = results;
        while (!copy.isEmpty()) {
            std::cout << "    - " << copy.removeFromHead() << std::endl;
        }
    }
}

/**
 * Main application entry point.
 * This runs the interactive Command-Line Interface (CLI).
 */
int main() {
    SearchEngine engine;

    printWelcomeMessage();

    try {
        // --- 1. Build the Index ---
        // This is the main setup step
        engine.buildIndexFromDirectory("documents");
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "\n!!! CRITICAL ERROR: Cannot access 'documents' directory. !!!" << std::endl;
        std::cerr << "!!! Please create a folder named 'documents' in the     !!!" << std::endl;
        std::cerr << "!!! same directory as the .exe and add your .txt files. !!!" << std::endl;
        std::cerr << "!!! Error details: " << e.what() << std::endl;
        std::cout << "\nPress Enter to exit...";
        std::cin.get();
        return 1; // Exit with error
    }
    catch (const std::exception& e) {
        std::cerr << "An unexpected error occurred during indexing: " << e.what() << std::endl;
        return 1;
    }

    printHelp();

    // --- 2. Run the Interactive Query Loop ---
    std::string line;
    while (true) {
        std::cout << "\n> ";
        if (!std::getline(std::cin, line) || line == "quit") {
            break; // Exit on "quit" or end-of-file (Ctrl+D/Ctrl+Z)
        }

        if (line.empty()) {
            continue;
        }

        // Use a stringstream to parse the command and its arguments
        std::stringstream ss(line);
        std::string command;
        ss >> command; // Get the first "word", which is the command

        try {
            if (command == "query") {
                std::string queryString;
                // Get the rest of the line as the query
                std::getline(ss, queryString);

                // Remove leading whitespace from query
                size_t first = queryString.find_first_not_of(" \t");
                if (first == std::string::npos) {
                    std::cout << "  Usage: query [term(s)]" << std::endl;
                    continue;
                }
                queryString = queryString.substr(first);

                LinkedList<std::string> results = engine.query(queryString);
                printResults(results, queryString);
            }
            else if (command == "view") {
                std::string docID;
                if (!(ss >> docID)) {
                    std::cout << "  Usage: view [docID]" << std::endl;
                    continue;
                }
                engine.viewDocument(docID);
            }
            else if (command == "back") {
                engine.goBack();
            }
            else if (command == "forward") {
                engine.goForward();
            }
            else if (command == "current") {
                std::cout << "  Current doc: " << engine.getCurrentDocument() << std::endl;
            }
            else if (command == "help") {
                printHelp();
            }
            else {
                std::cout << "  Unknown command: \"" << command << "\". Type 'help' for commands." << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "  An error occurred: " << e.what() << std::endl;
        }
    }

    std::cout << "\nGoodbye!" << std::endl;
    return 0; // Success
}