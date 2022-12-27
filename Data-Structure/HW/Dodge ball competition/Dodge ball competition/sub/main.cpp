// no
#include<iostream>
#include<vector>
#include<string>

using std::vector;
using std::cout;

class Pair {
public:
	int data;
	int height;
	Pair() :data(-1), height(-1) {}
	Pair(int d, int h) :data(d), height(h) {}
};

class Team {
	vector<Pair> allPlayer;
	int jbPlayer;
	int* oPlayer;

	void chooseJBP() { jbPlayer = allPlayer[0].data; }
	void chooseOP();
public:
	Team() :oPlayer(nullptr), jbPlayer(-1) {}
	Team(const Team& a);
	~Team() { delete oPlayer; }

	int _jbPlayer() { return jbPlayer; }
	int _oPlayer(int num);
	int size() { return allPlayer.size(); } // size of team
	void addPlayer(const Pair& a) { allPlayer.push_back(a); } // add player to the whole team
	void ascendingOrder();
	void choosePlayer(); // choose players to be a jbPlayer or oPlayer

	int& operator[](int num) { return allPlayer[num].data; }
};
class Node {
	int data;
	bool red;
	Node* left;
	Node* right;
	Node* parent;
public:
	Node() : red(true), left(nullptr), right(nullptr), parent(nullptr), data(-1) {}
	Node(const int& d, Node* p) : red(true), left(nullptr), right(nullptr), parent(p), data(d) {}
	Node(const Node& a);
	~Node() { delete left, right, parent; }

	int _data() { return data; }
	bool _red() { return red; }
	Node* _left() { return left; }
	Node* _right() { return right; }
	Node* _parent() { return parent; }

	void setRed(bool b) { red = b; }
	void set_new_Left(const int& d, Node* p) { left = new Node(d, p); } // new child
	void set_new_Right(const int& d, Node* p) { right = new Node(d, p); }
	void setLeft(Node* ptr) { left = ptr; } // change child
	void setRight(Node* ptr) { right = ptr; }
	void setparent(Node* ptr) { parent = ptr; } // change parent
};
class RBTree {
	Node* root;

	void changeColor(Node*& ptr);
	void RR(Node*& ptr);
	void RL(Node*& ptr);
	void LR(Node*& ptr);
	void LL(Node*& ptr);
	void rotate(Node*& ptr);
	void grouping(Node* ptr, Team& r, Team& b, int h);
public:
	RBTree() :root(nullptr) {}
	RBTree(Node* r) :root(r) {}
	RBTree(const RBTree& a);
	~RBTree() { delete root; }

	Node* getRoot() { return root; }
	void insert(int num);
	bool assign(Team& r, Team& b); // assign members to each team
	int find_height(Node* ptr); // height of the tree
};

void print(Team& red, Team& black); // output format

int main() {
	int input;
	RBTree tree;
	Team red, black;
	while (std::cin >> input) {
		tree.insert(input);
	}
	if (tree.assign(red, black))
	{
		red.choosePlayer();
		black.choosePlayer();
		print(red, black);
	}
	else {
		std::cout << "No game";
	}
}

void print(Team& red, Team& black) {
	cout << "Red team: ";
	for (int i = 0; i < red.size(); i++) {
		cout << red[i];
		if (i != red.size() - 1) cout << ",";
	}
	cout << std::endl << "outfield: ";
	for (int i = 0; i < 3; i++) {
		cout << red._oPlayer(i);
		if (i != 2) cout << ",";
	}
	cout << std::endl << "jump ball: " << red._jbPlayer() << std::endl;

	// black
	cout << "Black team: ";
	for (int i = 0; i < black.size(); i++) {
		cout << black[i];
		if (i != black.size() - 1) cout << ",";
	}
	cout << std::endl << "outfield: ";
	for (int i = 0; i < 3; i++) {
		cout << black._oPlayer(i);
		if (i != 2) cout << ",";
	}
	cout << std::endl << "jump ball: " << black._jbPlayer();
}

void RBTree::changeColor(Node*& ptr)
{
	ptr->setRed(true);
	ptr->_left()->setRed(false);
	ptr->_right()->setRed(false);
}

void RBTree::RR(Node*& ptr)
{
	Node* top = ptr->_right();
	if (ptr == root) root = top;
	ptr->setRight(top->_left());
	if (top->_left() != nullptr) top->_left()->setparent(ptr);
	top->setparent(ptr->_parent());
	if (top != root) { // parent -> ptr 
		if (top->_data() >= ptr->_parent()->_data()) ptr->_parent()->setRight(top);
		else ptr->_parent()->setLeft(top);
	}
	top->setLeft(ptr);
	ptr->setparent(top);
	top->_right()->setparent(top);
	top->_left()->setparent(top);
	// set color
	top->setRed(false);
	top->_left()->setRed(true);
	top->_right()->setRed(true);

}

void RBTree::RL(Node*& ptr)
{
	Node* top = ptr->_right()->_left();
	if (ptr == root) root = top;
	ptr->setRight(top->_left());
	if (top->_left() != nullptr) top->_left()->setparent(ptr);
	top->_parent()->setLeft(top->_right());
	if (top->_right() != nullptr) top->_right()->setparent(top->_parent());
	top->setLeft(ptr);
	top->setRight(top->_parent());
	top->_parent()->setparent(top);

	top->setparent(ptr->_parent());
	if (top != root) {
		if (top->_data() >= ptr->_parent()->_data()) ptr->_parent()->setRight(top);
		else ptr->_parent()->setLeft(top);
	}
	ptr->setparent(top);
	top->_right()->setparent(top);
	top->_right()->setRed(true);
	top->_left()->setRed(true);
	top->setRed(false);
	top->_left()->setparent(top);
}

void RBTree::LR(Node*& ptr)
{
	Node* top = ptr->_left()->_right();
	if (ptr == root) root = top;
	ptr->setLeft(top->_right());
	if (top->_right() != nullptr) top->_right()->setparent(ptr);
	top->_parent()->setRight(top->_left());
	if (top->_left() != nullptr) top->_left()->setparent(top->_parent());
	top->setRight(ptr);
	top->setLeft(top->_parent());
	top->_parent()->setparent(top);
	top->setparent(ptr->_parent());
	if (top != root) {
		if (top->_data() >= ptr->_parent()->_data()) ptr->_parent()->setRight(top);
		else ptr->_parent()->setLeft(top);
	}
	ptr->setparent(top);
	top->_right()->setparent(top);
	top->_left()->setparent(top);
	top->_right()->setRed(true);
	top->setRed(false);
	top->_left()->setRed(true);

}

void RBTree::LL(Node*& ptr)
{
	Node* top = ptr->_left();
	if (ptr == root) root = top;
	ptr->setLeft(top->_right());
	if (top->_right() != nullptr) top->_right()->setparent(ptr);
	top->setRight(ptr);	top->setparent(ptr->_parent());
	if (top != root) {
		if (top->_data() >= ptr->_parent()->_data()) ptr->_parent()->setRight(top);
		else ptr->_parent()->setLeft(top);
	}
	ptr->setparent(top);
	top->_right()->setparent(top);
	top->_left()->setparent(top);
	top->_right()->setRed(true);
	top->setRed(false);
	top->_left()->setRed(true);

}

void RBTree::rotate(Node*& ptr)
{
	if (ptr == nullptr) return;
	if (ptr->_left() != nullptr && ptr->_left()->_red() &&
		ptr->_left()->_left() != nullptr && ptr->_left()->_left()->_red()) return LL(ptr);
	if (ptr->_left() != nullptr && ptr->_left()->_red() &&
		ptr->_left()->_right() != nullptr && ptr->_left()->_right()->_red()) return LR(ptr);
	if (ptr->_right() != nullptr && ptr->_right()->_red() &&
		ptr->_right()->_left() != nullptr && ptr->_right()->_left()->_red()) return RL(ptr);
	if (ptr->_right() != nullptr && ptr->_right()->_red() &&
		ptr->_right()->_right() != nullptr && ptr->_right()->_right()->_red()) return RR(ptr);
}

RBTree::RBTree(const RBTree& a)
{
	root = new Node;
	*root = *a.root;
}

void RBTree::insert(int num)
{
	if (root == nullptr) { // insert root
		root = new Node(num, nullptr);
		root->setRed(false);
		return;
	}
	Node* cur = root, * pre = nullptr;
	while (cur != nullptr) {
		if (cur->_right() != nullptr && cur->_left() != nullptr // if two children
			&& cur->_left()->_red() && cur->_right()->_red()) { // and both are red
			changeColor(cur);
			if (cur != root && cur->_parent()->_red()) { // check two consequent red node -> rotation
				Node* temp = cur->_parent()->_parent();
				rotate(temp);
			}
		}
		// traverse
		if (num >= cur->_data()) {
			pre = cur;
			cur = cur->_right();
			continue;
		}
		pre = cur;
		cur = cur->_left();
	}
	if (num >= pre->_data()) pre->set_new_Right(num, pre);
	else pre->set_new_Left(num, pre);
	// check two consequent red node -> rotation
	if (pre->_red()) {
		Node* temp = pre->_parent();
		rotate(temp);
	}
	root->setRed(false);
}

bool RBTree::assign(Team& r, Team& b)
{
	int max = find_height(root);
	for (int i = 1; i <= max; i++) {
		grouping(root, r, b, i);
	}
	if (r.size() < 4 || b.size() < 4) return false;
	return true;
}

void RBTree::grouping(Node* ptr, Team& r, Team& b, int h)
{
	if (ptr == nullptr)	return;
	if (h == 1)
		if (ptr->_red()) r.addPlayer(Pair(ptr->_data(), h));
		else b.addPlayer(Pair(ptr->_data(), h));
	else if (h > 1) {
		grouping(ptr->_left(), r, b, h - 1);
		grouping(ptr->_right(), r, b, h - 1);
	}
}

int RBTree::find_height(Node* ptr)
{
	if (ptr == nullptr)
		return 0;
	else {
		int lheight = find_height(ptr->_left());
		int rheight = find_height(ptr->_right());

		if (lheight > rheight) {
			return (lheight + 1);
		}
		else {
			return (rheight + 1);
		}
	}
}

Team::Team(const Team& a) : jbPlayer(a.jbPlayer)
{
	allPlayer = a.allPlayer;
	oPlayer = new int;
	*oPlayer = *a.oPlayer;
}

int Team::_oPlayer(int num)
{
	if (num < 3) return oPlayer[num];
	return -1;
}

void Team::ascendingOrder()
{
	for (int i = 0; i < allPlayer.size(); i++) {
		int smallest_d = allPlayer[i].data;
		int index = -1;
		for (int s = i; s < allPlayer.size(); s++) {
			if (allPlayer[s].data < smallest_d) {
				smallest_d = allPlayer[s].data;
				index = s;
			}
		}
		if (index != -1) {
			Pair temp = allPlayer[i];
			allPlayer[i] = allPlayer[index];
			allPlayer[index] = temp;
		}
	}
}

void Team::choosePlayer()
{
	chooseJBP();
	chooseOP();
	ascendingOrder();
}

void Team::chooseOP()
{
	oPlayer = new int[3];
	int index = 0;
	if (allPlayer.size() < 3) return;
	for (int i = allPlayer.size() - 3; i < allPlayer.size(); i++) {
		oPlayer[index] = allPlayer[i].data;
		index++;
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2 - i; j++) {
			if (oPlayer[j] > oPlayer[j + 1]) {
				int temp = oPlayer[j];
				oPlayer[j] = oPlayer[j + 1];
				oPlayer[j + 1] = temp;
			}
		}
	}
}

Node::Node(const Node& a) :data(a.data), red(a.red) {
	left = new Node;
	*left = *a.left;
	right = new Node;
	*right = *a.right;
	parent = new Node;
	*parent = *a.parent;
}
