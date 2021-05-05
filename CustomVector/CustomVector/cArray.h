#pragma once
/*
* This is the base class for our custom arrays. All custom arrays should inherit
* from this class
*/
namespace VGP244 {
	template<typename T, size_t Size = 10>
	class cArray
	{
	protected:
		size_t mSize{};
		size_t mCap;
		std::shared_ptr<T> mData;

		void resize()
		{
			if (mSize > 0.8 * mCap)
			{
				mCap *= 2;
				std::shared_ptr<T> newData = std::shared_ptr<T>(new T[mCap], [](T* p)
				{
					delete[] p;
				});
				for (size_t i = 0; i < mCap; ++i)
					newData.get()[i] = mData.get()[i];
				mData.swap(newData);
			}
			else if (mSize < 0.3 * mCap)
			{
				mCap *= 0.5;
				std::shared_ptr<T> newData = std::shared_ptr<T>(new T[mCap], [](T* p)
				{
					delete[] p;
				});
				for (size_t i = 0; i < mCap; ++i)
					newData.get()[i] = mData.get()[i];
				mData.swap(newData);
			}
		}

		friend std::ostream& operator<<(std::ostream& os, cArray<T, Size>& a)
		{
			for (size_t i = 0; i < a.size(); ++i)
			{
				std::cout << a.at(i) << ", ";
			}
			std::cout << "\n";
			return os;
		}

	public:
		typedef T* Iterator;
		cArray(size_t len = Size, T val = T()) : mSize(0), mCap(len)// basic constructor which is also a default constructor
		{
			if (mCap == 0)
				return;
			mData = std::shared_ptr<T>(new T[mCap], [](T* p) {
				delete[] p;
			});
			for (size_t i = 0; i < mCap; ++i)
				mData.get()[i] = val;
		}

		cArray(const cArray<T>& src) : cArray(src) // copy constructor
		{
			if (src == NULL)
				return;

			mCap = src.mCap;
			mSize = src.mSize;
			mData = std::shared_ptr<T>(new T[mCap], [](T* p) {
				delete[] p;
			});
			for (size_t i = 0; i < mCap; ++i)
			{
				mData.get()[i] = src.at(i);
			}
		}

		~cArray()
		{
			std::cout << "\nDeconstruct ~cArray\n";
			mData.reset();
		}

		cArray& operator=(const cArray<T>& src)  // assignment operator
		{
			mCap = src.mCap;
			mSize = src.mSize;
			mData = std::shared_ptr<T>(new T[mCap], [](T* p) {
				delete[] p;
			});
			for (size_t i = 0; i < mCap; ++i)
			{
				mData.get()[i] = src.at(i);
			}
			return *this;
		}

		T at(size_t pos) const
		{
			if (pos > mCap || pos < 0)
			{
				std::cout << "ERROR:cArray.at(pos) access out of bounds.\n";
				return T();
			}
			return *(mData.get() + pos); // we need to make this safe against pos < 0 or pos >= mCap
		}

		T back() const // return value at the end of the array
		{
			return mData.get()[mSize - 1];
		}

		bool erase(T val)
		{
			if (mSize == 1 && mData.get()[0] == val)
			{
				mSize = 0;
				return true;
			}

			for (size_t i = 0; i < mSize; ++i)
			{
				if (mData.get()[i] == val)
				{
					while (i < mSize - 1)
					{
						mData.get()[i] = mData.get()[i + 1];
						i++;
					}
					mSize--;
					return true;
				}
			}
			return false;
		}

		T operator [](size_t idx) const
		{
			if (idx > mCap || idx < 0)
			{
				std::cout << "ERROR:cArray.[idx] access out of bounds.\n";
				return T();
			}
			return mData.get()[idx];
		}

		void pop_back()  // removes the last item from the end of the array
		{
			if (mSize == 0)
			{ 
				std::cout << "ERROR:cArray.pop_back() empty array.\n";
				return;
			}			
			--mSize;
			resize();
		}

		void pop_front()  // removes the first item from  the array
		{
			if (mSize == 0)
			{
				std::cout << "ERROR:cArray.pop_front() empty array.\n";
				return;
			}
			else if (mSize == 1)
				mSize = 0;

			size_t i = 0;
			while (i < mSize - 1)
			{
				mData.get()[i] = mData.get()[i + 1];
				i++;
			}
			mSize--;

		}

		Iterator begin()
		{
			return mData.get();
		}
		Iterator end()
		{
			return mData.get() + mSize;
		}

		const Iterator cbegin()
		{
			return mData.get();
		}
		const Iterator cend()
		{
			return mData.get() + mSize;
		}

		size_t size() const { return mSize; }
		size_t capacity() const { return mCap; }
		bool empty() const { return mSize == 0; }

	};

}
