#pragma once

#include <vector>

namespace Sorting {
	//private merge function
	//subarray 1 is vec[left...mid]
	//subarray 2 is vec[mid+1...right]

	//first up is creating the mergeSort
	//this will serve as the merge sort helper method
	//this method will be private
	
	//MERGE SORT CODE
	template <typename T>
	void merge(std::vector<T>& vec, int left, int mid, int right) {
		//1. find sizes of two subarrays to be merged
		int n1 = mid - left + 1;
		int n2 = right - mid;

		//2. Create temporary vectors
		std::vector<T> leftVec(n1);
		std::vector<T> rightVec(n2);

		//3. copy data to temporary vectors
		for (int i = 0; i < n1; ++i) {
			leftVec[i] = vec[left + i];
		}
		for (int j = 0; j < n2; ++j) {
			rightVec[j] = vec[mid + 1 + j];
		}

		//4. merge the temporary vectors back into vec[left...right]

		//inital indicies of first and second subarrays
		int i = 0;
		int j = 0;

		//initial index of merged subarray
		int k = left;

		while (i < n1 && j < n2) {
			if (leftVec[i] <= rightVec[j]) {
				vec[k] = leftVec[i];
				i++;
			}
			else {
				vec[k] = rightVec[j];
				j++;
			}
			k++;
		}

		//5. copying remaining elements of left[] if there are any
		while (i < n1) {
			vec[k] = leftVec[i];
			i++;
			k++;
		}

		//6. do the same for the right[]
		while (j < n2) {
			vec[k] = rightVec[j];
			j++;
			k++;
		}
	}

	//private recursive merge sort function
	template <typename T>
	void mergeSortRecursive(std::vector<T>& vec, int left, int right) {
		if (left >= right) {
			return; //base case: subarray has 0 or 1 elements, it's already sorted
		}

		//find the middle point
		//using (left + right) / 2 can overflow for large numbers
		int mid = left + (right - left) / 2;

		//sort first and second halves
		mergeSortRecursive(vec, left, mid);
		mergeSortRecursive(vec, mid + 1, right);

		//merge the sorted halves
		merge(vec, left, mid, right);
	}

	//public merge sort function, sorts only vectors (because they have to be vectors logically)
	template <typename T>
	void mergeSort(std::vector<T>& vec) {
		// Only sort if there's more than 1 element
		if (vec.size() > 1) {
			mergeSortRecursive(vec, 0, static_cast<int>(vec.size()) - 1);
		}
	}

	// QUICK SORT CODE

	//private partition codes
	template <typename T>
	int partition(std::vector<T>& vec, int low, int high) {
		T pivot = vec[high];
		int i = (low - 1);

		for (int j = low; j <= high - 1; j++) {
			//if currrent element is smaller or equal to pivot
			if (vec[j] <= pivot) {
				i++;
				std::swap(vec[i], vec[j]);
			}
		}
		//placing pivot in the correct spot
		std::swap(vec[i + 1], vec[high]);
		return (i + 1);
	}

	//private recursive quick sort function
	template <typename T>
	void quickSortRecursive(std::vector<T>& vec, int low, int high) {
		if (low < high) {
			//partitioned index
			int pie = partition(vec, low, high);

			//seperately sort elements before partition and after partition
			quickSortRecursive(vec, low, pie - 1);
			quickSortRecursive(vec, pie + 1, high);
		}
	}

	//public quick sort function
	template <typename T>
	void quickSort(std::vector<T>& vec) {
		//only sort if there is more than one element
		if (vec.size() > 1) {
			quickSortRecursive(vec, 0, static_cast<int>(vec.size()) - 1);
		}
	}
}