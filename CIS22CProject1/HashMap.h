#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>
#include <functional> // For std::hash

template <typename K, typename V>
class HashMap {
private:
	//private node for chains (linked lists) in each bucket
	struct Node {
		K key;
		V value;
		Node* next;

		Node(const K& k, const V& v) {
			key = k;
			value = v;
			next = nullptr;
		}
	};

	std::vector<Node*> buckets; //array of buckets for chaining
	int count;
	int capacity;
	
	//max load factor to trigger rehash
	static constexpr double MAX_LOAD_FACTOR = 0.75;

	/*
	* private helper to hash a key to a bucket index
	* uses std::hash
	*/
	int hash(const K& key) const {
		// Use std::hash to get a hash code, then modulo by capacity
		return static_cast<int>(std::hash<K>{}(key) % capacity);
	}

	/* private helper to resize and reinsert all items
	* called when load factor is exceeded
	*/
	void rehash() {
		int oldCapacity = capacity;
		std::vector<Node*> oldBuckets = buckets;

		//double the capacity and create a new, empty bucket array
		capacity *= 2;
		buckets.assign(capacity, nullptr); //resizes and fills with nullptr
		count = 0; //reset count as insert() will increment it

		//go thru all old buckets
		for (int i = 0; i < oldCapacity ++i) {
			Node* current = oldBuckets[i];
			//traverse the linked list in this bucket
			while (current != nullptr) {
				//re-insert the node into a new larger table
				insert(current->key, current->value);

				//clean up old node
				Node* temp = current;
				current = current->next;
				delete temp;
			}
		}
	}

	//private clear helper
	//deletes all nodes in all buckets
	void clearAllNodes() {
		for (int i = 0; i < capacity; ++i) {
			Node* current = buckets[i];
			while (current != nullptr) {
				Node* temp = current;
				current = current->next;
				delete temp;
			}
			buckets[i] = nullptr
		}
		count = 0;
	}

public:
	//constructor
	HashMap(int initialCapacity = 10) {
		//ensure capacity is at least 1
		capacity = (initialCapacity > 0) ? initialCapacity : 10;
		//initialize buckets vector to the given capacity, all with nullptr
		buckets.assign(capacity, nullptr);
		count = 0;
	}

	//desctructor
	~HashMap() {
		clearAllNodes();
	}

	//copy constructor
	HashMap(const HashMap<K, V>& other) {
		//initalize this map's members
		capacity = other.capacity;
		count = other.count;
		buckets.assign(capacity, nullptr);

		//deep copy
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

	// copy assignment operator
	HashMap<K, V> operator=(const HashMap<K, V>& other) {
		if (this == &other) {
			return *this; //handle self assignment
		}

		//clear existing data
		clearAllNodes();
		
		//copy data from other
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

};