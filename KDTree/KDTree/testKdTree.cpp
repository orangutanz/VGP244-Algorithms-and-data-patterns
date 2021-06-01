#include <array>
#include <iostream>

#include "kdtree.h"

using namespace vgp244;

int main()
{
	std::array<Point<int>, 20> pnts{
		Point<int>{2, 3}, {5, 9}, {2, -3}, {-5, 9}, {2, 33}, {51, -9}, {12, 13}, {65, 9}, {22, 53}, {5, 19},
					{15, 19}, {12, -3}, {-5, 19}, {12, 33}, {1, -9}, {2, 13}, {25, 21}, {32, 23}, {35, 29}, {6,6} };

	KDTree<int> kd3;
	for (auto pt : pnts)
		kd3.insert(pt);

	kd3.printTree();


	auto minx{ kd3.findMin(0) };
	auto miny{ kd3.findMin(1) };
	auto maxx{ kd3.findMax(0) };
	auto maxy{ kd3.findMax(1) };

	std::cout << "Min x point: " << minx.x << ", " << minx.y << std::endl;
	std::cout << "Min y point: " << miny.x << ", " << miny.y << std::endl;
	std::cout << "Max x point: " << maxx.x << ", " << maxx.y << std::endl;
	std::cout << "Max y point: " << maxy.x << ", " << maxy.y << std::endl;

	std::cout << "\n PrintRange (10, 10), (20,20): " << std::endl;
	kd3.printRange({ 10,10 }, { 20,20 });
	std::cout  << std::endl;

	std::cout << "find (3, 3)? " << kd3.find({ 3, 3 }) << std::endl;

	std::cout << "find (2, 33)? " << kd3.find({ 2, 33 }) << std::endl;

	std::cout << "delete (2, 33), (12, 33), (51, -9) and print:\n";
	kd3.remove({ 2, 33 });
	kd3.remove({ 12, 33 });
	kd3.remove({ 51, -9 });
	kd3.printTree();

	std::cout << "\nEnd of Testing KD Tree basic operations.\n";

	return 0;

}