#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <functional> // For std::hash

template <typename K, typename V>
class HashMap {
private:
    // 1. Private Node struct for the linked list in each bucket
    struct Node {
        K key;
        V value;
        Node* next;

        Node(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
    };

    // 2. Member variables
    std::vector<Node*> buckets; // The array of bucket-head pointers
    int count;                  // Number of key-value pairs
    int capacity;               // Size of the buckets vector

    // Max load factor to trigger a rehash
    static constexpr double MAX_LOAD_FACTOR = 0.75;

    // 3. Private hash helper function
    /**
     * Hashes a key to a bucket index.
     */
    int hash(const K& key) const {
        // Use std::hash to get a hash code, then modulo by capacity
        // Note: We use std::hash<K>{}(key) to create a temporary hash object and call it
        return static_cast<int>(std::hash<K>{}(key) % capacity);
    }

    // 4. Private rehash helper (required by project)
    /**
     * Resizes the table to double its capacity and re-inserts all items.
     */
    void rehash() {
        int oldCapacity = capacity;
        std::vector<Node*> oldBuckets = buckets;

        // Double the capacity and create a new, empty bucket array
        capacity *= 2;
        buckets.assign(capacity, nullptr); // Resizes and fills with nullptr
        count = 0; // Reset count, as insert() will increment it

        // Go through all old buckets
        for (int i = 0; i < oldCapacity; ++i) { // <--- Semicolon on line before this is crucial
            Node* current = oldBuckets[i];
            // Traverse the linked list in this bucket
            while (current != nullptr) {
                // Re-insert the node into the *new* larger table
                insert(current->key, current->value);

                // --- Clean up the old node ---
                Node* temp = current;
                current = current->next;
                delete temp;
                // --- End clean up ---
            }
        }
    } // <--- Missing semicolon here would cause errors

    // 5. Private clear helper (for destructor and assignment)
    /**
     * Deletes all nodes in all buckets.
     */
    void clearAllNodes() {
        for (int i = 0; i < capacity; ++i) {
            Node* current = buckets[i];
            while (current != nullptr) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
            buckets[i] = nullptr;
        }
        count = 0;
    } // <--- Missing semicolon here would cause errors

public:
    // --- Constructor ---
    HashMap(int initialCapacity = 10) {
        // Ensure capacity is at least 1
        capacity = (initialCapacity > 0) ? initialCapacity : 10;
        // Initialize buckets vector to the given capacity, all filled with nullptr
        buckets.assign(capacity, nullptr);
        count = 0;
    }

    // --- Destructor ---
    ~HashMap() {
        clearAllNodes();
    }

    // --- Copy Constructor ---
    HashMap(const HashMap<K, V>& other) {
        // Initialize this map's members
        capacity = other.capacity;
        count = other.count;
        buckets.assign(capacity, nullptr);

        // Perform a deep copy of all buckets and nodes
        for (int i = 0; i < capacity; ++i) {
            Node* otherCurrent = other.buckets[i];
            Node* thisTail = nullptr; // To build our new list

            while (otherCurrent != nullptr) {
                Node* newNode = new Node(otherCurrent->key, otherCurrent->value);

                if (thisTail == nullptr) {
                    // This is the first node in this bucket
                    buckets[i] = newNode;
                    thisTail = newNode;
                }
                else {
                    // Add to the end of our list
                    thisTail->next = newNode;
                    thisTail = newNode;
                }
                otherCurrent = otherCurrent->next;
            }
        }
    }

    // --- Copy Assignment Operator ---
    HashMap<K, V>& operator=(const HashMap<K, V>& other) {
        if (this == &other) {
            return *this; // Handle self-assignment
        }

        // 1. Clear existing data in this map
        clearAllNodes();

        // 2. Copy data from 'other' (similar to copy constructor)
        capacity = other.capacity;
        count = other.count;
        buckets.assign(capacity, nullptr);

        for (int i = 0; i < capacity; ++i) {
            Node* otherCurrent = other.buckets[i];
            Node* thisTail = nullptr;

            while (otherCurrent != nullptr) {
                Node* newNode = new Node(otherCurrent->key, otherCurrent->value);
                if (thisTail == nullptr) {
                    buckets[i] = newNode;
                    thisTail = newNode;
                }
                else {
                    thisTail->next = newNode;
                    thisTail = newNode;
                }
                otherCurrent = otherCurrent->next;
            }
        }

        return *this;
    }

    // --- Public Interface ---

    /**
     * Inserts a key-value pair. If the key already exists, updates the value.
     */
    void insert(const K& key, const V& value) {
        // 1. Check load factor, rehash if necessary *before* inserting
        // --- THIS IS LIKELY YOUR "Line 169" ---
        if (static_cast<double>(count + 1) / capacity > MAX_LOAD_FACTOR) {
            rehash();
        }

        // 2. Find the bucket
        int index = hash(key);
        Node* current = buckets[index];

        // 3. Search for the key in this bucket's list
        while (current != nullptr) {
            if (current->key == key) {
                // Key found: update value and return
                current->value = value;
                return;
            }
            current = current->next;
        }

        // 4. Key not found: create a new node and add it to the *front*
        //    (This is the simplest way to insert into a singly linked list)
        Node* newNode = new Node(key, value);
        newNode->next = buckets[index];
        buckets[index] = newNode;
        count++;
    }

    /**
     * Searches for a key and returns its value via out-parameter.
     * Returns true if found, false otherwise.
     */
    bool search(const K& key, V& outValue) const {
        int index = hash(key);
        Node* current = buckets[index];

        // Traverse the list at this bucket
        while (current != nullptr) {
            if (current->key == key) {
                // Key found
                outValue = current->value;
                return true;
            }
            current = current->next;
        }

        // Key not found
        return false;
    }

    /**
     * Removes a key-value pair from the map.
     * Returns true if the key was found and removed, false otherwise.
     */
    bool remove(const K& key) {
        int index = hash(key);
        Node* current = buckets[index];
        Node* prev = nullptr; // Keep track of the previous node

        // Traverse the list
        while (current != nullptr) {
            if (current->key == key) {
                // Key found, now remove it
                if (prev == nullptr) {
                    // Case 1: The node to remove is the head of the list
                    buckets[index] = current->next;
                }
                else {
                    // Case 2: The node is in the middle or at the end
                    prev->next = current->next;
                }

                delete current;
                count--;
                return true;
            }
            // Move to the next node
            prev = current;
            current = current->next;
        }

        // Key not found
        return false;
    }

    // --- Getters ---

    int size() const {
        return count;
    }

    bool isEmpty() const {
        return count == 0;
    }

    // --- Debugging ---
    /**
     * Prints the contents of the hash map (for testing).
     */
    void print() const {
        std::cout << "HashMap (" << count << " items, " << capacity << " capacity):\n";
        for (int i = 0; i < capacity; ++i) {
            std::cout << "  Bucket " << i << ": ";
            Node* current = buckets[i];
            if (current == nullptr) {
                std::cout << "(empty)\n";
            }
            else {
                std::cout << "[";
                while (current != nullptr) {
                    std::cout << " (" << current->key << ": " << current->value << ") ";
                    if (current->next != nullptr) {
                        std::cout << "->";
                    }
                    current = current->next;
                }
                std::cout << "]\n";
            }
        }
    }
};