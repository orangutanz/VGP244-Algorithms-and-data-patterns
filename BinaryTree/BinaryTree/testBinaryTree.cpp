#include <iostream>

#include "binaryTree.h"


using namespace myBinaryTree;
using namespace std;


int main(int argc, char** argv)
{
	BinTree<int, std::less<int>> intLessBTree;

	intLessBTree.insert(24);
	intLessBTree.insert(15);
	intLessBTree.insert(33);
	intLessBTree.insert(14);
	intLessBTree.insert(20);
	intLessBTree.insert(82);
	intLessBTree.insert(21);
	intLessBTree.insert(25);
	intLessBTree.insert(31);
	intLessBTree.insert(18);
	intLessBTree.insert(5);
	intLessBTree.insert(15);
	intLessBTree.insert(9);

	cout << "Number of nodes in the tree: " << intLessBTree.getCount() << endl;
	
	// testing some traversal algorithm:
	// first test: accumulate all the numbers in intLessBTree using 3 different traversals
	int total{ 0 };
	auto Accumulator{ [&total](int val) { total += val; } };
	intLessBTree.apply(Accumulator, PreOrder);
	cout << "Apply Accumulator preOrder style: total="<< total <<endl;

	total = 0;
	intLessBTree.apply(Accumulator, InOrder);
	cout << "Apply Accumulator inOrder style: total=" << total << endl;

	total = 0;
	intLessBTree.apply(Accumulator, PostOrder);
	cout << "Apply Accumulator postOrder style: total=" << total << endl;

	// add pritn observer:
	//cout << "TODO: add print observer, to print all the data in the tree. data must be comma separated. "
	//	<<" Do it for all 3 traversal methods\n";
	cout << "\nPreOrder print: ";
	intLessBTree.print_observer(Accumulator, PreOrder);
	cout << "\nInOrder print:";
	intLessBTree.print_observer(Accumulator, InOrder);
	cout << "\nPostOrder print:";
	intLessBTree.print_observer(Accumulator, PostOrder);
	cout << endl << endl;
	
	// testing search
	cout << "search for val 31: " << intLessBTree.search_it(31) << endl;;

	cin.get();

	return 0;
}
