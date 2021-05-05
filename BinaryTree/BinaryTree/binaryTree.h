#pragma once
#include <memory>
#include <iostream>

namespace myBinaryTree {

	enum eOrder { PreOrder, InOrder, PostOrder};

	template<typename T, typename CMP>
	class BinTree
	{
	private:
		struct Node;  // forward declaring of Node
		typedef std::shared_ptr<Node> spNode;

		spNode root;
		CMP cmp; // used to compare data of type T: cmp(T t1, T t2) --> true or false
		size_t count{};  // keep track of number of nodes in the tree

		void insert_recursive(Node node, T val)
		{
			/* If the tree is empty, return a new Node */
			if (node == nullptr)
			{
				count++;
				node = std::make_shared<Node>(val);
				return;
			}
			if (cmp(val, node->data))
			{
				node->left = insert_recursive(node->left, val);
			}
			else
			{
				node->right = insert_recursive(node->right, val);
			}
		}
	public:
		struct Node
		{
			T data;
			spNode left;
			spNode right;
			Node(T d = {}) : data(d), left(nullptr), right(nullptr)
			{}
			~Node()
			{
				left = nullptr;
				right = nullptr;
			}
		};

		BinTree() : root(nullptr) {}

		size_t getCount() const { return count; }

		// This is iterative insert method.
		void insert(T val)
		{
			++count;
			//std::cout << "insert: " << val << "\n";
			// first check if root has not be set yet. If not, then set it to Node that contains val. (hint use std::make_shared, reset(new Node(val))
			if (root == nullptr)
			{
				//root.reset(new Node(val));  // root = std::make_shared<Node>(val)
				root = std::make_shared<Node>(val);
				return;
			}

			// use iterative style to figure what where to insert the data
			spNode node2Insert{};
			spNode temp{ root };
			while (temp != nullptr)
			{
				node2Insert = temp;
				if (cmp(val, temp->data) == true)
					temp = temp->left;
				else
					temp = temp->right;
			}

			// now create a child and attach it to node2Insert.
			if (cmp(val, node2Insert->data))
				node2Insert->left = std::make_shared<Node>(val);
			else
				node2Insert->right = std::make_shared<Node>(val);
		}

		// This is recursive version of insert. TODO: implement this, you'd probably need to add another private method to the BTree or Node.
		void insert_recursive(T val)
		{
			if (root == nullptr)
			{
				count++;
				std::cout << "insert_recursive: " << val << "\n";
				return root = std::make_shared<Node>(val);
			}

			spNode temp{ root };
			if (cmp(val, temp->data) == true)
			{
				insert_recursive(temp->left, val);
			}
			else
			{
				insert_recursive(temp->right, val);
			}
			//std::cout << "insert_recursive: student to implement\n";
		}

		// traversal support:
		template<typename Visitor>
		void applyPreOrderVisit(Visitor ob, spNode node, bool print)
		{
			if (node == nullptr)
				return;
			ob(node->data);
			if (print)
				std::cout << node->data << ", ";
			applyPreOrderVisit(ob, node->left, print);
			applyPreOrderVisit(ob, node->right, print);
		}

		template<typename Visitor>
		void applyInOrderVisit(Visitor ob, spNode node, bool print)
		{
			if (node == nullptr)
				return;
			applyPreOrderVisit(ob, node->left, print);
			ob(node->data);
			if (print)
				std::cout << node->data << ", ";
			applyPreOrderVisit(ob, node->right, print);
			//std::cout << "applyInOrderVisit: student implement\n";
		}

		template<typename Visitor>
		void applyPostOrderVisit(Visitor ob, spNode node, bool print)
		{
			if (node == nullptr)
				return;
			applyPreOrderVisit(ob, node->left, print);
			applyPreOrderVisit(ob, node->right, print);
			ob(node->data);
			if (print)
				std::cout << node->data << ", ";
			//std::cout << "applyPostOrderVisit: student implement\n";
		}

		template<typename Visitor>
		void apply(Visitor ob, eOrder order)
		{
			switch (order)
			{
			case PreOrder:
				applyPreOrderVisit(ob, root, false);
				break;
				
			case InOrder:
				applyInOrderVisit(ob, root, false);
				break;

			case PostOrder:
				applyPostOrderVisit(ob, root, false);
				break;

			default:
				std::cout << "ERROR! apply:: Wrong order was passed here\n";
				exit(1);
			}
		}

		template<typename Visitor>
		void print_observer(Visitor ob, eOrder order)
		{
			switch (order)
			{
			case PreOrder:
				applyPreOrderVisit(ob, root, true);
				break;

			case InOrder:
				applyInOrderVisit(ob, root, true);
				break;

			case PostOrder:
				applyPostOrderVisit(ob, root, true);
				break;

			default:
				std::cout << "ERROR! print_observer:: Wrong order was passed here\n";
				exit(1);
			}
		}

		// this method searches for val using iterative emthod.
		// if found, return true, otherwise false.
		// TODO: student implement the emthod.
		bool search_it(T val)
		{
			bool result{ false };

			spNode temp{ root };
			while (temp != nullptr)
			{
				if (val == temp->data)
				{
					result = true;
					break;
				}
				else if (cmp(val,temp->data) == true)
				{
					temp = temp->left;
				}
				else
				{
					temp = temp->right;
				}
			}
			if (result == true)
			{
				std::cout << "Search found: " << temp->data << "\n";
			}
			else
			{
				std::cout << "Search not found." << "\n";
			}
			//std::cout << "search_it: student must implement\n";
			return result;
		}

	};

}
