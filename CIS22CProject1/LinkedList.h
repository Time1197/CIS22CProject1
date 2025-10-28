#pragma once

#include <iostream>
#include <stdexcept>

template <typename T>
class LinkedList {
private:
	struct Node {
		T data;
		Node* next;

		//node constructor
		Node(const T& value) {
			data = value;
			next = nullptr;
		}
	};

	Node* head; //pointer to the first node
	int count; //number of items in the list

public:
	//linked list constructor
	LinkedList() {
		head = nullptr;
		count = 0;
	}

	//destructor
	~LinkedList() {
		clear();
	}

	//Copy Constructor
	LinkedList(const LinkedList<T>& other) {
		*this = other;
	}

	//Copy assignment operator
	LinkedList<T>& operator=(const LinkedList<T>& other) {
		//1. checking for self assignment
		if (this == &other) {
			return *this;
		}

		//2. Clear this list current contents
		clear();

		//3. Copy elements from other list
		if (other.head == nullptr) {
			return *this; //other list is empty
		}

		//Copying first node
		head = new Node(other.head->data);
		Node* thisCurrent = head;
		Node* otherCurrent = other.head->next;
		count = 1;

		//Copy the rest of the nodes
		while (otherCurrent != nullptr) {
			thisCurrent->next = new Node(otherCurrent->data);
			thisCurrent = thisCurrent->next;
			otherCurrent = otherCurrent->next;
			count++;
		}
		
		return *this;

	}

	//Operations

	//insert at the tail of the list
	void insert(const T& data) {
		Node* newNode = new Node(data);
		if (head == nullptr) {
			// List is empty
			head = newNode;
		}
		else {
			// Traverse to the end
			Node* current = head;
			while (current->next != nullptr) {
				current = current->next;
			}
			// current is now the last node
			current->next = newNode;
		}
		count++;
	}

	//deletes the first occurance of data
	//returns true if deleted, false if not found
	bool deleteNode(const T& data) {
		if (head == nullptr) {
			return false; // List is empty
		}

		// Case 1: Delete the head
		if (head->data == data) {
			Node* temp = head;
			head = head->next;
			delete temp;
			count--;
			return true;
		}

		// Case 2: Delete from middle or tail
		Node* current = head;
		while (current->next != nullptr && current->next->data != data) {
			current = current->next;
		}

		// If we found the node (current->next is the one to delete)
		if (current->next != nullptr) {
			Node* temp = current->next;
			current->next = current->next->next;
			delete temp;
			count--;
			return true;
		}

		return false; // Not found
	}

	// Finds the first node with 'data'
	// Returns true if found, false otherwise
	bool find(const T& data) const {
		Node* current = head;
		while (current != nullptr) {
			if (current->data == data) {
				return true;
			}
			current = current->next;
		}
		return false;
	}

	// Returns the number of items
	int size() const {
		return count;
	}

	// Checks if the list is empty
	bool isEmpty() const {
		return head == nullptr;
	}

	// Helper to completely clear the list (used by destructor and assignment)
	void clear() {
		Node* current = head;
		while (current != nullptr) {
			Node* temp = current;
			current = current->next;
			delete temp;
		}
		head = nullptr;
		count = 0;
	}

	//Functions for future implementation in Queue
	// Removes and returns the data from the head
	T removeFromHead() {
		if (isEmpty()) {
			throw std::out_of_range("Cannot remove from empty list");
		}

		Node* temp = head;
		T data = head->data;
		head = head->next;

		delete temp;
		count--;
		return data;
	}

	// Gets data from the head without removing it
	T& getFromHead() const {
		if (isEmpty()) {
			throw std::out_of_range("Cannot get from empty list");
		}
		return head->data;
	}

	// --- Debugging ---

	// Prints the list contents (for testing)
	void print() const {
		Node* current = head;
		std::cout << "List (" << size() << " items): [";
		while (current != nullptr) {
			std::cout << current->data;
			if (current->next != nullptr) {
				std::cout << ", ";
			}
			current = current->next;
		}
		std::cout << "]" << std::endl;
	}
	//testing
};