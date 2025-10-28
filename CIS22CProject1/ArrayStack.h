#pragma once

#include <vector>
#include <stdexcept>

template <typename T>
class ArrayStack {
private:
	std::vector<T> container;

public:
	//default constructor
	//The vector "container" is automatically constructed
	ArrayStack() {};

	//no destructors/copy/assigments needed because the "container" vector already handles
	//its own memory

	//Stack Operations
	void push(const T& data) {
		container.push_back(data); //add to end of vector.
	}

	//remove top item from stack
	void pop() {
		if (isEmpty()) {
			throw std::out_of_range("Cannot pop from empty stack");
		}
		container.pop_back();
	}

	//gives the top item of stack w/o removing
	T& top() {
		if (isEmpty()) {
			throw std::out_of_range("Cannot get top from empty stack");
		}
		return container.back;
	}

	//same as above, but for constant stacks
	const T& top() const {
		if (isEmpty()) {
			throw std::out_of_range("Cannot get top from empty stack");
		}
		return container.back();
	}

	//check if empty or not
	bool isEmpty() const {
		return container.empty();
	}

	//number of items in stack
	int size() const {
		return container.size();
	}
};