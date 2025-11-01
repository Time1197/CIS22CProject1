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
 * Call elapsed_ms() to get the time in milliseconds.
 */
struct Timer {
    std::chrono::high_resolution_clock::time_point start_time;

    Timer() : start_time(std::chrono::high_resolution_clock::now()) {}

    // Returns elapsed time in milliseconds as a double
    double elapsed_ms() {
        auto end_time = std::chrono::high_resolution_clock::now();
        // Get duration in nanoseconds, then convert to milliseconds
        long long ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
        return static_cast<double>(ns) / 1'000'000.0;
    }
};

/**
 * Loads all words from a given text file into a vector.
 */
std::vector<std::string> loadWordsFromFile(const std::string& filepath) {
    std::vector<std::string> words;
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "!!! FATAL ERROR: Could not open test file: " << filepath << std::endl;
        std::cerr << "!!! Make sure you are running this from the correct directory." << std::endl;
        return words; // Return empty vector
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
    std::cout << "  Quick Sort Time:   " << t_qs.elapsed_ms() << " ms" << std::endl;

    // Test Merge Sort
    Timer t_ms;
    Sorting::mergeSort(vec_ms);
    std::cout << "  Merge Sort Time:   " << t_ms.elapsed_ms() << " ms" << std::endl;
}

/**
 * Test Task: Searching Algorithms
 * Compares Linear Search vs. Binary Search.
 */
void testSearching(const std::vector<std::string>& unsortedWords, const std::vector<std::string>& sortedWords) {
    std::cout << "\n--- Testing Average Search Time (" << sortedWords.size() << " items) ---" << std::endl;

    // Create a list of 1,000 terms to search for
    const int NUM_SEARCHES = 1000;
    std::vector<std::string> searchTerms;
    searchTerms.reserve(NUM_SEARCHES);
    std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> dist(0, sortedWords.size() - 1);

    for (int i = 0; i < NUM_SEARCHES; ++i) {
        // 90% chance of searching for a word that exists
        if (i % 10 != 0) {
            searchTerms.push_back(sortedWords[dist(gen)]);
        }
        else {
            // 10% chance of searching for a word that does not exist
            searchTerms.push_back("!!-NOT-IN-THE-LIST-" + std::to_string(i));
        }
    }

    // Test Linear Search (on unsorted data)
    Timer t_linear;
    for (const auto& term : searchTerms) {
        Searching::linearSearch(unsortedWords, term);
    }
    double total_linear_ms = t_linear.elapsed_ms();
    std::cout << "  Linear Search (avg): " << (total_linear_ms / NUM_SEARCHES) << " ms" << std::endl;
    std::cout << "    (Total for " << NUM_SEARCHES << " searches: " << total_linear_ms << " ms)" << std::endl;


    // Test Binary Search (on *sorted* data)
    Timer t_binary;
    for (const auto& term : searchTerms) {
        Searching::binarySearch(sortedWords, term);
    }
    double total_binary_ms = t_binary.elapsed_ms();
    std::cout << "  Binary Search (avg): " << (total_binary_ms / NUM_SEARCHES) << " ms" << std::endl;
    std::cout << "    (Total for " << NUM_SEARCHES << " searches: " << total_binary_ms << " ms)" << std::endl;
}

/**
 * Generic helper to time the insertion of all words into any map type.
 * Returns time in milliseconds.
 */
template <typename MapType>
double timeMapInsertion(const std::vector<std::string>& words) {
    MapType map;
    Timer t;
    for (size_t i = 0; i < words.size(); ++i) {
        map.insert(words[i], static_cast<int>(i)); // Key=word, Value=index
    }
    return t.elapsed_ms();
}

/**
 * Generic helper to time searching for all words in a pre-built map.
 * Returns total time in milliseconds.
 */
template <typename MapType>
double timeMapSearch(const MapType& map, const std::vector<std::string>& wordsToFind) {
    int dummyValue; // To hold the search result
    Timer t;
    for (const auto& word : wordsToFind) {
        map.search(word, dummyValue);
    }
    return t.elapsed_ms();
}

/**
 * Test Task: Map Insertion and Search
 * Runs all map comparisons for a given word list.
 */
void runMapTests(const std::vector<std::string>& words, const std::vector<std::string>& searchTerms) {

    // --- 1. Test Insertion Time ---
    std::cout << "  --- Testing Map Insertion (" << words.size() << " words) ---" << std::endl;
    double bst_i_ms = timeMapInsertion<BSTMap<std::string, int>>(words);
    double hash_i_ms = timeMapInsertion<HashMap<std::string, int>>(words);
    double avl_i_ms = timeMapInsertion<AVLMap<std::string, int>>(words);
    double rb_i_ms = timeMapInsertion<RBMap<std::string, int>>(words);

    std::cout << "    BSTMap  Insertion: " << bst_i_ms << " ms" << std::endl;
    std::cout << "    HashMap Insertion: " << hash_i_ms << " ms" << std::endl;
    std::cout << "    AVLMap  Insertion: " << avl_i_ms << " ms" << std::endl;
    std::cout << "    RBMap   Insertion: " << rb_i_ms << " ms" << std::endl;

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

    // --- 3. Test Average Search Time (using the 1000 random search terms) ---
    std::cout << "  --- Testing Map Average Search (for " << searchTerms.size() << " random terms) ---" << std::endl;
    double bst_s_ms = timeMapSearch(bstMap, searchTerms);
    double hash_s_ms = timeMapSearch(hashMap, searchTerms);
    double avl_s_ms = timeMapSearch(avlMap, searchTerms);
    double rb_s_ms = timeMapSearch(rbMap, searchTerms);

    std::cout << "    BSTMap  Avg Search: " << (bst_s_ms / searchTerms.size()) << " ms" << std::endl;
    std::cout << "    HashMap Avg Search: " << (hash_s_ms / searchTerms.size()) << " ms" << std::endl;
    std::cout << "    AVLMap  Avg Search: " << (avl_s_ms / searchTerms.size()) << " ms" << std::endl;
    std::cout << "    RBMap   Avg Search: " << (rb_s_ms / searchTerms.size()) << " ms" << std::endl;
}

/**
 * Main test-runner function.
 */
int main() {
    // Set cout to print doubles with fixed notation and 6 decimal places
    std::cout << std::fixed << std::setprecision(6);

    std::cout << "========================================" << std::endl;
    std::cout << "  Running Performance Analysis" << std::endl;
    std::cout << "========================================" << std::endl;

    // --- 1. Set up test data ---
    // Load the "sorted" list from the file
    std::vector<std::string> sortedWords = loadWordsFromFile("documents/long_sorted.txt");
    if (sortedWords.empty()) {
        std::cerr << "!!! Test run failed. Could not load data." << std::endl;
        return 1;
    }
    std::cout << "Loaded " << sortedWords.size() << " test words from documents/long_sorted.txt" << std::endl;


    // 2. Create a randomized version
    std::vector<std::string> randomWords = sortedWords;
    std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());
    std::shuffle(randomWords.begin(), randomWords.end(), gen);

    // 3. Create a list of 1,000 random search terms
    const int NUM_SEARCHES = 1000;
    std::vector<std::string> searchTerms;
    searchTerms.reserve(NUM_SEARCHES);
    std::uniform_int_distribution<int> dist(0, sortedWords.size() - 1);
    for (int i = 0; i < NUM_SEARCHES; ++i) {
        searchTerms.push_back(sortedWords[dist(gen)]);
    }

    // --- 5. Run the tests ---

    // Test Sorting
    // (We test sorting on the *random* list, as sorting a sorted list is not a good test)
    testSorting(randomWords);

    // Test Searching
    // We pass both the random and sorted lists to the search test
    testSearching(randomWords, sortedWords);

    // Test Maps on SORTED data (this should expose the BST's weakness)
    std::cout << "\n--- Testing Maps with SORTED Input ---" << std::endl;
    runMapTests(sortedWords, searchTerms);

    // Test Maps on RANDOM data (this is the average case)
    std::cout << "\n--- Testing Maps with RANDOM Input ---" << std::endl;
    runMapTests(randomWords, searchTerms);

    std::cout << "\n========================================" << std::endl;
    std::cout << "  Performance Analysis Complete" << std::endl;
    std::cout << "========================================" << std::endl;

    return 0;
}

