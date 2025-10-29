#pragma once

#include <vector>

namespace Searching {
	/*
	* Preforms a linear search on a vector
	* this algorithm should have a time complexity of O(n)
	* in the worse case, it must check every element
	*/

	template <typename T>
	bool linearSearch(const std::vector<T>& vec, const T& itemToFind) {
		//range based for loop to iterate thru element
		for (const T& element : vec) {
			if (element == itemToFind) {
				return true; //found the element
			}
		}
		return false; //did not find it
	}

	/*
	* Performs a binary search on a vector
	* !! IMPORTANT: The vector must be sorted!
	* Time complexity should be O(log n)
	*/
	template <typename T>
	bool binarySearch (const std::vector<T>& vec, const T& itemToFind) {
		int left = 0;
		int right = static_cast<int>(vec.size()) - 1;

		while (left <= right) {
			int mid = left + (right - left) / 2;

			if (vec[mid] == itemToFind) {
				return true; //found it
			}

			// if item is greater, ignore the left half
			if (vec[mid] < itemToFind) {
				left = mid + 1;
			}
			//if it is smaller, ignore the right half
			else {
				right = mid - 1;
			}

			//if loop finishes and item was not found
		}
		return false;
	}
}