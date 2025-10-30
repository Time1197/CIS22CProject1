#pragma once

#include <stdexcept>
#include <iostream>

template <typename K, typename V>
class BSTMap {
private:
	//private node for tree
	struct Node {
		K key;
		V value;
		Node* left;
		Node* right;

		Node(const K& k, const V& v) {
			key = k;
			value = v;
			left = nullptr;
			right = nullptr;
		}
	};

	Node* root;
	int count; //numnber of key value pairs in map

	//private recursive helpers

	//recusively finds the correct spot and inserts nod
	//returns the new root of (sub)tree
	Node* insertRecursive(Node* node, const K& key, const V& value) {
		//base case
		if (node == nullptr) {
			count++;
			return new Node(key, value);
		}

		//recursive steps
		if (key < node->key) {
			//if key is larger, go lefts
			node->left = insertRecursive(node->left, key, value);
		}
		else if (key > node->key) {
			//if key is larger, go right
			node->right = insertRecursive(node->right, key, value);
		}
		else {
			//if the node alreadyt exists, update value
			node->value = value;
		}

		return node;
	}

	//recursively searches for a key
	/*
	* parameters:
	* node, the current node being checked
	* key, the key to search for
	* outValue, A reference to store the found value
	* returns true if a key was found, false otherwise
	*/
	bool searchRecursive(Node* node, const K& key, V& outValue) const {
		if (node == nullptr) {
			return false; //base case: key not found
		}

		if (key == node->key) {
			outValue = node->value; //value is found
			return true;
		}

		if (key < node->key) {
			return searchRecursive(node->left, key, outValue); //search left
		}
		else {
			return searchRecursive(node->right, key, outValue); //search right
		}
	}

	//recursively deletes all nodes in a post order traversal
	//delete children/subtree, then itself
	void clear(Node* node) {
		if (node != nullptr) {
			clear(node->left); //delete left subtree
			clear(node->right); //delete right subtree
			delete node; //delete its own node node
		}
	}

	//recursively copies the tree in pre-order traversal
	//copy self, then copy children
	//returns a pointer to the new copied node
	Node* copyRecursive(Node* node) {
		if (node == nullptr) {
			return nullptr;
		}

		//create a new node with the same data
		Node* newNode = new Node(node->key, node->value);
		//copy it's children
		newNode->left = copyRecursive(node->left);
		newNode->right = copyRecursive(node->right);

		return newNode;
	}

	//recursively prints the tree in order
	void printInOrder(Node* node) const {
		if (node == nullptr) return;
		printInOrder(node->left);
		std::cout << " (" << node->key << ": " << node->value << ")\n";
		printInOrder(node->right);
	}

public:
	//constructor
	BSTMap() {
		root = nullptr;
		count = 0;
	}

	//destructor
	~BSTMap() {
		clear(root);
	}

	//copy constructor
	BSTMap(const BSTMap<K, V>& other) {
		root = nullptr;
		count = other.count;
		root = copyRecursive(other.root);
	}

	//copy assigment operator
	BSTMap<K, V>& operator=(const BSTMap<K, V>& other) {
		//check for self assignment
		if (this == &other) {
			return *this;
		}
		//clear existing memory
		clear(root);
		//copy new data
		root = copyRecursive(other.root);
		count = other.count;
		
		return *this;
	}

	//public interface
	void insert(const K& key, const V& value) {
		root = insertRecursive(root, key, value);
	}

	/*
	* searches for a key and returns is value via out-parameter
	* parameters:
	* key, the key to search for
	* outValue A variable to hold the result if found
	* return true if the key was found and if outValue is set, false otherwise
	*/
	bool search(const K& key, V& outValue) const {
		return searchRecursive(root, key, outValue);
	}

	int size() const {
		return count;
	}

	bool isEmpty() const {
		return root == nullptr;
	}

	//debugging
	//prints the contents of tree in-order (sorted by key)
	void print() const {
		std::cout << "BSTMap (" << size() << " items):\n";
		if (isEmpty()) {
			std::cout << " (empty)\n";
		}
		printInOrder(root);
	}
};