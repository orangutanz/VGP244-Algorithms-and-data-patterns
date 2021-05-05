#include <iostream>
#include <random>

#include "unorderedArray.h"
#include "orderedArray.h"
#include "cArray.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <ctime>

using namespace VGP244;
using namespace std;



int main()
{

	cout << "====== Week1 Assignment:customArray ======\n";
	{
	uoArray<int> intUOArray(10, -1);

	std::cout << "Initial 10 ints:\n";
	for (int i = 0; i < 10; ++i)
		intUOArray.push_back(rand());	
	std::cout << intUOArray << std::endl;	
	
	std::cout << "Plus Next 10 ints:\n";
	for (int i = 0; i < 10; ++i)
		intUOArray.push_back(rand());
	std::cout << intUOArray << std::endl;
	
	std::cout << "set i = 5 to -5 and print:\n";
	intUOArray[5] = -5;
	std::cout << intUOArray << std::endl;

	cout << "pop_back and print:\n";
	intUOArray.pop_back();
	cout << intUOArray << endl;
	
	cout << "insert at index 7, 3 copies of 33:\n";
	intUOArray.insert(33, 7, 3);
	cout << intUOArray << endl;

	cout << "Search for 8:\n";
	int foundIdx{ intUOArray.search(8) };
	cout << ((foundIdx == -1) ? "8 not found " : "8 found at index ") << foundIdx << endl << endl;
	
	cout << "Sort using std::less :\n";
	intUOArray.sort<std::less<int>>();
		
	for (auto item = intUOArray.cbegin(); item != intUOArray.cend(); ++item)
		cout << *item << ", ";
	}

	cout << "\n======Week2 Assignment:orderedArray======\n";
	{
		cout << "\nTesting ordered array using 10 random integers (%300):\n";
		oArray<int, std::less<int>> intOArray;

		for (int i = 0; i < 10; ++i)
			intOArray.insert(rand() % 300);
		cout << intOArray << endl;

		for (int i = 0; i < 10; ++i)
			intOArray.insert(rand() % 300);
		cout << "Plus Next 10 ints:\n";
		cout << intOArray << std::endl;

		cout << "size=" << intOArray.size() << ", capacity=" << intOArray.capacity() << endl;

		cout << "\npop_back and print:\n";
		intOArray.pop_back();
		cout << intOArray << endl;

		cout << "\npop_front and print:\n";
		intOArray.pop_front();
		cout << intOArray << endl;

		cout << "\nInsert 3 copies of 33:\n";
		intOArray.insert(33, 3);
		cout << intOArray << endl;

		cout << "Search for 8:\n";
		int foundIdx{ intOArray.search(8) };
		cout << ((foundIdx == -1) ? "8 not found " : "8 found at index ") << foundIdx << endl;
				
		cout << "\nRangeprint between 100 and 150: \n";
		intOArray.rangePrint(100, 150);
		cout << intOArray << endl;
	}

	cin.get();
	return 0;
}