#include <iostream>
#include <string>
#include <vector>
#include <fstream>      // For std::ifstream
#include <chrono>       // For timing
#include <algorithm>    // For std::shuffle
#include <random>       // For std::default_random_engine
#include <iomanip>      // For std::fixed and std::setprecision

// --- Include all your data structure headers ---
#include "Tokenizer.h"
#include "LinkedList.h"
#include "Sorting.h"
#include "Searching.h"
#include "BSTMap.h"
#include "HashMap.h"
#include "AVLMap.h"
#include "RBMap.h"

/**
 * A simple timer utility.
 * Create an instance to start the timer.
 * Call elapsed_nanoseconds() to get the time since creation.
 */
struct Timer {
    std::chrono::high_resolution_clock::time_point start_time;

    Timer() : start_time(std::chrono::high_resolution_clock::now()) {}

    // Returns elapsed time in nanoseconds
    long long elapsed_nanoseconds() {
        auto end_time = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
    }
};

/**
 * Loads words from a file into a vector.
 * We use this to get our test data.
 */
std::vector<std::string> loadWords(const std::string& filename) {
    std::vector<std::string> words;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "ERROR: Could not open test file: " << filename << std::endl;
        return words;
    }
    std::string word;
    while (file >> word) {
        words.push_back(word);
    }
    file.close();
    return words;
}

/**
 * Test Task: Sorting Algorithms
 * Compares Quick Sort vs. Merge Sort.
 */
void testSorting(const std::vector<std::string>& words) {
    std::cout << "\n--- Testing Sorting (on " << words.size() << " words) ---" << std::endl;

    // We must test on copies, as the sorts are in-place
    std::vector<std::string> vec_qs = words;
    std::vector<std::string> vec_ms = words;

    // Test Quick Sort
    Timer t_qs;
    Sorting::quickSort(vec_qs);
    long long qs_time_ns = t_qs.elapsed_nanoseconds();
    std::cout << "  Quick Sort Time:   " << (double)qs_time_ns / 1'000'000.0 << " ms" << std::endl;

    // Test Merge Sort
    Timer t_ms;
    Sorting::mergeSort(vec_ms);
    long long ms_time_ns = t_ms.elapsed_nanoseconds();
    std::cout << "  Merge Sort Time:   " << (double)ms_time_ns / 1'000'000.0 << " ms" << std::endl;
}

/**
 * Test Task: Searching Algorithms
 * Compares Linear Search vs. Binary Search.
 */
void testSearching(const std::vector<std::string>& unsortedWords, const std::vector<std::string>& sortedWords) {
    std::cout << "\n--- Testing Searching (" << sortedWords.size() << " words) ---" << std::endl;

    // A small list of items to find
    std::vector<std::string> searchTerms = {
        sortedWords[0],                                 // First item
        sortedWords[sortedWords.size() / 2],            // Middle item
        sortedWords[sortedWords.size() - 1],            // Last item
        "!!-NOT-IN-THE-LIST-!!"                       // Non-existent item
    };

    const int linear_loops = 100;
    // Test Linear Search (on unsorted data)
    Timer t_linear;
    for (int i = 0; i < linear_loops; ++i) { // Repeat to get a measurable time
        for (const auto& term : searchTerms) {
            Searching::linearSearch(unsortedWords, term);
        }
    }
    double avg_linear_ms = ((double)t_linear.elapsed_nanoseconds() / 1'000'000.0) / linear_loops;
    std::cout << "  Linear Search (avg): " << avg_linear_ms << " ms" << std::endl;

    const int binary_loops = 1000;
    // Test Binary Search (on *sorted* data)
    Timer t_binary;
    for (int i = 0; i < binary_loops; ++i) { // Repeat more times, it's much faster
        for (const auto& term : searchTerms) {
            Searching::binarySearch(sortedWords, term);
        }
    }
    double avg_binary_ms = ((double)t_binary.elapsed_nanoseconds() / 1'000'000.0) / binary_loops;
    std::cout << "  Binary Search (avg): " << avg_binary_ms << " ms" << std::endl;
}

/**
 * Generic helper to time the insertion of all words into any map type.
 * Returns time in nanoseconds.
 */
template <typename MapType>
long long timeMapInsertion(const std::vector<std::string>& words) {
    MapType map;
    Timer t;
    for (size_t i = 0; i < words.size(); ++i) {
        map.insert(words[i], static_cast<int>(i)); // Key=word, Value=index
    }
    return t.elapsed_nanoseconds();
}

/**
 * Generic helper to time searching for all words in a pre-built map.
 * Returns time in nanoseconds.
 */
template <typename MapType>
long long timeMapSearch(const MapType& map, const std::vector<std::string>& words) {
    int dummyValue; // To hold the search result
    Timer t;
    for (const auto& word : words) {
        map.search(word, dummyValue);
    }
    return t.elapsed_nanoseconds();
}

/**
 * Test Task: Map Insertion and Search
 * Runs all map comparisons for a given word list.
 */
void runMapTests(const std::vector<std::string>& words) {

    // --- 1. Test Insertion Time ---
    std::cout << "  --- Testing Map Insertion (" << words.size() << " words) ---" << std::endl;
    long long bst_i_ns = timeMapInsertion<BSTMap<std::string, int>>(words);
    long long hash_i_ns = timeMapInsertion<HashMap<std::string, int>>(words);
    long long avl_i_ns = timeMapInsertion<AVLMap<std::string, int>>(words);
    long long rb_i_ns = timeMapInsertion<RBMap<std::string, int>>(words);

    std::cout << "    BSTMap  Insertion: " << (double)bst_i_ns / 1'000'000.0 << " ms" << std::endl;
    std::cout << "    HashMap Insertion: " << (double)hash_i_ns / 1'000'000.0 << " ms" << std::endl;
    std::cout << "    AVLMap  Insertion: " << (double)avl_i_ns / 1'000'000.0 << " ms" << std::endl;
    std::cout << "    RBMap   Insertion: " << (double)rb_i_ns / 1'000'000.0 << " ms" << std::endl;

    // --- 2. Build Maps (untimed) ---
    // We build them first so we can *only* time the search.
    BSTMap<std::string, int> bstMap;
    HashMap<std::string, int> hashMap;
    AVLMap<std::string, int> avlMap;
    RBMap<std::string, int> rbMap;
    for (size_t i = 0; i < words.size(); ++i) {
        bstMap.insert(words[i], i);
        hashMap.insert(words[i], i);
        avlMap.insert(words[i], i);
        rbMap.insert(words[i], i);
    }

    // --- 3. Test Search Time (Total time to search for all items) ---
    std::cout << "  --- Testing Map Search (total for all " << words.size() << " words) ---" << std::endl;
    long long bst_s_ns = timeMapSearch(bstMap, words);
    long long hash_s_ns = timeMapSearch(hashMap, words);
    long long avl_s_ns = timeMapSearch(avlMap, words);
    long long rb_s_ns = timeMapSearch(rbMap, words);

    std::cout << "    BSTMap  Search: " << (double)bst_s_ns / 1'000'000.0 << " ms" << std::endl;
    std::cout << "    HashMap Search: " << (double)hash_s_ns / 1'000'000.0 << " ms" << std::endl;
    std::cout << "    AVLMap  Search: " << (double)avl_s_ns / 1'000'000.0 << " ms" << std::endl;
    std::cout << "    RBMap   Search: " << (double)rb_s_ns / 1'000'000.0 << " ms" << std::endl;
}

/**
 * Main test-runner function.
 */
int main() {
    // Set cout to print doubles with fixed notation and 3 decimal places
    std::cout << std::fixed << std::setprecision(3);

    std::cout << "========================================" << std::endl;
    std::cout << "  Running Performance Analysis" << std::endl;
    std::cout << "========================================" << std::endl;

    // 1. Load the base data
    // We use "long_sorted.txt" as it's a good-sized list.
    std::vector<std::string> sortedWords = loadWords("documents/long_sorted.txt");
    if (sortedWords.empty()) {
        std::cerr << "Test file is empty or not found. Aborting." << std::endl;
        return 1;
    }

    // 2. Create a randomized version of the data
    std::vector<std::string> randomWords = sortedWords;
    std::shuffle(randomWords.begin(), randomWords.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));

    // 3. Run the tests

    // Test Sorting
    // (We test sorting on the *random* list, as sorting a sorted list is not a good test)
    testSorting(randomWords);

    // Test Searching
    testSearching(randomWords, sortedWords);

    // Test Maps on SORTED data (this should expose the BST's weakness)
    std::cout << "\n--- Testing Maps with SORTED Input ---" << std::endl;
    runMapTests(sortedWords);

    // Test Maps on RANDOM data (this is the average case)
    std::cout << "\n--- Testing Maps with RANDOM Input ---" << std::endl;
    runMapTests(randomWords);

    std::cout << "\n========================================" << std::endl;
    std::cout << "  Performance Analysis Complete" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}