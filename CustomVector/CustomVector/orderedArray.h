#pragma once
#include <iostream>
#include <assert.h>
#include "cArray.h"

namespace VGP244 {
	template<typename T, typename COM, size_t Size=10>
	class oArray : public cArray<T, Size>
	{
	private:
		COM op;

		friend std::ostream& operator<<(std::ostream& os, oArray<T, COM, Size>& a)
		{
			for (size_t i = 0; i < a.size(); ++i)
			{
				std::cout << a.at(i) << ", ";
			}
			std::cout << "\n";
			return os;
		}

		/*
		* entry function to find Up most data index such that
		* op(mData[up], val) is still true.
		*/
		size_t findUp(T val) const
		{
			auto count{ cArray< T, Size >::size() };
			if (count == 0)
			{
				return 0;
			}
			assert(count > 0);
			return findUpIndex(val, 0, count - 1);
		}

		/*
		* finds val in the range [s, e] (s is start index and e
		* is end index of the range. The range includes both s and e)
		* This method uses "divide-and-conquer"
		* strategy to find the highest index pos such that op(mData[pos],val) == true.
		* Uses recursive method:
		* find mid point in the range, u, and figure out which half could contain val.
		*/
		size_t findUpIndex(T val, size_t s, size_t e) const
		{
			assert(e >= s);
			if (s == e)
				return e;

			auto u{ (s + e) / 2 };  // midpoint in range [s, e]
			if (op(cArray<T, Size>::mData.get()[u], val) == true)
				return findUpIndex(val, u + 1, e);
			else // means op(mData[u], val) == false
				return findUpIndex(val, s, u);
		}

	public:

		oArray(size_t len = Size, T val = T()) : cArray<T, Size>(len, val)
		{
			std::cout << "oArray(): \n";
		}

		/*
		* TODO: implement assignment operator
		*/
		oArray<T, COM, Size>& operator=(const oArray<T, COM, Size>& src)  // assignment operator
		{
			op = src.op;
			cArray<T, Size>::operator=(src);
			return *this;
		}

		/*
		* TODO:
		* insert num of copies of val in the array.
		* This method does the following steps:
		* i) call findUp() to find the highest index "up" in the array
		* whose data is still "less" than val. Here "less" means op(mData[up], val) == true
		* ii) move all items mData from up+1 forward to right by num.
		* iii) copy num copies of val from up+1 to up+num in mData.
		*/
		void insert(T val, size_t num = 1)
		{

			size_t up{ findUp(val) };

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

			//insert at up for num of times
			for (size_t i = 0; i < num; ++i)
			{
				T tempCurrent = cArray<T, Size>::mData.get()[up + i];
				T tempNext = tempCurrent;
				cArray<T, Size>::mData.get()[up + i] = val;

				//move later elements back
				for (size_t j = up + i + 1; j < cArray<T, Size>::mSize; ++j)
				{
					tempNext = cArray<T, Size>::mData.get()[j];
					cArray<T, Size>::mData.get()[j] = tempCurrent;
					tempCurrent = tempNext;
				}

			}
		}

		/*
		* TODO: implement eraseAll based on the ordered array description.
		*/
		// erases all occurrances of 'val' , return number of instances of 'val' erased;
		int eraseAll(T val)
		{
			if (cArray<T, Size>::mSize == 1 && cArray<T, Size>::mData.get()[0] == val)
			{
				cArray<T, Size>::mSize = 0;
				return 1;
			}
			size_t up = findUp(val);

			int num = 0;

			while (cArray<T, Size>::mData.get()[up] == val)
			{
				size_t j = up;
				while (j < cArray<T, Size>::mSize - 1)
				{
					cArray<T, Size>::mData.get()[j] = cArray<T, Size>::mData.get()[j + 1];
					++j;
				}
				cArray<T, Size>::mSize--;
				++up;
				++num;
			}
			return num;
		}


		/*
		* search for first occurrance of 'val' in the array. If found, return its index. If not found return -1;
		* TODO: implement using the 'Divide-and-Conquer' strategy.
		* hint: you can use findupindex() here similar to how it is used above, except
		* here you are looking to see if val is in the array.
		*/
		int search(T val) const
		{
			if (cArray<T, Size>::mSize == 0)
			{
				return 0;
			}
			else if (cArray<T, Size>::mSize == 1 && cArray<T, Size>::mData.get()[0] == val)
			{
				return 1;
			}
			size_t up = findUpIndex(val, 0, cArray<T, Size>::mSize == 0);
			if (cArray<T, Size>::mData.get()[up] == val)
			{
				return up;
			}
			return -1;
		}		

		/*
		* Finds for all values in the range [from, to] in the array and print them out.
		*	TODO: use findupindex for both 'from' and 'to' to find  'lower' and 'upper' indices
		* for the print. Then print all elements starting from lower+1 all the way to upper.
		*/
		void rangePrint(T from, T to)
		{
			if (cArray<T, Size>::size() == 0)
				return;

			size_t lower = findUpIndex(from, 0, (cArray<T, Size>::size()));
			size_t upper = findUpIndex(to, 0, (cArray<T, Size>::size()));
			std::cout << "index from " << lower + 1 << " to " << upper << " :";

			for (size_t i = lower; i < upper; ++i)
			{
				std::cout << cArray<T, Size>::at(i) << ", ";
			}
			std::cout << "\n";
		}
	};
}