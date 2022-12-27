#include<iostream>
#include<string>
using std::cin;
using std::cout;
using std::string;
using std::endl;
using std::to_string;

struct node{
	int data;
	struct node* rptr;
	struct node* lptr;
	struct node* parent;
};
typedef struct node node;

class Tree {
	node* root_ptr;
	node root;
public:
	Tree() { root_ptr = nullptr; root.parent = nullptr; root.rptr = nullptr; root.lptr = nullptr; }
	Tree(int num);
	//~Tree() { delete root_ptr; }
	void insert(int num);
	void in_order(node* p, string& str);
	void pre_order(node* p, string& str);
	void post_order(node* p, string& str);
	node* get_tree_root_ptr() { return root_ptr; }
	void print_root() { cout << root.data << endl; }
};

int main() {
	string str = "";
	int input, input_amount = 1;
	cin >> input;
	if (!cin.eof()) {
		Tree tree(input);
		while (cin >> input) {
			tree.insert(input);
			input_amount++;
		}
		node* traverse = tree.get_tree_root_ptr();
		tree.in_order(traverse, str);
		str.erase(str.end() - 1);
		cout << str << endl;
		str = "";
		traverse = tree.get_tree_root_ptr();
		tree.pre_order(traverse, str);
		str.erase(str.end() - 1);
		cout << str << endl;
		str = "";
		traverse = tree.get_tree_root_ptr();
		tree.post_order(traverse, str);
		str.erase(str.end() - 1);
		cout << str;
	}
}

Tree::Tree(int num)
{
	root.parent = nullptr; 
	root.lptr = nullptr;
	root.rptr = nullptr;
	root.data = num;
	root_ptr = &root;
}

void Tree::insert(int num)
{
	node* current = root_ptr; // used to point to the current node
	node* past = new node();
	bool left = false;
	while (current != nullptr) {
		if (num > current->data) {
			past = current;
			current = current->rptr;
			left = false;
		}
		else {
			past = current;
			current = current->lptr;
			left = true;
		}
	}
	current = new node();
	current->parent = past;
	current->lptr = nullptr;
	current->rptr = nullptr;
	current->data = num;
	if (left) {
		past->lptr = current;
		return;
	}
	past->rptr = current;
}

void Tree::in_order(node* p, string& str)
{
	if (p == nullptr) return;
	in_order(p->lptr, str);
	str += to_string(p->data) + " ";
	in_order(p->rptr, str);
}

void Tree::pre_order(node* p, string& str)
{
	if (p == nullptr) return;
	str += to_string(p->data) + " ";
	pre_order(p->lptr, str);
	pre_order(p->rptr, str);
}

void Tree::post_order(node* p, string& str)
{
	if (p == nullptr) return;
	post_order(p->lptr, str);
	post_order(p->rptr, str);
	str += to_string(p->data) + " ";
}
