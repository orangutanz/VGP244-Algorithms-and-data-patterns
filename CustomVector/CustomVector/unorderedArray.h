#pragma once

#include <iostream>
#include "cArray.h"

namespace VGP244 {

	template<typename T, size_t Size=10>
	class uoArray : public cArray<T, Size>
	{
	private:

		//overriding output operator
		friend std::ostream& operator<<(std::ostream& os, uoArray<T, Size>& a)
		{
			for (size_t i = 0; i < a.size(); ++i)
			{
				std::cout << a.at(i) <<", ";
			}
			std::cout << "\n";
			return os;
		}

	public:
		uoArray(size_t len = Size, T val = T()) : cArray<T, Size>(len, val)
		{
			std::cout << "uoArray(): \n";
		}

		uoArray<T, Size>& operator=(const uoArray<T, Size>& src) // assignment operator
		{
			cArray<T, Size>::operator=(src);
			return *this;
		}

		void push_back(T val)  // inserts val at the end of the array
		{
			cArray<T, Size>::resize();
			if (cArray<T, Size>::mSize < cArray<T, Size>::mCap)
			{
				cArray<T, Size>::mData.get()[cArray<T, Size>::mSize++] = val;
			}
			//std::cout << "push_back(" << val << ")\n";
		}

		T& operator[](size_t idx)
		{
			if (idx > cArray<T, Size>::mSize)
			{
				std::cout << "ERROR:uoArray[idx] access out of bounds.\n";
				return cArray<T, Size>::mData.get()[0];
			}
			return cArray<T, Size>::mData.get()[idx];
		}

		// inserts num copy of val at index location "pos". Make sure to check there is enough room in uoArray for the insert
		void insert(T val, size_t pos, size_t num = 1)
		{
			//if pos is beyond mSize put it at the back of the array
			if (pos > cArray<T, Size>::mSize)
			{
				pos = cArray<T, Size>::mSize;
			}

			cArray<T, Size>::mSize += num;
			//if insert times are beyond the cap * 2, do a custom resize
			if (cArray<T, Size>::mSize > cArray<T, Size>::mCap * 2)
			{
				cArray<T, Size>::mCap = (cArray<T, Size>::mCap * 2) + (cArray<T, Size>::mSize + num);//make sure the size is enough
				std::shared_ptr<T> newData = std::shared_ptr<T>(new T[cArray<T, Size>::mCap], [](T* p)
				{
					delete[] p;
				});
				for (size_t i = 0; i < cArray<T, Size>::mCap; ++i)
					newData.get()[i] = cArray<T, Size>::mData.get()[i];
				cArray<T, Size>::mData.swap(newData);
			}
			//normal resize
			else
			{
				cArray<T, Size>::resize();
			}

			//insert at pos for num of times
			for (size_t i = 0; i < num; ++i)
			{
				T tempCurrent = cArray<T, Size>::mData.get()[pos + i];
				T tempNext = tempCurrent;
				cArray<T, Size>::mData.get()[pos + i] = val;

				//move later elements back
				for (size_t j = pos + i + 1; j < cArray<T, Size>::mSize; ++j)
				{
					tempNext = cArray<T, Size>::mData.get()[j];
					cArray<T, Size>::mData.get()[j] = tempCurrent;
					tempCurrent = tempNext;
				}

			}			
			//std::cout << "insert()\n";
		}


		/*
		* TODO: implement eraseAll based on the description.
		*/
		// erases all occurrances of 'val' , return number of instances of 'val' erased;
		int eraseAll(T val)
		{
			if (cArray<T, Size>::mSize == 1 && cArray<T, Size>::mData.get()[0] == val)
			{
				cArray<T, Size>::mSize = 0;
				return 1;
			}

			int num = 0;
			for (size_t i = 0; i < cArray<T, Size>::mSize; ++i)
			{
				if (cArray<T, Size>::mData.get()[i] == val)
				{
					size_t j = i;
					while (j < cArray<T, Size>::mSize - 1)
					{
						cArray<T, Size>::mData.get()[j] = cArray<T, Size>::mData.get()[j + 1];
						j++;
					}
					cArray<T, Size>::mSize--;
					num++;
				}
			}

			return num;
		}

		// search for first occurrance of 'val' in the array. If found, return its index. If not found return -1;
		int search(T val) const
		{
			for (size_t i = 0; i < cArray<T, Size>::mSize; ++i)
			{
				if (cArray<T, Size>::mData.get()[i] == val)
				{
					return i;
				}
			}
			return -1;
		}

		// use one of your favorite sorting algorithms to sort the data in the array using the comparison operator OP.
		template<typename COM>
		void sort()
		{
			if (cArray<T, Size>::mSize < 1)
				return;
			COM op; // use op comparision operator for comparing items for sorting. op(item1, item2) returns false or true
			for (size_t i = 0; i < cArray<T, Size>::mSize - 1; ++i)
			{		
				//bubble sort (data[i],data[j])
				int j = i + 1;
				while ( j > 0 && (op(cArray<T, Size>::mData.get()[j], cArray<T, Size>::mData.get()[j - 1])) )
				{
					T temp = cArray<T, Size>::mData.get()[j];

					cArray<T, Size>::mData.get()[j] = cArray<T, Size>::mData.get()[j - 1];
					cArray<T, Size>::mData.get()[j - 1] = temp;
					j--;
					if (!(op(cArray<T, Size>::mData.get()[j], cArray<T, Size>::mData.get()[j - 1]))) // no longer needs to compare
					{
						j = -1; // skip sorted elements
					}
				} 
			}
		}
	};

}
