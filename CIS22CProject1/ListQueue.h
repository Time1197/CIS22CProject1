#pragma once

#include "LinkedList.h"
#include <stdexcept>

template <typename T>
class ListQueue {
private:
	//the backing data strucutre will be LinkedList
	LinkedList<T> list;

public:
	//Constructor
	//default is fine because it will still be based upon a linked list
	ListQueue(){}

	//since linkedlist handles its own memory, no destcutor or copy constructors are needed

	//Queue Operations

	//adds an item to the back of the queue
	void enqueue(const T& data) {
		//insert() adds to the tail
		list.insert(data);
	}

	T dequeue() {
		if (isEmpty()) {
			throw std::out_of_range("Cannot dequeue from empty queue");
		}
		return list.removeFromHead(); //defined in linkedlist
	}

	//get front without removing from queue
	T& front() {
		if (isEmpty()) {
			throw std::out_of_range("Cannot get front from empty queue");
		}
		return list.getFromHead();
	}

	//constant version for  constnat queues
	const T& front() const {
		if (isEmpty()) {
			throw std::out_of_range("Cannot get front from empty queue");
		}
		return list.getFromHead();
	}

	//checks if the queue is empty
	bool isEmpty() const {
		return list.isEmpty();
	}

	//returns the number of items in the queue
	int size() const {
		return list.size();
	}
};