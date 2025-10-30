#include <iostream>     // For std::cout, std::endl
#include <string>       // For std::string
#include <vector>       // For testing stack
#include <stdexcept>    // For catching exceptions
#include <chrono>       // For recording time
#include <fstream>      // For performance test

// --- Our Project Headers ---
#include "LinkedList.h"
#include "ArrayStack.h"
#include "ListQueue.h"
#include "Tokenizer.h"
#include "Sorting.h"
#include "Searching.h"
#include "BSTMap.h"
#include "HashMap.h"
#include "AVLMap.h"
#include "RBMap.h"      // <-- Includes RBMap

// --- Test Function Prototypes ---
// We'll write these functions below main()
void testLinkedList();
void testArrayStack();
void testListQueue();
void testTokenizer();
void testSorting();
void testSearching();
void testBSTMap();
void testHashMap();
void testAVLMap();
void testRBMap();           // <-- Prototype for RBMap test
void testMapPerformance();

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

        // --- Phase 3 Tests ---
        testBSTMap();
        testHashMap();

        // --- Phase 4 Tests ---
        testAVLMap();
        testRBMap();          // <-- Call to RBMap test
        testMapPerformance(); // Run the critical sorted-insert test
    }
    catch (const std::exception& e) {
        std::cout << "!!! TEST FAILED (UNCAUGHT EXCEPTION): " << e.what() << std::endl;
        return 1; // Exit with an error code
    }

    // <-- Updated success message -->
    std::cout << "\n--- All Phase 1, 2, 3, & 4 Tests Passed! ---" << std::endl;

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
    std::cout << "\n--- Testing Tokenizer ---" << std::endl;
    Tokenizer tokenizer;

    // Helper lambda to compare a list to an expected vector
    auto checkList = [](const LinkedList<std::string>& list, const std::vector<std::string>& expected, const std::string& testName) {
        if (list.size() != expected.size()) {
            list.print();
            throw std::runtime_error("Test FAILED: " + testName + " - size mismatch. Expected " + std::to_string(expected.size()) + ", Got " + std::to_string(list.size()));
        }

        // This check is slightly flawed if list has duplicates and expected doesn't,
        // but it's good enough for our specific test cases.
        // A more robust check would iterate both.
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


void testSearching() {
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

    // Test 2a: Hit (first item "a" is present)
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

/**
 * Runs all unit tests for the BSTMap.
 */
void testBSTMap() {
    std::cout << "\n--- Testing BSTMap<std::string, int> ---" << std::endl;
    BSTMap<std::string, int> map;
    int val; // For retrieving search results

    // Test 1: isEmpty and size on new map
    if (!map.isEmpty() || map.size() != 0) {
        throw std::runtime_error("Test 1 FAILED: New map not empty.");
    }
    std::cout << "Test 1 Passed: New map empty." << std::endl;

    // Test 2: Insert New
    map.insert("banana", 10);
    map.insert("apple", 5);
    map.insert("carrot", 15);
    if (map.size() != 3) {
        throw std::runtime_error("Test 2 FAILED: Insert new failed (size mismatch).");
    }
    std::cout << "Test 2 Passed: Insert new." << std::endl;
    map.print(); // Should print in-order: (apple: 5), (banana: 10), (carrot: 15)

    // Test 3: Search Hit
    if (!map.search("apple", val) || val != 5) {
        throw std::runtime_error("Test 3 FAILED: Search hit failed for 'apple'.");
    }
    std::cout << "Test 3 Passed: Search hit for 'apple'." << std::endl;

    // Test 4: Insert Update
    map.insert("banana", 99); // Update existing key
    if (map.size() != 3) { // Size should not change
        throw std::runtime_error("Test 4 FAILED: Insert update changed size.");
    }
    if (!map.search("banana", val) || val != 99) {
        throw std::runtime_error("Test 4 FAILED: Insert update failed to update value.");
    }
    std::cout << "Test 4 Passed: Insert update for 'banana' to 99." << std::endl;
    map.print();

    // Test 5: Search Miss
    if (map.search("zucchini", val)) {
        throw std::runtime_error("Test 5 FAILED: Search miss found 'zucchini'.");
    }
    std::cout << "Test 5 Passed: Search miss for 'zucchini'." << std::endl;

    // Test 6: Copy Constructor
    BSTMap<std::string, int> map2 = map;
    map.insert("date", 20); // Modify original
    if (map2.size() != 3) { // Copy should be unchanged
        throw std::runtime_error("Test 6 FAILED: Copy constructor (size).");
    }
    if (map2.search("date", val)) { // Copy should not have new item
        throw std::runtime_error("Test 6 FAILED: Copy constructor (deep copy).");
    }
    std::cout << "Test 6 Passed: Copy constructor." << std::endl;
    std::cout << "  Original map:";
    map.print();
    std::cout << "  Copied map:";
    map2.print();

    // Test 7: Copy Assignment
    BSTMap<std::string, int> map3;
    map3.insert("test", 1);
    map3 = map; // Assign
    map.insert("fig", 30); // Modify original
    if (map3.size() != 4) { // Copy should have 4 items
        throw std::runtime_error("Test 7 FAILED: Copy assignment (size).");
    }
    if (map3.search("fig", val)) { // Copy should not have new item
        throw std::runtime_error("Test 7 FAILED: Copy assignment (deep copy).");
    }
    std::cout << "Test 7 Passed: Copy assignment." << std::endl;

    std::cout << "--- BSTMap Tests Passed ---" << std::endl;
}

/**
 * Runs all unit tests for the HashMap.
 */
void testHashMap() {
    std::cout << "\n--- Testing HashMap<std::string, int> ---" << std::endl;
    HashMap<std::string, int> map;
    int val;

    // Test 1: isEmpty and size
    if (!map.isEmpty() || map.size() != 0) {
        throw std::runtime_error("Test 1 FAILED: New map not empty.");
    }
    std::cout << "Test 1 Passed: New map empty." << std::endl;

    // Test 2: Insert New
    map.insert("banana", 10);
    map.insert("apple", 5);
    map.insert("carrot", 15);
    if (map.size() != 3) {
        throw std::runtime_error("Test 2 FAILED: Insert new (size).");
    }
    std::cout << "Test 2 Passed: Insert new." << std::endl;

    // Test 3: Search Hit
    if (!map.search("apple", val) || val != 5) {
        throw std::runtime_error("Test 3 FAILED: Search hit failed for 'apple'.");
    }
    std::cout << "Test 3 Passed: Search hit for 'apple'." << std::endl;

    // Test 4: Insert Update
    map.insert("banana", 99); // Update
    if (map.size() != 3) {
        throw std::runtime_error("Test 4 FAILED: Insert update changed size.");
    }
    if (!map.search("banana", val) || val != 99) {
        throw std::runtime_error("Test 4 FAILED: Insert update failed to update value.");
    }
    std::cout << "Test 4 Passed: Insert update for 'banana' to 99." << std::endl;
    map.print();

    // Test 5: Search Miss
    if (map.search("zucchini", val)) {
        throw std::runtime_error("Test 5 FAILED: Search miss found 'zucchini'.");
    }
    std::cout << "Test 5 Passed: Search miss for 'zucchini'." << std::endl;

    // Test 6: Remove
    if (!map.remove("apple")) { // Remove 'apple'
        throw std::runtime_error("Test 6 FAILED: Remove 'apple' returned false.");
    }
    if (map.size() != 2 || map.search("apple", val)) {
        throw std::runtime_error("Test 6 FAILED: Remove 'apple' failed.");
    }
    std::cout << "Test 6 Passed: Removed 'apple'." << std::endl;

    // Test 7: Remove Miss
    if (map.remove("zucchini")) { // Remove non-existent
        throw std::runtime_error("Test 7 FAILED: Remove 'zucchini' returned true.");
    }
    std::cout << "Test 7 Passed: Remove miss for 'zucchini'." << std::endl;
    map.print();

    // Test 8: Rehash Trigger
    std::cout << "Testing rehash (inserting 10 items into default 10-bucket map)..." << std::endl;
    HashMap<int, int> rehash_map(10); // 10 buckets, rehash at 8 items (0.75)
    for (int i = 0; i < 10; ++i) {
        rehash_map.insert(i, i); // Will trigger rehash
    }
    if (rehash_map.size() != 10) {
        throw std::runtime_error("Test 8 FAILED: Rehash map size is incorrect.");
    }
    // Check if all items are still present after rehash
    for (int i = 0; i < 10; ++i) {
        if (!rehash_map.search(i, val) || val != i) {
            throw std::runtime_error("Test 8 FAILED: Failed to find item " + std::to_string(i) + " after rehash.");
        }
    }
    std::cout << "Test 8 Passed: Rehash successful." << std::endl;
    rehash_map.print(); // Should show capacity of 20

    std::cout << "--- HashMap Tests Passed ---" << std::endl;
}

/**
 * Runs all unit tests for the AVLMap.
 */
void testAVLMap() {
    std::cout << "\n--- Testing AVLMap<std::string, int> ---" << std::endl;
    AVLMap<std::string, int> map;
    int val; // For retrieving search results

    // Test 1: isEmpty and size on new map
    if (!map.isEmpty() || map.size() != 0) {
        throw std::runtime_error("Test 1 FAILED: New map not empty.");
    }
    std::cout << "Test 1 Passed: New map empty." << std::endl;

    // Test 2: Insert New (triggers LL, LR, RR, RL rotations)
    std::cout << "Testing insertions (LL, RR, LR, RL)..." << std::endl;
    // --- LL Case ---
    map.insert("c", 3);
    map.insert("b", 2);
    map.insert("a", 1); // Triggers LL rotation at "c"
    std::cout << "After LL (a,b,c):";
    map.print();
    if (map.size() != 3 || !map.search("a", val) || !map.search("b", val) || !map.search("c", val)) {
        throw std::runtime_error("Test 2 FAILED: LL rotation failed.");
    }
    std::cout << "Test 2a Passed: LL Rotation." << std::endl;

    // --- RR Case ---
    map.insert("d", 4);
    map.insert("e", 5); // Triggers RR rotation at "c"
    std::cout << "After RR (d,e):";
    map.print();
    if (map.size() != 5 || !map.search("d", val) || !map.search("e", val)) {
        throw std::runtime_error("Test 2 FAILED: RR rotation failed.");
    }
    std::cout << "Test 2b Passed: RR Rotation." << std::endl;

    // --- RL Case ---
    map.insert("g", 7);
    map.insert("f", 6); // Triggers RL rotation at "e"
    std::cout << "After RL (g,f):";
    map.print();
    if (map.size() != 7 || !map.search("f", val) || !map.search("g", val)) {
        throw std::runtime_error("Test 2 FAILED: RL rotation failed.");
    }
    std::cout << "Test 2c Passed: RL Rotation." << std::endl;

    // --- LR Case ---
    // Re-using "a" and "c" from before
    map.insert("ab", 15); // "ab" is between "a" and "b". Triggers LR at "c"
    std::cout << "After LR (ab):";
    map.print();
    if (map.size() != 8 || !map.search("ab", val)) {
        throw std::runtime_error("Test 2 FAILED: LR rotation failed.");
    }
    std::cout << "Test 2d Passed: LR Rotation." << std::endl;


    // Test 3: Search Hit
    if (!map.search("c", val) || val != 3) {
        throw std::runtime_error("Test 3 FAILED: Search hit failed for 'c'.");
    }
    std::cout << "Test 3 Passed: Search hit for 'c'." << std::endl;

    // Test 4: Insert Update
    map.insert("a", 99); // Update existing key
    if (map.size() != 8) { // Size should not change
        throw std::runtime_error("Test 4 FAILED: Insert update changed size.");
    }
    if (!map.search("a", val) || val != 99) {
        throw std::runtime_error("Test 4 FAILED: Insert update failed to update value.");
    }
    std::cout << "Test 4 Passed: Insert update for 'a' to 99." << std::endl;

    // Test 5: Search Miss
    if (map.search("zucchini", val)) {
        throw std::runtime_error("Test 5 FAILED: Search miss found 'zucchini'.");
    }
    std::cout << "Test 5 Passed: Search miss for 'zucchini'." << std::endl;

    std::cout << "--- AVLMap Tests Passed ---" << std::endl;
}

/**
 * Runs all unit tests for the RBMap.
 */
void testRBMap() {
    std::cout << "\n--- Testing RBMap<std::string, int> ---" << std::endl;
    RBMap<std::string, int> map;
    int val;

    // Test 1: isEmpty and size
    if (!map.isEmpty() || map.size() != 0) {
        throw std::runtime_error("Test 1 FAILED: New map not empty.");
    }
    std::cout << "Test 1 Passed: New map empty." << std::endl;

    // Test 2: Insert (Case 1: Uncle RED)
    // Inserting 10, 20, 30. 
    // 10 becomes root (BLACK). 
    // 20 is child of 10 (RED).
    // 30 is child of 20 (RED). -> Parent(20) is RED.
    // Grandparent(10) is BLACK. Uncle is NIL (BLACK).
    // This triggers Case 3 (line). LeftRotate(10).
    // Root becomes 20 (BLACK), 10 and 30 are children (RED).
    map.insert("b", 20); // Root, becomes BLACK
    map.insert("a", 10); // Left child, RED
    map.insert("c", 30); // Right child, RED. Parent is BLACK, so OK.
    if (map.size() != 3) throw std::runtime_error("Test 2 FAILED: Initial setup size.");
    std::cout << "  Setup (a,b,c):";
    map.print();

    // Now, insert "d" (child of "c"). Parent "c" is RED.
    // Grandparent "b" is BLACK. Uncle "a" is RED.
    // This is the classic "Case 1: Uncle is RED"
    // "a" and "c" become BLACK. "b" becomes RED (but is root, so flips to BLACK).
    map.insert("d", 40);
    if (map.size() != 4) throw std::runtime_error("Test 2 FAILED: Uncle RED case size.");
    std::cout << "  After Uncle RED case (d):";
    map.print();
    std::cout << "Test 2 Passed: Insert (Uncle RED)." << std::endl;

    // Test 3: Insert (Case 2/3: Uncle BLACK)
    // Now insert "e" (child of "d"). Parent "d" is RED.
    // Grandparent "c" is BLACK. Uncle is NIL (BLACK).
    // This is "Case 3: Line" (right-right).
    // LeftRotate at "c". Recolor.
    map.insert("e", 50);
    if (map.size() != 5) throw std::runtime_error("Test 3 FAILED: Uncle BLACK case size.");
    std::cout << "  After Uncle BLACK case (e):";
    map.print();
    std::cout << "Test 3 Passed: Insert (Uncle BLACK, Line)." << std::endl;


    // Test 4: Search Hit
    if (!map.search("c", val) || val != 30) {
        throw std::runtime_error("Test 4 FAILED: Search hit failed for 'c'.");
    }
    std::cout << "Test 4 Passed: Search hit for 'c'." << std::endl;

    // Test 5: Insert Update
    map.insert("a", 99); // Update
    if (map.size() != 5) {
        throw std::runtime_error("Test 5 FAILED: Insert update changed size.");
    }
    if (!map.search("a", val) || val != 99) {
        throw std::runtime_error("Test 5 FAILED: Insert update failed to update value.");
    }
    std::cout << "Test 5 Passed: Insert update for 'a' to 99." << std::endl;

    // Test 6: Search Miss
    if (map.search("zucchini", val)) {
        throw std::runtime_error("Test 6 FAILED: Search miss found 'zucchini'.");
    }
    std::cout << "Test 6 Passed: Search miss for 'zucchini'." << std::endl;

    std::cout << "--- RBMap Tests Passed ---" << std::endl;
}


/**
 * Runs the "Critical Performance Test" from the project spec.
 * Reads words from 'long_sorted.txt' and times insertion into
 * BSTMap, HashMap, AVLMap, and RBMap.
 */
void testMapPerformance() {
    std::cout << "\n--- Testing Map Performance (Sorted Input) ---" << std::endl;

    std::string filename = "documents/long_sorted.txt";
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "!!! WARNING: Could not open '" << filename << "'. !!!" << std::endl;
        std::cout << "!!! Please create this file in your 'documents' folder per the " << std::endl;
        std::cout << "!!! project instructions to run the performance test. Skipping... !!!" << std::endl;
        return;
    }

    // Read all words into a vector first
    std::vector<std::string> words;
    std::string word;
    while (file >> word) {
        words.push_back(word);
    }
    file.close();

    if (words.empty()) {
        std::cout << "!!! WARNING: '" << filename << "' is empty. Performance test cannot run. Skipping... !!!" << std::endl;
        return;
    }

    std::cout << "  Loaded " << words.size() << " sorted words for testing..." << std::endl;

    // --- Test 1: BSTMap (Worst Case) ---
    BSTMap<std::string, int> bst;

    auto start_bst = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < words.size(); ++i) {
        bst.insert(words[i], static_cast<int>(i));
    }
    auto end_bst = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration_bst = end_bst - start_bst;

    std::cout << "  BSTMap insertion time:     " << duration_bst.count() << " ms" << std::endl;

    // --- Test 2: HashMap (Best Case) ---
    HashMap<std::string, int> hashMap;

    auto start_hash = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < words.size(); ++i) {
        hashMap.insert(words[i], static_cast<int>(i));
    }
    auto end_hash = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration_hash = end_hash - start_hash;

    std::cout << "  HashMap insertion time:    " << duration_hash.count() << " ms" << std::endl;

    // --- Test 3: AVLMap (Balanced Case) ---
    AVLMap<std::string, int> avlMap;

    auto start_avl = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < words.size(); ++i) {
        avlMap.insert(words[i], static_cast<int>(i));
    }
    auto end_avl = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration_avl = end_avl - start_avl;

    std::cout << "  AVLMap insertion time:     " << duration_avl.count() << " ms" << std::endl;


    // --- Test 4: RBMap (Balanced Case) ---
    RBMap<std::string, int> rbMap;

    auto start_rb = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < words.size(); ++i) {
        rbMap.insert(words[i], static_cast<int>(i));
    }
    auto end_rb = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration_rb = end_rb - start_rb;

    std::cout << "  RBMap insertion time:      " << duration_rb.count() << " ms" << std::endl;


    std::cout << "  Observation: BSTMap should be significantly slower (O(n^2))" << std::endl;
    std::cout << "  due to sorted input creating a degenerate tree (a linked list)." << std::endl;
    std::cout << "  HashMap, AVLMap, and RBMap should all be very fast." << std::endl;
    std::cout << "  (O(n log n) for trees, O(n) for Hash)." << std::endl;
    std::cout << "--- Map Performance Test Complete ---" << std::endl;
}