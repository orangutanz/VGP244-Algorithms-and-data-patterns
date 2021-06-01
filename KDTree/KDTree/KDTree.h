#pragma once
#include <iostream>

namespace vgp244 {
	template<typename T>
	struct Point {
		T x, y;
		Point(T x = T{}, T y = T{}) : x(x), y(y) {}
		bool operator==(const Point<T>& rhs)  // comparision operator overriding
		{
			return (rhs.x == x && rhs.y == y);
		}

		T operator[](int idx)  // subscript operator overriding
		{
			if (idx == 0)
				return x;
			return y;
		}
	};

	template<typename T>
	class KDTree
	{
	private:
		template <typename T>
		struct node
		{
			Point<T> data;
			node<T>* left;
			node<T>* right;
			node(Point<T> p) : data(p), left(nullptr), right(nullptr) {}
		};

		node<T>* root{ nullptr };

		/* recursively insert point p in node n*/
		node<T>* insert(Point<T> p, node<T>* n, int cd)
		{
			if (n == nullptr)
				n = new node<T>(p);
			else if (p == n->data)
			{
				std::cerr << "ERROR! Duplicate points are not allowed!\n";
				return nullptr;
			}
			else if (p[cd] < n->data[cd])
				n->left = insert(p, n->left, (cd + 1) % 2);
			else
				n->right = insert(p, n->right, (cd + 1) % 2);

			return n;
		}

		// recursively finds node with minimum value for d dimension, starting from node n with cutting dimension cd.
		node<T>* findMin(node<T>* n, int d, int depth)
		{ 
			// Base cases
			if (n == nullptr)
				return nullptr;

			// Current dimension is computed using current depth and total dimensions (2)
			unsigned cd = depth % 2;

			// Compare point with n with respect to cd (Current dimension)
			if (cd == d) 
			{
				if (n->left == nullptr)
					return n;
				return (n->data, findMin(n->left, d, depth + 1));
			}
			// If current dimension is different then minimum can be anywher in this subtree
			return (n->data, findMin(n->left, d, depth + 1), findMin(n->right, d, depth + 1));
		}

		// recursively finds node with maximum value for d dimension, starting from node n with cutting dimension cd.
		node<T>* findMax(node<T>* n, int d, int depth)
		{
			// Base cases
			if (n == nullptr)
				return nullptr;

			// Current dimension is computed using current depth and total dimensions (2)
			unsigned cd = depth % 2;

			// Compare point with n with respect to cd (Current dimension)
			if (cd == d)
			{
				if (n->right == nullptr)
					return n;
				return (n->data, findMax(n->right, d, depth + 1));
			}
			// If current dimension is different then minimum can be anywher in this subtree

			return (n->data, findMax(n->left, d, depth + 1), findMax(n->right, d, depth + 1));
		}

		// recursively prints all the points in the given range, starting from node n.
		void printRange(Point<T> low, Point<T> high, node<T>* n, int cd)
		{
			if ((n->data[0] >= low[0]) && (n->data[0] <= high[0])&& (n->data[1] >= low[1]) && (n->data[1] <= high[1]))
			{
				std::cout << "(" <<n->data[0] << "," << n->data[1] << ") ";
			}
			if (n->left != NULL)
			{
				printRange(low, high, n->left, (cd + 1) % 2);
			}
			if (n->right != NULL)
			{
				printRange(low, high, n->right, (cd + 1) % 2);
			}			

			//std::cerr << "printRange: TODO by students\n";
		}

		/* recursively looks for data p in the tree. If found return true.
		* steps are similar to printRange: 
		1) check if n.data == p, if so return true. else 
		2) find out if you need to call find(p, n.left, (cd+1)%2 or find(p, n.right, (cd+1)%2)
		*/
		bool find(Point<T> p, node<T>* n, int depth)
		{
			// Base cases
			if (n == nullptr)
				return false;
			if (p == n->data)
				return true;

			unsigned cd = depth % 2;

			//(p, n.left, (cd+1)%2 or find(p, n.right, (cd+1)%2)
			if (p[cd] < n->data[cd])
				return find(p, n->left, depth + 1);

			return find(p, n->right, depth + 1);
		}

		// recursively removes the node with data p from the tree.
		// NOTE: remove should be called only if p actually exist in the tree. 
		node<T>* remove(Point<T> p, node<T>* n, int depth)
		{
			// Given point is not present
			if (n == nullptr)
				return nullptr;

			// Find dimension of current node
			int cd = depth % 2;

			// If the point to be deleted is present at root
			if (n->data == p)
			{
				// 2.b) If right child is not NULL
				if (n->right != NULL)
				{
					// Find minimum of root's dimension in right subtree
					n->data = n->right->data;
					n->right = remove(n->right->data, n->right, depth + 1);
				}
				else if (n->left != NULL) // same as above
				{
					n->data = n->left->data;
					n->left = remove(n->left->data, n->left, depth + 1);
				}
				else // If node to be deleted is leaf node
				{
					delete n;
					return nullptr;
				}
				return n;
			}
			// 2) If current node doesn't contain point, search downward
			if (p[cd] < n->data[cd])
				n->left = remove(p, n->left, depth + 1);
			else
				n->right = remove(p, n->right, depth + 1);
			return n;					

			//std::cerr << "remove: TODO by students\n";
			//return nullptr;
		}
	public:
		void insert(Point<T> p)
		{
			root = insert(p, root, 0);
		}

		Point<T> findMin(int d)
		{
			node<T>* foundNode{ findMin(root, d, 0) };
			if (foundNode == nullptr)
			{
				std::cerr << "ERROR! findMin returned null. Maybe the tree is empty?\n";
				return Point<T>{};
			}
			return foundNode->data;
		}

		Point<T> findMax(int d)
		{
			node<T>* foundNode{ findMax(root, d, 0) };
			if (foundNode == nullptr)
			{
				std::cerr << "ERROR! findMax returned null. Maybe the tree is empty?\n";
				return Point<T>{};
			}
			return foundNode->data;
		}

		// prints all the points in the tree which fall in the rectangular range (low, high). This means all points p : low[0]<= p[0] <= high[0]  && low[1]<= p[1] <= high[1]
		void printRange(Point<T> low, Point<T> high)
		{
			printRange(low, high, root, 0);
		}

		bool find(Point<T> p)
		{
			return find(p, root, 0);
		}
		void remove(Point<T> p)
		{
			if (find(p))
				root = remove(p, root, 0);
		}

		// uses printRange to print the tree
		void printTree()
		{
			auto xmin{ findMin(0) };
			auto xmax{ findMax(0) };
			auto ymin{ findMin(1) };
			auto ymax{ findMax(1) };
			std::cout << "printTree: tree range is low:(" << xmin.x << ", " << ymin.y << "), high:(" << xmax.x << ", " << ymax.y << ").\n ";
			
			printRange((xmin,xmax),(ymin,ymax));
		}
	};  // kdtree

}  // namespace
