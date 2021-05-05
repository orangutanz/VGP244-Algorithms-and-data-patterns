#pragma once

#pragma once
#include <memory>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <vector>
#include <iomanip>
#include <queue>


namespace myAVLTree {

	enum eOrder { PreOrder, InOrder, PostOrder, LevelOrder };

	template<typename T, typename CMP>
	class AVLTree
	{
	private:
		struct Node;  // forward declaring of Node
		typedef std::shared_ptr<Node> spNode;

		spNode root;
		CMP cmp; // used to compare data of type T: cmp(T t1, T t2) --> true or false
		size_t count{};  // keep track of number of nodes in the tree

		int height(spNode p)
		{
			if (!p)
				return -1;
			return 1 + std::max(height(p->left), height(p->right)); // TODO: turn this into a non-recursive version. Once you do, remove the above printout.
			
		}

		int treeHeight() // with count_node this can be the non-recursive version of height.
		{
			if (!root)
				return -1;
			else if (count == 1)
			{
				return 0;
			}
			else if (count == 2 || count == 3)
			{
				return 1;
			}

			// height of AVL tree is h, maximum number of nodes can be ((2 of the power(h+1)) ¡V 1).
			int height = 2;
			size_t logOfN = 8; // Height of N(2) = (2 * 2 * 2) - 1 = 7 maximum number of nodes.
			while ((logOfN - 1) <= count)
			{
				logOfN *= 2;
				++height;
			}
			return height;
		}

		//    left subtree
		//          (p)       >         (q)                        
		//	       /  \       >        /   \                         
		//        (q) (n4)    >      (t)   (p)                  
		//       /  \         >     /   \  /   \	      	      
		//     (t) (n3)       >   (n1)(n2)(n3)(n4)                     
		//    /  \     	      >   
		//  (n1)(n2)         
		void rotateRight(spNode p) //left subtree
		{
			spNode q{ p->left };
			p->left = q->right;
			if(q->right)
				q->right->parent = p;
			q->right = p;
			q->parent = p->parent;
			p->parent = q;
			spNode par{ q->parent };
			if (par)
			{
				if (par->left == p)
					par->left = q;
				else
					par->right = q;
			}
			else
				root = q;
			return;
		}
		//  right subtree
		//      (p)         >            (q)   
		//	    / \         >           /   \       
		//	  (n1)(q)       >         (p)   (t)           
		//	      / \     	>        /  \   /  \           
		//	    (n2)(t)     >     (n1)(n2) (n3)(n4)             
		//	        / \   	>    
		//        (n3)(T4)                    
		void rotateLeft(spNode p) 
		{
			spNode q{ p->right };
			p->right = q->left;
			if (q->left)
				q->left->parent = p;
			q->left = p;
			q->parent = p->parent;
			p->parent = q;
			spNode par{ q->parent };
			if (par)
			{
				if (par->left == p)
					par->left = q;
				else
					par->right = q;
			}
			else
				root = q;
			return;
		}

		int balanceFactor(spNode p)
		{
			if (!p)
				return 0;
			return height(p->right) - height(p->left);
		}
		// This method is in charge of iteratively balancing the subtree under the node p
		void balance(spNode p)
		{			
			while (p) 
			{
				auto  p_bf{ balanceFactor(p) };
				if (abs(p_bf) > 2)
					assert(false);
				auto par{ p->parent };
				if (p_bf == 2)
				{
					// means it is heavy on right. so we have to do an external left rotation and possibly an internal right rotation
					if (balanceFactor(p->right) < 0) // means right subtree is heavy on the left, so have to do a right rotation on it.
						rotateRight(p->right);    // right left subtree	
					rotateLeft(p);                // right right subtree	
				}
				else if (p_bf == -2)
				{
					if(balanceFactor(p->left) > 0)
						rotateLeft(p->left);     //left right subtree	
					rotateRight(p);		         //left left subtree			
				}
				p = par;
			}
		}

		// This is internal iterative insert method.
		void insert(spNode p, T val)
		{
			//first use iterative style to figure what where to insert the data:
			spNode node2Insert{};
			spNode temp{ p };
			while (temp != nullptr)
			{
				node2Insert = temp;
				if (cmp(val, temp->data) == true)
					temp = temp->left;
				else
					temp = temp->right;
			}

			// now create a child and attach it to node2Insert.
			auto newnode{ std::make_shared<Node>(val) };
			newnode->parent = node2Insert;
			if (cmp(val, node2Insert->data))
				node2Insert->left = newnode;
			else
				node2Insert->right = newnode;

			balance(node2Insert);
		}

		// TODO: complete the implementation
		bool remove(spNode p, T val)
		{
			// first search to see if val is in the tree:
			bool result{ false };
			spNode curr = p;
			while (curr && result == false)
			{
				if (curr->data == val)
				{
					result = true;
					break;
				}
				else
				{
					if (val < curr->data)
						curr = curr->left;
					else
						curr = curr->right;
				}
			}

			if (result == false)
				return false;  // so nothing to remove
			--count;
			//now we have found the node curr to delete. See which of the 3 cases it is:
			// cas1: curr is an external node: in this case, simply delete the node
			auto par{ curr->parent };
			if (curr->left == nullptr && curr->right == nullptr)
			{
				spNode par{ curr->parent };
				if (par->left == curr)
					curr->left = nullptr;
				else
					curr->right = nullptr;

				curr = nullptr;  // curr.reset();
			}
			else
			{
				//case 2: if curr has only one child. In this case connect curr's child to curr's parent and then delete curr
				if (curr->left == nullptr || curr->right == nullptr)
				{
					if (curr->parent->left == curr && curr->right == nullptr)
					{						
						curr->parent->left = curr->left;
						curr->left->parent = curr->parent;
					}
					else if (curr->parent->left == curr && curr->left == nullptr)
					{
						curr->parent->left = curr->right;
						curr->right->parent = curr->parent;
					}
					else if (curr->parent->right == curr && curr->right == nullptr)
					{
						curr->parent->right = curr->left;
						curr->left->parent = curr->parent;
					}
					else
					{
						curr->parent->right = curr->right;
						curr->right->parent = curr->parent;
					}
					curr = nullptr;  // curr.reset();
				}
				else  // means curr is a fully internal node with 2 childern.
				{
					// 1- find the smallest node  ,min, in the right subtree.
					spNode min = curr->right;
					while (min->left) // loop down to find the leftmost leaf node
					{					
						min = min->left;
					}
					// 2- swap the data in curr and min
					curr->data = min->data;
					// 3- delete min from right subtree.
					if (min->right)
					{
						min->parent->left = min->right;
					}
					else
					{
						min->parent->left = nullptr;
					}
					min = nullptr;  // curr.reset();
				}
			}
			balance(par);

			return true;

		}
	public:
		struct Node
		{
			T data;
			spNode left;
			spNode right;
			spNode parent; // this is needed for retracing back up, used in balancing.
			Node(T d = {}) : data(d), left(nullptr), right(nullptr)
			{}
			~Node()
			{
			//	std::cout << "~Node:" << data << std::endl;
				left = nullptr;
				right = nullptr;
			}
			void clear()
			{
				if (left)
				{
					left->clear();
					left.reset();
				}
				if (right)
				{
					right->clear();
					right.reset();
				}
			}
		};

		AVLTree() : root(nullptr) {}

		void clear()
		{
			if(root)
				root->clear();
			root = nullptr;
			count = 0;
		}

		size_t getCount() const { return count; }
		int getHeight() { return treeHeight(); }

		// interface for inserting data in the tree:
		void insert(T val)
		{
			++count;
			// if p is null, this means create a node and return it.
			if ( root == nullptr)
			{
				root = std::make_shared<Node>(val);
			}
			else
				insert(root, val);
		}

		// interface for deleting a data item:
		bool remove(T val)
		{
			return remove(root, val);
		}

		// traversal support:
		template<typename Visitor>
		void applyPreOrderVisit(Visitor ob, spNode node)
		{
			if (node == nullptr)
				return;
			ob(node->data);
			applyPreOrderVisit(ob, node->left);
			applyPreOrderVisit(ob, node->right);
		}

		template<typename Visitor>
		void applyInOrderVisit(Visitor ob, spNode node)
		{
			if (node == nullptr)
				return;
			applyInOrderVisit(ob, node->left);
			ob(node->data);
			applyInOrderVisit(ob, node->right);
		}

		template<typename Visitor>
		void applyPostOrderVisit(Visitor ob, spNode node)
		{
			if (node == nullptr)
				return;
			applyInOrderVisit(ob, node->left);
			applyInOrderVisit(ob, node->right);
			ob(node->data);
		}
		
		template<typename Visitor>
		void applyLevelOrderVisit(Visitor ob, spNode node)
		{
			if (node == nullptr)
				return;

			std::queue<spNode> que;
			que.push(node);
			spNode ptr = node;

			while (!que.empty())
			{
				ptr = que.front();
				if (ptr) 
				{					
					ob(ptr->data);
					que.pop();
				}
				if (ptr->left)
				{
					que.push(ptr->left);
				}
				if (ptr->right)
				{
					que.push(ptr->right);
				}
			}

		}


		template<typename Visitor>
		void apply(Visitor ob, eOrder order)
		{
			switch (order)
			{
			case PreOrder:
				applyPreOrderVisit(ob, root);
				break;

			case InOrder:
				applyInOrderVisit(ob, root);
				break;

			case PostOrder:
				applyPostOrderVisit(ob, root);
				break;

			case LevelOrder:
				applyLevelOrderVisit(ob, root);
				break;

			default:
				std::cout << "ERROR! apply:: Wrong order was passed here\n";
				exit(1);
			}
		}

		// this method searches for val using iterative emthod.
		// if found, return true, otherwise false.
		// TODO: student implement the emthod.
		bool search_it(T val)
		{
			bool result{ false };
			spNode curr = root;
			while (curr && result == false)
			{
				if (curr->data == val)
				{
					result = true;
					break;
				}
				else
				{
					if (val < curr->data)
						curr = curr->left;
					else
						curr = curr->right;
				}
			}
			return result;
		}

		// print the tree inOrder style up to num number of levels.
		void printLevels(int num = 0)
		{
			prettyPrint(num);
		}
	protected:
		// counts the number of data node in subtree under p, in the given range: recursive
	/*	size_t range_count(spNode p, T low, T high)
		{
			// Base case:
			if (p == nullptr)
				return 0;

			// if p node in in the range, then it is counted and also we need to count the number of nodess in the left
			// and right subtree:
			if (p->data >= low && p->data <= high)
			{
				return 1 + range_count(p->left, low, high) + range_count(p->right, low, high);
			}

			if (p->data < low)
				return range_count(p->right, low, high);

			// means node->data > high
			return range_count(p->left, low, high);
		}
	*/


		void prettyPrint(int levels)
		{
			spNode node{ root };
			using std::cout;
			using std::endl;
			using std::setw;
			using std::string;

			spNode emptyNode{};
			int maxLevel{ levels<=0 ? height(node) : levels};
			std::vector<spNode> vec;
			vec.push_back(node);
			int offset{ 0 };
			while (!vec.empty() && maxLevel >= 0)
			{
				std::vector<spNode> levelNodes;
				while (!vec.empty())
				{
					string str = vec.back() ? "(" + std::to_string(vec.back()->data) + ")" : "( )";
					cout << setw(3 + offset /2 + maxLevel * 3) << str;
					levelNodes.push_back(vec.back());
					vec.pop_back();
				}
				++offset;
				cout << endl << endl;
				while (!levelNodes.empty())
				{
					spNode& nd = levelNodes.back();
					levelNodes.pop_back();
					if (nd && nd->right)
						vec.push_back(nd->right);
					else
						vec.push_back(emptyNode);
					if (nd && nd->left)
						vec.push_back(nd->left);
					else
						vec.push_back(emptyNode);
				}
				--maxLevel;
			}

		}


	};

}
