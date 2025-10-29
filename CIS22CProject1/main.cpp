#include <iostream>     // For std::cout, std::endl
#include <string>       // For std::string
#include <vector>       // For testing stack
#include <stdexcept>    // For catching exceptions
#include <chrono>       // For recording time

// --- Our Project Headers ---
#include "LinkedList.h"
#include "ArrayStack.h"
#include "ListQueue.h"
#include "Tokenizer.h"
#include "Sorting.h"

// --- Test Function Prototypes ---
// We'll write these functions below main()
void testLinkedList();
void testArrayStack();
void testListQueue();
void testTokenizer();
void testSorting();

// --- Main Function ---
// This is the entry point of our program.
int main() {
    // Run each of our test suites
    // We wrap them in try/catch blocks in case our
    // data structures throw exceptions (which they should!)

    try {
        testLinkedList();
        testArrayStack();
        testListQueue();
        testTokenizer();
        testSorting();
    }
    catch (const std::exception& e) {
        std::cout << "!!! TEST FAILED (UNCAUGHT EXCEPTION): " << e.what() << std::endl;
        return 1; // Exit with an error code
    }

    std::cout << "\n--- All Phase 1 Tests Passed! ---" << std::endl;

    // Pause the console before closing (Visual Studio specific)
    std::cout << "\nPress Enter to exit...";
    std::cin.get();

    return 0; // Success
}


// --- Test Function Implementations ---

/**
 * Runs all unit tests for the LinkedList class.
 */
void testLinkedList() {
    std::cout << "--- Testing LinkedList<int> ---" << std::endl;
    LinkedList<int> list;

    // Test 1: isEmpty and size on new list
    if (!list.isEmpty() || list.size() != 0) {
        throw std::runtime_error("Test 1 FAILED: New list not empty.");
    }
    std::cout << "Test 1 Passed: New list empty." << std::endl;

    // Test 2: insert()
    list.insert(10);
    list.insert(20);
    list.insert(30); // List is [10, 20, 30]
    if (list.size() != 3 || list.isEmpty()) {
        throw std::runtime_error("Test 2 FAILED: Insert or size failed.");
    }
    list.print();
    std::cout << "Test 2 Passed: insert() and size()." << std::endl;

    // Test 3: deleteNode() - middle
    if (!list.deleteNode(20)) { // List should be [10, 30]
        throw std::runtime_error("Test 3 FAILED: deleteNode(20) returned false.");
    }
    if (list.size() != 2 || list.find(20)) {
        throw std::runtime_error("Test 3 FAILED: deleteNode(20) failed to remove.");
    }
    list.print();
    std::cout << "Test 3 Passed: deleteNode() middle." << std::endl;

    // Test 4: deleteNode() - head
    if (!list.deleteNode(10)) { // List should be [30]
        throw std::runtime_error("Test 4 FAILED: deleteNode(10) returned false.");
    }
    if (list.size() != 1 || list.find(10)) {
        throw std::runtime_error("Test 4 FAILED: deleteNode(10) failed to remove head.");
    }
    list.print();
    std::cout << "Test 4 Passed: deleteNode() head." << std::endl;

    // Test 5: deleteNode() - last item
    if (!list.deleteNode(30)) { // List should be []
        throw std::runtime_error("Test 5 FAILED: deleteNode(30) returned false.");
    }
    if (list.size() != 0 || !list.isEmpty()) {
        throw std::runtime_error("Test 5 FAILED: deleteNode(30) failed to remove last item.");
    }
    list.print();
    std::cout << "Test 5 Passed: deleteNode() last item." << std::endl;

    // Test 6: deleteNode() - on empty list
    if (list.deleteNode(100)) { // Should do nothing
        throw std::runtime_error("Test 6 FAILED: deleteNode(100) on empty list returned true.");
    }
    std::cout << "Test 6 Passed: deleteNode() on empty list." << std::endl;

    // Test 7: Copy Constructor (Deep Copy)
    LinkedList<int> list1;
    list1.insert(1);
    list1.insert(2);
    LinkedList<int> list2 = list1; // Call copy constructor
    list1.insert(3); // Modify original

    if (list2.size() != 2) { // list2 should still only have 2 items
        throw std::runtime_error("Test 7 FAILED: Copy constructor failed (size mismatch).");
    }
    std::cout << "List 1 (should be 3 items): "; list1.print();
    std::cout << "List 2 (should be 2 items): "; list2.print();
    std::cout << "Test 7 Passed: Copy Constructor (deep copy)." << std::endl;

    // Test 8: Copy Assignment Operator (Deep Copy)
    LinkedList<int> list3;
    list3.insert(99);
    list3 = list1; // Call copy assignment
    list1.insert(4); // Modify original again

    if (list3.size() != 3) { // list3 should have 3 items
        throw std::runtime_error("Test 8 FAILED: Copy assignment failed (size mismatch).");
    }
    std::cout << "List 1 (should be 4 items): "; list1.print();
    std::cout << "List 3 (should be 3 items): "; list3.print();
    std::cout << "Test 8 Passed: Copy Assignment (deep copy)." << std::endl;

    std::cout << "--- LinkedList Tests Passed ---" << std::endl;
}


/**
 * Runs all unit tests for the ArrayStack class.
 */
void testArrayStack() {
    std::cout << "\n--- Testing ArrayStack<std::string> ---" << std::endl;
    ArrayStack<std::string> stack;

    // Test 1: isEmpty and size
    if (!stack.isEmpty() || stack.size() != 0) {
        throw std::runtime_error("Test 1 FAILED: New stack not empty.");
    }
    std::cout << "Test 1 Passed: New stack empty." << std::endl;

    // Test 2: push() and top()
    stack.push("hello");
    stack.push("world");
    if (stack.size() != 2 || stack.top() != "world") {
        throw std::runtime_error("Test 2 FAILED: push() or top() failed.");
    }
    std::cout << "Test 2 Passed: push() and top()." << std::endl;

    // Test 3: pop()
    stack.pop(); // Should remove "world"
    if (stack.size() != 1 || stack.top() != "hello") {
        throw std::runtime_error("Test 3 FAILED: pop() failed.");
    }
    std::cout << "Test 3 Passed: pop()." << std::endl;

    // Test 4: pop() to empty
    stack.pop(); // Should remove "hello"
    if (!stack.isEmpty() || stack.size() != 0) {
        throw std::runtime_error("Test 4 FAILED: pop() to empty failed.");
    }
    std::cout << "Test 4 Passed: pop() to empty." << std::endl;

    // Test 5: pop() on empty
    try {
        stack.pop();
        // If we get here, it didn't throw. That's a failure.
        throw std::runtime_error("Test 5 FAILED: pop() on empty stack did not throw exception.");
    }
    catch (const std::out_of_range& e) {
        // This is the expected good behavior
        std::cout << "Test 5 Passed: pop() on empty correctly threw: " << e.what() << std::endl;
    }

    std::cout << "--- ArrayStack Tests Passed ---" << std::endl;
}


/**
 * Runs all unit tests for the ListQueue class.
 */
void testListQueue() {
    std::cout << "\n--- Testing ListQueue<int> ---" << std::endl;
    ListQueue<int> queue;

    // Test 1: isEmpty and size
    if (!queue.isEmpty() || queue.size() != 0) {
        throw std::runtime_error("Test 1 FAILED: New queue not empty.");
    }
    std::cout << "Test 1 Passed: New queue empty." << std::endl;

    // Test 2: enqueue() and front()
    queue.enqueue(100);
    queue.enqueue(200); // Queue is [100, 200]
    if (queue.size() != 2 || queue.front() != 100) {
        throw std::runtime_error("Test 2 FAILED: enqueue() or front() failed.");
    }
    std::cout << "Test 2 Passed: enqueue() and front()." << std::endl;

    // Test 3: dequeue()
    int val = queue.dequeue(); // Should remove 100
    if (val != 100 || queue.size() != 1 || queue.front() != 200) {
        throw std::runtime_error("Test 3 FAILED: dequeue() failed.");
    }
    std::cout << "Test 3 Passed: dequeue()." << std::endl;

    // Test 4: dequeue() to empty
    val = queue.dequeue(); // Should remove 200
    if (val != 200 || !queue.isEmpty() || queue.size() != 0) {
        throw std::runtime_error("Test 4 FAILED: dequeue() to empty failed.");
    }
    std::cout << "Test 4 Passed: dequeue() to empty." << std::endl;

    // Test 5: dequeue() on empty
    try {
        queue.dequeue();
        // If we get here, it didn't throw. That's a failure.
        throw std::runtime_error("Test 5 FAILED: dequeue() on empty queue did not throw exception.");
    }
    catch (const std::out_of_range& e) {
        // This is the expected good behavior
        std::cout << "Test 5 Passed: dequeue() on empty correctly threw: " << e.what() << std::endl;
    }

    std::cout << "--- ListQueue Tests Passed ---" << std::endl;
}


/**
 * Runs all unit tests for the Tokenizer class.
 * This function *requires* the 'documents' folder to exist
 * in the same directory as the executable.
 */
void testTokenizer() {
    std::cout << "\n--- Testing Tokenizer ---" << std::endl;
    Tokenizer tokenizer;

    // Helper lambda to compare a list to an expected vector
    auto checkList = [](const LinkedList<std::string>& list, const std::vector<std::string>& expected, const std::string& testName) {
        if (list.size() != expected.size()) {
            throw std::runtime_error("Test FAILED: " + testName + " - size mismatch.");
        }
        for (const auto& word : expected) {
            if (!list.find(word)) {
                throw std::runtime_error("Test FAILED: " + testName + " - missing word: " + word);
            }
        }
        std::cout << "Test Passed: " << testName << " - Output: ";
        list.print();
        };

    // Test 1: doc1.txt
    LinkedList<std::string> doc1 = tokenizer.processFile("documents/doc1.txt");
    checkList(doc1, { "the", "quick", "brown", "fox" }, "doc1.txt");

    // Test 2: doc2.txt
    LinkedList<std::string> doc2 = tokenizer.processFile("documents/doc2.txt");
    checkList(doc2, { "the", "quick", "dog", "and", "the", "lazy", "fox" }, "doc2.txt");

    // Test 3: doc3.txt (The "c++" test)
    LinkedList<std::string> doc3 = tokenizer.processFile("documents/doc3.txt");
    checkList(doc3, { "a", "fast", "c++", "program", "is", "a", "good", "c++", "program" }, "doc3.txt");

    // Test 4: doc4.txt
    LinkedList<std::string> doc4 = tokenizer.processFile("documents/doc4.txt");
    checkList(doc4, { "hello", "world", "hello", "world" }, "doc4.txt");

    // Test 5: empty.txt
    LinkedList<std::string> empty = tokenizer.processFile("documents/empty.txt");
    checkList(empty, {}, "empty.txt");

    // Test 6: Missing file
    try {
        tokenizer.processFile("documents/MISSING_FILE.txt");
        throw std::runtime_error("Test 6 FAILED: Processing a missing file did not throw exception.");
    }
    catch (const std::runtime_error& e) {
        std::cout << "Test 6 Passed: Missing file correctly threw: " << e.what() << std::endl;
    }

    std::cout << "--- Tokenizer Tests Passed ---" << std::endl;
}

// --- Helper function to print a vector ---
template <typename T>
void printVector(const std::vector<T>& vec, const std::string& title) {
    std::cout << title << " (" << vec.size() << " items): [";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i < vec.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}

// --- Helper function to check if a vector is sorted correctly ---
bool isSorted(const std::vector<std::string>& vec, const std::vector<std::string>& expected) {
    if (vec.size() != expected.size()) {
        return false;
    }
    for (size_t i = 0; i < vec.size(); ++i) {
        if (vec[i] != expected[i]) {
            return false;
        }
    }
    return true;
}


/**
 * Runs all unit tests for the Sorting algorithms.
 */
void testSorting() {
    std::cout << "\n--- Testing Sorting Algorithms ---" << std::endl;

    // Define our test cases
    std::vector<std::vector<std::string>> testInputs = {
        {},                                 // Empty
        {"a"},                              // Single element
        {"a", "b", "c"},                    // Pre-sorted
        {"c", "b", "a"},                    // Reverse-sorted
        {"c", "a", "c", "b", "a"},          // Duplicates
        {"foo", "bar", "baz", "apple", "zebra", "banana"} // Random
    };

    // Define the expected (correctly sorted) outputs
    std::vector<std::vector<std::string>> expectedOutputs = {
        {},
        {"a"},
        {"a", "b", "c"},
        {"a", "b", "c"},
        {"a", "a", "b", "c", "c"},
        {"apple", "banana", "bar", "baz", "foo", "zebra"}
    };

    // --- Test Merge Sort ---
    std::cout << "Testing Sorting::mergeSort..." << std::endl;
    for (size_t i = 0; i < testInputs.size(); ++i) {
        // Create a copy of the input to sort
        std::vector<std::string> vecToSort = testInputs[i];

        // --- Timing ---
        auto start = std::chrono::high_resolution_clock::now();
        Sorting::mergeSort(vecToSort); // Call the sort
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::micro> duration = end - start;

        // --- Verification ---
        if (!isSorted(vecToSort, expectedOutputs[i])) {
            printVector(testInputs[i], "Test FAILED (Merge Sort) on input:");
            printVector(vecToSort, "  Got:");
            printVector(expectedOutputs[i], "  Expected:");
            throw std::runtime_error("Merge Sort failed!");
        }
        printVector(vecToSort, "  Test Passed");
        std::cout << "    (Took " << duration.count() << " microseconds)" << std::endl;
    }
    std::cout << "--- Merge Sort Tests Passed ---" << std::endl;


    // --- Test Quick Sort ---
    std::cout << "\nTesting Sorting::quickSort..." << std::endl;
    for (size_t i = 0; i < testInputs.size(); ++i) {
        // Create a copy of the input to sort
        std::vector<std::string> vecToSort = testInputs[i];

        // --- Timing ---
        auto start = std::chrono::high_resolution_clock::now();
        Sorting::quickSort(vecToSort); // Call the sort
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::micro> duration = end - start;

        // --- Verification ---
        if (!isSorted(vecToSort, expectedOutputs[i])) {
            printVector(testInputs[i], "Test FAILED (Quick Sort) on input:");
            printVector(vecToSort, "  Got:");
            printVector(expectedOutputs[i], "  Expected:");
            throw std::runtime_error("Quick Sort failed!");
        }
        printVector(vecToSort, "  Test Passed");
        std::cout << "    (Took " << duration.count() << " microseconds)" << std::endl;
    }
    std::cout << "--- Quick Sort Tests Passed ---" << std::endl;

    std::cout << "--- All Sorting Tests Passed ---" << std::endl;
}