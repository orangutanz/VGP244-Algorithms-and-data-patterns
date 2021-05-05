
#include <iostream>
#include <fstream>
#include <ctime>
#include <random>
#include <map>

#include "avl.h"

using namespace myAVLTree;
using namespace std;


int main(int argc, char** argv)
{
	cout << "Part1:\n";
	AVLTree<int, std::less<int>> intLessAVLTree;
	srand(time(0));
	vector<int> numVec;
	intLessAVLTree.clear();
	for (int i = 0; i < 50; ++i)
	{
		numVec.push_back(rand() % 100 - 50);
		//intLessAVLTree.print();
		//cout << "insert "<< numVec.at(i)<<endl << endl;
		intLessAVLTree.insert(numVec.at(i));
	}
	intLessAVLTree.printLevels();
	cout << endl;

	cout << "Search:\n";
	cout << numVec[10]<<" found? " << intLessAVLTree.search_it(numVec[10]) << endl;
	cout << "102 found? " << intLessAVLTree.search_it(102) << endl;

	cout << "test deleting " << numVec[10] << "  and print:" << intLessAVLTree.remove(numVec[10]) << endl;
	intLessAVLTree.printLevels();
	cout << "test deleting " << numVec[0] << "  and print:" << intLessAVLTree.remove(numVec[0]) << endl;
	intLessAVLTree.printLevels();
	cout << "test deleting 101(not in tree)  and print:" << intLessAVLTree.remove(101) << endl;
	intLessAVLTree.printLevels();

	cout << "testing Level order Traversal:\n";
	// implement prints as an observer. Define PrintObserver
	auto PrintObserver{ [](int val) { std::cout << val << ", "; } };
	intLessAVLTree.apply(PrintObserver , LevelOrder);
	intLessAVLTree.clear();
	cout << endl << endl;
	/*
	* Part 2:
For testing you are going to use testData.txt and examineData.txt, both attached to this assignment.
testData.txt provides a very large number of numbers to be used to build an AVL tree. 
1- Once it is built, you should print the height of the avl tree
2- Use  printLevels to print the first 4 levels of the tree nodes.
3- For next step, you are going to remove nodes from the tree in batches of 50 from the list in examineData.txt. After removing
each batch, save the height and the number of nodes in the tree in a height map as declared like 
this:  std::map<int, int> heightMap;
This is going to map from tree size(meaning number of items in the tree) to its height, for example heightMap[300] is the height of the tree with 300 nodes.
4-Once the process is finished, you must print the data in the map in nice column style: Count column, and Height Column.
Bellow is the starting code for loading testData and build the tree.​
	*/

	//size_t inputLimits = 833; // harde coded for speed up testing
	cout << "Part2: building a large data count AVL tree and test its log performance.\n";
	cout << "Enter (number) to start loading (number) amount of data...\n";
	cout << "Recommend (200-4000) for speedy testing.\n";
	size_t inputLimits = 20;
	do
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cin >> inputLimits;
	} while (!std::cin.good());
	//1.built
	cout << "\n1.Build from testData.txt\n";
	AVLTree<int, std::less<int>> testAVLTree;		
	ifstream input{ "testData.txt" };	
	int num{};	
	std::ios_base::sync_with_stdio(true);
	while ((input >> num) && testAVLTree.getCount() < inputLimits)
	{
		testAVLTree.insert(num);
		cout << "\rProcessed " << testAVLTree.getCount() << " of data " << flush;
	}	
	//2.print
	cout << "\n2.Print 4 levels of tree from root :\n";
	testAVLTree.printLevels(4);

	//3.remove and add to map
	cout << "\n3.Remove according to examineData.txt and make a heightMap\n";
	std::map<size_t, size_t> heightMap; // heightMap(Count,Height)
	ifstream removalData{ "examineData.txt" };
	size_t removalCounts = 0;
	while (removalData >> num) // added some early numbers for testing
	{
		if (testAVLTree.remove(num))
		{
			//cout << "\rRemoving " << num << " found. " << flush;
			++removalCounts;
		}
		else
		{
			//cout << "\rRemoving " << num << " not found. " << flush;
		}
		if (removalCounts == 50)
		{
			cout << "Removed 50 data. " ;
			cout << "Current data count: "<< testAVLTree.getCount() << ", height: " <<testAVLTree.getHeight() << endl;
			removalCounts = 0;
			heightMap.emplace(testAVLTree.getCount(), testAVLTree.getHeight());
		}
	}
	//4.print map
	cout << "\n4.Print heightMap[Count, Height]\n";
	if (!heightMap.empty())
	{
		for (auto& tree : heightMap)
		{
			cout << "  [" << tree.first << ':' << tree.second << ']' << endl;
		}
	}
	else
	{
		cout << "heightMap is empty" << endl;
	}

	testAVLTree.clear();
	cout << "End of the program." << endl;
	do
	{
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cin.get();
	} while (!cin.good());
	return 0;

}