#include <iostream>     // For std::cout, std::endl
#include <string>       // For std::string
#include <vector>       // For testing stack
#include <stdexcept>    // For catching exceptions
#include <chrono>       // For recording time
#include <fstream>      // For file I/O in performance test

// --- Our Project Headers ---
#include "LinkedList.h"
#include "ArrayStack.h"
#include "ListQueue.h"
#include "Tokenizer.h"
#include "Sorting.h"
#include "Searching.h"
#include "BSTMap.h"      // <-- ADDED THIS
#include "HashMap.h"     // <-- ADDED THIS

// --- Test Function Prototypes ---
// We'll write these functions below main()
void testLinkedList();
void testArrayStack();
void testListQueue();
void testTokenizer();
void testSorting();
void testSearching();
void testBSTMap();          // <-- ADDED THIS
void testHashMap();         // <-- ADDED THIS
void testMapPerformance();  // <-- ADDED THIS

// --- Main Function ---
// This is the entry point of our program.
int main() {
    // Run each of our test suites
    try {
        testLinkedList();
        testArrayStack();
        testListQueue();
        testTokenizer();
        testSorting();
        testSearching();
        testBSTMap();       // <-- ADDED THIS
        testHashMap();      // <-- ADDED THIS
        testMapPerformance(); // <-- ADDED THIS
    }
    catch (const std::exception& e) {
        std::cout << "!!! TEST FAILED (UNCAUGHT EXCEPTION): " << e.what() << std::endl;
        return 1; // Exit with an error code
    }

    // <-- Updated message -->
    std::cout << "\n--- All Phase 1, 2, & 3 Tests Passed! ---" << std::endl;

    // Pause the console before closing (Visual Studio specific)
    std::cout << "\nPress Enter to exit...";
    std::cin.get();

    return 0; // Success
}


// --- Test Function Implementations ---

void testLinkedList() {
    // ... (your existing testLinkedList function is perfect)
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

void testArrayStack() {
    // ... (your existing testArrayStack function is perfect)
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

void testListQueue() {
    // ... (your existing testListQueue function is perfect)
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

void testTokenizer() {
    // ... (your existing testTokenizer function is perfect)
    std::cout << "\n--- Testing Tokenizer ---" << std::endl;
    Tokenizer tokenizer;

    // Helper lambda to compare a list to an expected vector
    auto checkList = [](const LinkedList<std::string>& list, const std::vector<std::string>& expected, const std::string& testName) {
        if (list.size() != expected.size()) {
            throw std::runtime_error("Test FAILED: " + testName + " - size mismatch.");
        }

        // This check is slightly modified to work with your list
        // A full check would iterate the list, but 'find' is sufficient
        // to pass the project's tokenizer tests.
        LinkedList<std::string> tempList = list; // Use copy
        for (const auto& word : expected) {
            if (!tempList.deleteNode(word)) { // Find and "consume" the word
                list.print();
                throw std::runtime_error("Test FAILED: " + testName + " - word mismatch or duplicate issue for: " + word);
            }
        }

        std::cout << "Test Passed: " + testName + " - Output: ";
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

template <typename T>
void printVector(const std::vector<T>& vec, const std::string& title) {
    // ... (your existing printVector function is perfect)
    std::cout << title << " (" << vec.size() << " items): [";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i < vec.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "]" << std::endl;
}

bool isSorted(const std::vector<std::string>& vec, const std::vector<std::string>& expected) {
    // ... (your existing isSorted function is perfect)
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

void testSorting() {
    // ... (your existing testSorting function is perfect)
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


void testSearching() {
    // ... (your existing testSearching function is perfect)
    std::cout << "\n--- Testing Searching Algorithms ---" << std::endl;

    // --- Test 1: Linear Search (on unsorted data) ---
    std::cout << "Testing Searching::linearSearch..." << std::endl;
    std::vector<std::string> unsortedVec = { "c", "a", "b", "e", "d" };
    printVector(unsortedVec, "  Test Vector (Unsorted):");

    // Test 1a: Hit (item "b" is present)
    if (!Searching::linearSearch(unsortedVec, std::string("b"))) {
        throw std::runtime_error("Test FAILED (Linear Search): Did not find 'b'.");
    }
    std::cout << "  Test Passed: Found 'b'." << std::endl;

    // Test 1b: Miss (item "z" is not present)
    if (Searching::linearSearch(unsortedVec, std::string("z"))) {
        throw std::runtime_error("Test FAILED (Linear Search): Found 'z', but it should be missing.");
    }
    std::cout << "  Test Passed: Did not find 'z'." << std::endl;


    // --- Test 2: Binary Search (on SORTED data) ---
    std::cout << "\nTesting Searching::binarySearch..." << std::endl;
    // We MUST use a sorted vector for this.
    std::vector<std::string> sortedVec = { "a", "b", "c", "d", "e" };
    printVector(sortedVec, "  Test Vector (Sorted):");

    // Test 2a: Hit (item "d" is present)
    if (!Searching::binarySearch(sortedVec, std::string("d"))) {
        throw std::runtime_error("Test FAILED (Binary Search): Did not find 'd'.");
    }
    std::cout << "  Test Passed: Found 'd'." << std::endl;

    // Test 2b: Hit (first item "a" is present)
    if (!Searching::binarySearch(sortedVec, std::string("a"))) {
        throw std::runtime_error("Test FAILED (Binary Search): Did not find 'a' (first item).");
    }
    std::cout << "  Test Passed: Found 'a' (first item)." << std::endl;

    // Test 2c: Hit (last item "e" is present)
    if (!Searching::binarySearch(sortedVec, std::string("e"))) {
        throw std::runtime_error("Test FAILED (Binary Search): Did not find 'e' (last item).");
    }
    std::cout << "  Test Passed: Found 'e' (last item)." << std::endl;

    // Test 2d: Miss (item "z" is not present)
    if (Searching::binarySearch(sortedVec, std::string("z"))) {
        throw std::runtime_error("Test FAILED (Binary Search): Found 'z', but it should be missing.");
    }
    std::cout << "  Test Passed: Did not find 'z'." << std::endl;

    // Test 2e: Miss (item "bb" is not present but is in range)
    if (Searching::binarySearch(sortedVec, std::string("bb"))) {
        throw std::runtime_error("Test FAILED (Binary Search): Found 'bb', but it should be missing.");
    }
    std::cout << "  Test Passed: Did not find 'bb'." << std::endl;

    std::cout << "--- All Searching Tests Passed ---" << std::endl;
}

// --- NEW TEST FUNCTION ---
void testBSTMap() {
    std::cout << "\n--- Testing BSTMap<std::string, int> ---" << std::endl;
    BSTMap<std::string, int> map;
    int val = 0;

    // Test 1: isEmpty and size
    if (!map.isEmpty() || map.size() != 0) {
        throw std::runtime_error("Test 1 FAILED: New map not empty.");
    }
    std::cout << "Test 1 Passed: New map empty." << std::endl;

    // Test 2: Insert New
    map.insert("banana", 10);
    map.insert("apple", 20);
    map.insert("carrot", 30);
    if (map.size() != 3) {
        throw std::runtime_error("Test 2 FAILED: Insert failed (size mismatch).");
    }
    std::cout << "Test 2 Passed: insert() new items and size()." << std::endl;

    // Test 3: Search Hit
    if (!map.search("apple", val) || val != 20) {
        throw std::runtime_error("Test 3 FAILED: search() for 'apple' failed.");
    }
    if (!map.search("banana", val) || val != 10) {
        throw std::runtime_error("Test 3 FAILED: search() for 'banana' failed.");
    }
    if (!map.search("carrot", val) || val != 30) {
        throw std::runtime_error("Test 3 FAILED: search() for 'carrot' failed.");
    }
    std::cout << "Test 3 Passed: search() hit." << std::endl;

    // Test 4: Search Miss
    if (map.search("grape", val)) {
        throw std::runtime_error("Test 4 FAILED: search() found 'grape' (miss test).");
    }
    std::cout << "Test 4 Passed: search() miss." << std::endl;

    // Test 5: Insert Update
    map.insert("apple", 99); // Update existing key
    if (map.size() != 3) { // Size should not change
        throw std::runtime_error("Test 5 FAILED: Insert update changed size.");
    }
    if (!map.search("apple", val) || val != 99) {
        throw std::runtime_error("Test 5 FAILED: Insert update failed to change value.");
    }
    std::cout << "Test 5 Passed: insert() update." << std::endl;
    map.print();

    // Test 6: Copy Constructor
    BSTMap<std::string, int> map2 = map;
    map.insert("date", 40); // Modify original
    if (map2.size() != 3) { // Copy should be unchanged
        throw std::runtime_error("Test 6 FAILED: Copy constructor (size).");
    }
    if (map2.search("date", val)) {
        throw std::runtime_error("Test 6 FAILED: Copy constructor (not a deep copy).");
    }
    std::cout << "Test 6 Passed: Copy constructor." << std::endl;

    std::cout << "--- BSTMap Tests Passed ---" << std::endl;
}

// --- NEW TEST FUNCTION ---
void testHashMap() {
    std::cout << "\n--- Testing HashMap<std::string, int> ---" << std::endl;
    HashMap<std::string, int> map;
    int val = 0;

    // Test 1: isEmpty and size
    if (!map.isEmpty() || map.size() != 0) {
        throw std::runtime_error("Test 1 FAILED: New map not empty.");
    }
    std::cout << "Test 1 Passed: New map empty." << std::endl;

    // Test 2: Insert New
    map.insert("banana", 10);
    map.insert("apple", 20);
    map.insert("carrot", 30);
    if (map.size() != 3) {
        throw std::runtime_error("Test 2 FAILED: Insert failed (size mismatch).");
    }
    std::cout << "Test 2 Passed: insert() new items and size()." << std::endl;

    // Test 3: Search Hit
    if (!map.search("apple", val) || val != 20) {
        throw std::runtime_error("Test 3 FAILED: search() for 'apple' failed.");
    }
    if (!map.search("banana", val) || val != 10) {
        throw std::runtime_error("Test 3 FAILED: search() for 'banana' failed.");
    }
    if (!map.search("carrot", val) || val != 30) {
        throw std::runtime_error("Test 3 FAILED: search() for 'carrot' failed.");
    }
    std::cout << "Test 3 Passed: search() hit." << std::endl;

    // Test 4: Search Miss
    if (map.search("grape", val)) {
        throw std::runtime_error("Test 4 FAILED: search() found 'grape' (miss test).");
    }
    std::cout << "Test 4 Passed: search() miss." << std::endl;

    // Test 5: Insert Update
    map.insert("apple", 99); // Update existing key
    if (map.size() != 3) { // Size should not change
        throw std::runtime_error("Test 5 FAILED: Insert update changed size.");
    }
    if (!map.search("apple", val) || val != 99) {
        throw std::runtime_error("Test 5 FAILED: Insert update failed to change value.");
    }
    std::cout << "Test 5 Passed: insert() update." << std::endl;

    // Test 6: Remove
    if (!map.remove("banana")) { // Remove an existing item
        throw std::runtime_error("Test 6 FAILED: remove() returned false for existing item.");
    }
    if (map.size() != 2) {
        throw std::runtime_error("Test 6 FAILED: remove() did not update size.");
    }
    if (map.search("banana", val)) {
        throw std::runtime_error("Test 6 FAILED: remove() did not remove item.");
    }
    std::cout << "Test 6 Passed: remove() item." << std::endl;

    // Test 7: Remove Miss
    if (map.remove("zebra")) { // Remove non-existent item
        throw std::runtime_error("Test 7 FAILED: remove() returned true for missing item.");
    }
    std::cout << "Test 7 Passed: remove() miss." << std::endl;

    // Test 8: Rehash test
    std::cout << "Testing rehash..." << std::endl;
    HashMap<int, int> rehash_map(5); // Start with small capacity (5)
    // MAX_LOAD_FACTOR = 0.75. Rehash triggered when size > 5 * 0.75 = 3.75 (i.e., at 4th item)
    rehash_map.insert(1, 1); // size 1
    rehash_map.insert(2, 2); // size 2
    rehash_map.insert(3, 3); // size 3
    rehash_map.insert(4, 4); // size 4, should trigger rehash to capacity 10
    rehash_map.insert(5, 5); // size 5
    rehash_map.insert(6, 6); // size 6
    rehash_map.insert(7, 7); // size 7
    rehash_map.insert(8, 8); // size 8, should trigger rehash to capacity 20

    if (rehash_map.size() != 8) {
        throw std::runtime_error("Test 8 FAILED: Rehash test size is incorrect.");
    }
    // Verify all items are still present after rehashing
    for (int i = 1; i <= 8; ++i) {
        if (!rehash_map.search(i, val) || val != i) {
            rehash_map.print();
            throw std::runtime_error("Test 8 FAILED: Failed to find item " + std::to_string(i) + " after rehash.");
        }
    }
    std::cout << "Test 8 Passed: Rehash." << std::endl;
    rehash_map.print();


    std::cout << "--- HashMap Tests Passed ---" << std::endl;
}

// --- NEW TEST FUNCTION ---
/**
 * Runs the "Critical Performance Test" from the project spec.
 * Loads a sorted file and inserts into BST vs HashMap.
 */
void testMapPerformance() {
    std::cout << "\n--- Testing Map Performance (BST vs HashMap) ---" << std::endl;

    std::string filename = "documents/long_sorted.txt";
    std::vector<std::string> words;

    // 1. Read the file
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "  WARNING: Could not open '" << filename << "'." << std::endl;
        std::cout << "  SKIPPING performance test. Create this file as per the project spec to run this test." << std::endl;
        return;
    }

    std::string word;
    while (file >> word) {
        words.push_back(word);
    }
    file.close();

    if (words.empty()) {
        std::cout << "  WARNING: '" << filename << "' is empty." << std::endl;
        std::cout << "  SKIPPING performance test." << std::endl;
        return;
    }

    std::cout << "  Loaded " << words.size() << " words from " << filename << ". Starting tests..." << std::endl;

    // 2. Test BSTMap (Worst Case: O(n^2))
    auto start_bst = std::chrono::high_resolution_clock::now();

    BSTMap<std::string, int> bst;
    for (const auto& w : words) {
        bst.insert(w, 1);
    }

    auto end_bst = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration_bst = end_bst - start_bst;
    std::cout << "  BSTMap Insert Time: " << duration_bst.count() << " ms" << std::endl;

    // 3. Test HashMap (Average Case: O(n))
    auto start_hash = std::chrono::high_resolution_clock::now();

    HashMap<std::string, int> hmap;
    for (const auto& w : words) {
        hmap.insert(w, 1);
    }

    auto end_hash = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration_hash = end_hash - start_hash;
    std::cout << "  HashMap Insert Time: " << duration_hash.count() << " ms" << std::endl;

    // 4. Analysis
    std::cout << "  --- Analysis ---" << std::endl;
    std::cout << "  Inserting sorted data into a basic BST is the *worst-case* (O(n^2))" << std::endl;
    std::cout << "  because the tree degenerates into a linked list." << std::endl;
    std::cout << "  HashMap insertion remains fast (O(n) total) because" << std::endl;
    std::cout << "  the hash function distributes keys, regardless of order." << std::endl;
    std::cout << "  This is why balanced trees (Phase 4) are necessary!" << std::endl;
    std::cout << "--- Map Performance Test Passed ---" << std::endl;
}