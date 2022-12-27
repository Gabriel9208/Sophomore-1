#include<iostream>

class Node {
	int data;
	bool red;
	Node* left;
	Node* right;
	Node* parant;
public:
	Node() : red(true), left(nullptr), right(nullptr), parant(nullptr), data(-1) {}
	Node(int d, Node* p) : red(true), left(nullptr), right(nullptr), parant(p), data(d) {}
	~Node() { delete left, right, parant; }
	int _data() { return data; }
	bool _red() { return red; }
	Node* _left() { return left; }
	Node* _right() { return right; }
	Node* _parant() { return parant; }

	void setRed(bool b) { red = b; }
	void setLeft(int d, Node* p) { left = new Node(d,p); }
	void setRight(int d, Node* p) { right = new Node(d, p); }
	void setLeft(Node* ptr) { left = ptr; }
	void setRight(Node* ptr) { right = ptr; }
	void setParant(Node* ptr) { parant = ptr; }
};
class RBTree {
	Node* root;
	void changeColor(Node*& ptr);
	Node* RR(Node*& ptr);
	Node* RL(Node*& ptr);
	Node* LR(Node*& ptr);
	Node* LL(Node*& ptr);
	Node* rotate(Node*& ptr);
public:
	RBTree() :root(nullptr) {}
	RBTree(Node* r) :root(r) {}
	Node* getRoot() { return root; }
	void insert(int num);
	void inOrder(Node* ptr);
};
int main() {
	int input = 0;
	RBTree tree;
	while (std::cin >> input) {
		tree.insert(input);
	}
	tree.inOrder(tree.getRoot());
}

void RBTree::changeColor(Node*& ptr)
{
	ptr->setRed(true);
	ptr->_left()->setRed(false);
	ptr->_right()->setRed(false);
}

Node* RBTree::RR(Node*& ptr)
{
	Node* top = ptr->_right();
	if (ptr == root) root = top;
	ptr->setRight(top->_left());
	top->setLeft(ptr);
	return top;
}

Node* RBTree::RL(Node*& ptr)
{
	Node* top = ptr->_right()->_left();
	if (ptr == root) root = top;
	ptr->setRight(top->_left());
	top->_parant()->setLeft(top->_right());
	top->setLeft(ptr);
	top->setRight(top->_parant());
	return top;
}

Node* RBTree::LR(Node*& ptr)
{
	Node* top = ptr->_left()->_right();
	if (ptr == root) root = top;
	ptr->setLeft(top->_right());
	top->_parant()->setRight(top->_left());
	top->setRight(ptr);
	top->setLeft(top->_parant());
	return top;
}

Node* RBTree::LL(Node*& ptr)
{
	Node* top = ptr->_left();
	if (ptr == root) root = top;
	ptr->setLeft(top->_right());
	top->setRight(ptr);
	return top;
}

Node* RBTree::rotate(Node*& ptr)
{
	if (ptr->_left() != nullptr && ptr->_left()->_red() && 
		ptr->_left()->_left() != nullptr && ptr->_left()->_left()->_red()) return LL(ptr);
	if (ptr->_left() != nullptr && ptr->_left()->_red() && 
		ptr->_left()->_right() != nullptr && ptr->_left()->_right()->_red()) return LR(ptr);
	if (ptr->_right() != nullptr && ptr->_right()->_red() && 
		ptr->_right()->_left() != nullptr && ptr->_right()->_left()->_red()) return RL(ptr);
	if (ptr->_right() != nullptr && ptr->_right()->_red() && 
		ptr->_right()->_right() != nullptr && ptr->_right()->_right()->_red()) return RR(ptr);
	return ptr;
}

void RBTree::insert(int num)
{
	Node* ptr = root;
	bool end = false;
	if (root == nullptr) { // insert root
		root = new Node(num, nullptr);
		root->setRed(false);
		return;
	}
	while (!end) {
		if (ptr->_right() != nullptr && ptr->_left() != nullptr) { 
			// check two red children
			if (ptr->_left()->_red() && ptr->_right()->_red()) changeColor(ptr);
			// check two consequent red node -> rotation
			if (ptr != root && ptr->_parant() != root) {
				ptr = ptr->_parant()->_parant();
				ptr = rotate(ptr);
			}
		}
		// traverse
		if (num >= ptr->_data()) {
			if (ptr->_right() == nullptr) end = true;
			else ptr = ptr->_right();
		}
		else {
			if (ptr->_left() == nullptr) end = true;
			else ptr = ptr->_left();
		}
	}
	if (num >= ptr->_data()) ptr->setRight(num, ptr);
	else ptr->setLeft(num, ptr);
	// check two consequent red node -> rotation
	if (ptr->_red()) {
		ptr = ptr->_parant();
		// rotation
		ptr = rotate(ptr);
	}
	root->setRed(false);
}

void RBTree::inOrder(Node* ptr)
{
	if (ptr == nullptr) return;
	inOrder(ptr->_left());
	std::cout << ptr->_data() << " ";
	inOrder(ptr->_right());
}

