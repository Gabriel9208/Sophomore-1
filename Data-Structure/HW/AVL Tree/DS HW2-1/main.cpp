#include<iostream>
#include<string>
#include<sstream>
using namespace std;

struct node {
	int data;
	struct node* lptr;
	struct node* rptr;
	int balance;
	bool is_root;
};
typedef struct node node;

class AVL_Tree {
	node* root_ptr;
	int rotate_num;
	string rotation_type;
	// insertion rotation
	void LL_rotation(node*& ptr, bool from_del = 0);
	void RR_rotation(node*& ptr, bool from_del = 0);
	void RL_rotation(node*& ptr, bool from_del = 0);
	void LR_rotation(node*& ptr, bool from_del = 0);
	// deletion insertion
	void R0_rotation(node*& ptr, char type);
	void R1_rotation(node*& ptr, char type);
	void R_1_rotation(node*& ptr, char type);
	//find ptr
	node* find(int num, node* ptr);
	node* find_max(node* ptr);

	int height(node* ptr);

public:
	AVL_Tree() { root_ptr = new node(); rotate_num = 0; rotation_type = ""; }
	AVL_Tree(int num);
	~AVL_Tree() { delete root_ptr; }
	void insert(int num, node*& ptr, bool& height_change);
	void deletion(int num, node*& ptr);
	node* get_root() { return root_ptr; }
	void in_order(node* ptr, string& str);
	int get_rotate_num() { return rotate_num; }
	string get_rotation_type() { return rotation_type; }
};

int main() {
	string input;
	string modify="";
	cin >> input;
	for (int i = 0; i < input.size(); i++) {
		if (input[i] != ',') modify += input[i];
		else modify += " ";
	}
	stringstream in(modify);
	if (!cin.eof()) {
		in >> input;
		AVL_Tree tree(stoi(input));
		while (in >> input) {
			bool  height_change = false;
			node* ptr = tree.get_root();
			tree.insert(stoi(input), ptr, height_change);
		}
		while (cin >> input) {
			bool  height_change = false;
			node* temp_ptr = tree.get_root();
			switch (input[0]) {
			case 'I':case 'i':
				cin >> input;
				tree.insert(stoi(input), temp_ptr, height_change);
				break;
			case 'D':case 'd':
				cin >> input;
				tree.deletion(stoi(input), temp_ptr);
			}
		}
		string output = "";
		node* in_order_ptr = tree.get_root();
		tree.in_order(in_order_ptr, output);
		output.erase(output.end() - 1);
		cout << output << endl << tree.get_rotate_num() << endl;
		output = tree.get_rotation_type();
		output.erase(output.end() - 1);
		cout << output;
	}
}

AVL_Tree::AVL_Tree(int num) {
	root_ptr = new node();
	root_ptr->is_root = true;
	root_ptr->data = num;
	root_ptr->balance = 0;
	root_ptr->lptr = nullptr;
	root_ptr->rptr = nullptr;
	rotate_num = 0;
	rotation_type = "";
}

void AVL_Tree::insert(int num, node*& ptr, bool& height_change) {
	if(ptr == nullptr) {
		ptr = new node;
		ptr->data = num;
		ptr->lptr = nullptr;
		ptr->rptr = nullptr;
		ptr->balance = 0;
		height_change = true;
		ptr->is_root = false;
	}
	else {
		if (num > ptr->data) { // add to the right
			insert(num, ptr->rptr, height_change);
			if (height_change) { // learn!
				switch (ptr->balance) {
				case -1:
					if (ptr->rptr->balance == -1) RR_rotation(ptr);
					else RL_rotation(ptr);
					height_change = false;
					break;
				case 0:
					ptr->balance--;
					break;
				case 1:
					ptr->balance--;
					height_change = false;
					break;
				}
			}
		}
		else { // add to the left
			insert(num, ptr->lptr, height_change);
			if (height_change) {
				switch (ptr->balance) {
				case -1:
					ptr->balance++;
					height_change = false;
					break;
				case 0:
					ptr->balance++;
					break;
				case 1:
					if (ptr->lptr->balance == 1) LL_rotation(ptr);
					else LR_rotation(ptr);
					height_change = false;
				}
			}
		}
	}
}

void AVL_Tree::deletion(int num, node*& ptr)
{
	if (ptr == nullptr) return;
	if (num == ptr->data) {
		if (ptr->rptr == nullptr && ptr->lptr == nullptr) ptr = nullptr; // no child 
		else if (ptr->rptr == nullptr && ptr->lptr != nullptr) { // only have left child
			node* temp = ptr;
			ptr = ptr->lptr; 
			delete temp;
		}
		else if (ptr->rptr != nullptr && ptr->lptr == nullptr) { // only have right child
			node* temp = ptr;
			ptr = ptr->rptr;			
			delete temp;
		}
		else if (ptr->rptr != nullptr && ptr->lptr != nullptr) { // have two child
			node node_ptr;
			node_ptr = *find_max(ptr->lptr); // duplicate only
			int data_num = node_ptr.data;
			node* left_ptr = node_ptr.lptr;
			node* right_ptr = node_ptr.rptr;
			deletion(node_ptr.data, root_ptr);
			ptr->data = data_num;
			ptr->lptr = left_ptr;
			ptr->rptr = right_ptr;
			ptr->balance = height(ptr->lptr) - height(ptr->rptr);
		}
		return;
	}
	else if (num > ptr->data) deletion(num, ptr->rptr);
	else deletion(num, ptr->lptr);
	ptr->balance = height(ptr->lptr) - height(ptr->rptr);
	switch (ptr->balance) {
	case 2:
		if (ptr->lptr->balance == 0) R0_rotation(ptr, 'l');
		else if (ptr->lptr->balance == 1) R1_rotation(ptr, 'l');
		else R_1_rotation(ptr, 'l');
		break;
	case -2:
		if (ptr->rptr->balance == 0) R0_rotation(ptr, 'r');
		else if (ptr->rptr->balance == 1) R1_rotation(ptr, 'r');
		else R_1_rotation(ptr, 'r');
	}
}

void AVL_Tree::in_order(node* ptr, string& str)
{
	if (ptr == nullptr) return;
	in_order(ptr->lptr, str);
	str += to_string(ptr->data) + " ";
	in_order(ptr->rptr, str);
}

void AVL_Tree::LL_rotation(node*& ptr, bool from_del)
{
	node* left = ptr->lptr;
	ptr->lptr = left->rptr;
	left->rptr = ptr;
	if (ptr->is_root) {
		ptr->is_root = false;
		left->is_root = true;
		root_ptr = left;
	}
	ptr = left;
	ptr->rptr->balance = height(ptr->rptr->lptr) - height(ptr->rptr->rptr);
	ptr->lptr->balance = height(ptr->lptr->lptr) - height(ptr->lptr->rptr);
	ptr->balance = height(ptr->lptr) - height(ptr->rptr);
	if (!from_del) {
		rotation_type += "LL,";
		rotate_num++;
	}
}

void AVL_Tree::RR_rotation(node*& ptr, bool from_del)
{
	node* right = ptr->rptr;
	ptr->rptr = right->lptr;
	right->lptr = ptr;
	if (ptr->is_root) {
		ptr->is_root = false;
		right->is_root = true;
		root_ptr = right;
	}
	ptr = right;
	ptr->rptr->balance = height(ptr->rptr->lptr) - height(ptr->rptr->rptr);
	ptr->lptr->balance = height(ptr->lptr->lptr) - height(ptr->lptr->rptr);
	ptr->balance = height(ptr->lptr) - height(ptr->rptr);
	if (!from_del) {
		rotation_type += "RR,";
		rotate_num++;
	}
}

void AVL_Tree::RL_rotation(node*& ptr, bool from_del)
{
	node* top = ptr->rptr->lptr;
	ptr->rptr->lptr = top->rptr;
	top->rptr = ptr->rptr;
	ptr->rptr = top->lptr;
	top->lptr = ptr;
	if (ptr->is_root) {
		ptr->is_root = false;
		top->is_root = true;
		root_ptr = top;
	}
	ptr = top;
	ptr->rptr->balance = height(ptr->rptr->lptr) - height(ptr->rptr->rptr);
	ptr->lptr->balance = height(ptr->lptr->lptr) - height(ptr->lptr->rptr);
	ptr->balance = height(ptr->lptr) - height(ptr->rptr);
	if (!from_del) {
		rotation_type += "RL,";
		rotate_num++;
	}
}

void AVL_Tree::LR_rotation(node*& ptr, bool from_del)
{
	node* top = ptr->lptr->rptr;
	ptr->lptr->rptr = top->lptr;
	top->lptr = ptr->lptr;
	ptr->lptr = top->rptr;
	top->rptr = ptr;
	if (ptr->is_root) {
		ptr->is_root = false;
		top->is_root = true;
		root_ptr = top;
	}
	ptr = top;
	ptr->rptr->balance = height(ptr->rptr->lptr) - height(ptr->rptr->rptr);
	ptr->lptr->balance = height(ptr->lptr->lptr) - height(ptr->lptr->rptr);
	ptr->balance = height(ptr->lptr) - height(ptr->rptr);
	if (!from_del) {
		rotation_type += "LR,";
		rotate_num++;
	}
}

void AVL_Tree::R0_rotation(node*& ptr, char type)
{
	switch (type) {
	case 'l': LL_rotation(ptr, true); break;
	case 'r': RR_rotation(ptr, true);
	}
	rotate_num++;
	rotation_type += "R0,";
}

void AVL_Tree::R1_rotation(node*& ptr, char type)
{
	switch (type) {
	case 'l': LL_rotation(ptr, true); break;
	case 'r': RL_rotation(ptr, true);
	}
	rotate_num++;
	rotation_type += "R1,";
}

void AVL_Tree::R_1_rotation(node*& ptr, char type)
{
	switch (type) {
	case 'l': LR_rotation(ptr, true); break;
	case 'r': RR_rotation(ptr, true);
	}
	rotate_num++;
	rotation_type += "R-1,";
}

node* AVL_Tree::find(int num, node* ptr)
{
	if (ptr == nullptr) return nullptr;
	if (num == ptr->data) return ptr;
	if (num > ptr->data) return find(num, ptr->rptr);
	else return find(num, ptr->lptr);
}

node* AVL_Tree::find_max(node* ptr)
{
	if (ptr->rptr == nullptr) return ptr;
	return find_max(ptr->rptr);
}

int AVL_Tree::height(node* ptr)
{
	if (ptr == nullptr) return 0; // notice this line
	if (ptr->lptr == nullptr && ptr->rptr == nullptr) return 1;
	int left = height(ptr->lptr), right = height(ptr->rptr);
	if (left > right) return left + 1;
	else return right + 1;
}
