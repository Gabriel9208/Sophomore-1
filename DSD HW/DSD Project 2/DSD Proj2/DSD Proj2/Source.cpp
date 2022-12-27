#include<iostream>
#include<fstream>
#include<math.h>
#include<cmath>
#include<sstream>
#include<vector>
#include<sstream>
using namespace std;

typedef struct {
	char name;
	vector<int> num;
	string literal;
	int one_num;
	int dash_num;
	bool dont_care;
}mint;

typedef struct {
	string literal;
	bool dont_care;
	int one_num;
	int dash_num;
}info;

typedef struct {
	string data;
	int left;
	int right;
}node;

typedef struct {
	int num;
	int amount;
}min_count;

class Tree {
	vector<node> tree; // index start from 1
public:
	Tree() { node n; n.data = ""; n.left = -1; n.right = -1; tree.push_back(n); }
	Tree(int literal_num);
	~Tree() {}
	node& operator [](int a);
	int size() { return tree.size(); }
};

bool operator==(const mint& a, const mint& b);
void read_file(ifstream& input, ofstream& output, vector<info>& initial_input);
void fill_in_minterm(vector<info>& initial_input, vector<mint>& minterm);
void fill_in_dotCar(vector<mint>& minterm, vector<mint>& dotCar);
void match(vector<mint>& minterm);
void find_essential(vector<mint>& minterm, vector<string>& result, vector<int>& to_cover, vector<mint>& dotCar);
void petrick(vector<int>& to_cover, vector<mint>& minterm, ofstream& output, vector<string>& result);
void final_print(vector<string>& result, ofstream& output);

int main(int argc, char**argv) {
	ifstream input;
	ofstream output;
	input.open(argv[argc - 2]);
	output.open(argv[argc - 1]);
	vector<info> initial_input;
	vector<mint> minterm; 
	vector<mint> dotCar;
	vector<string> result;
	vector<int> to_cover;
	read_file(input, output, initial_input);
	fill_in_minterm(initial_input, minterm);
	fill_in_dotCar(minterm, dotCar);
	match(minterm);
	find_essential(minterm, result, to_cover, dotCar);
	petrick(to_cover, minterm, output, result);
	final_print(result, output);
}

bool operator==(const mint& a, const mint& b)
{
	if (a.literal == b.literal) return a.num == b.num;
	return false;
}

void read_file(ifstream& input, ofstream& output, vector<info>& initial_input) {
	string fileName;
	if (input.is_open()) {
		string temp = "";
		int inputAmount = 0, inpAmo = 0, total = 0;
		while (input >> temp) {
			// specify the sizes of tree and varName
			if (temp == ".i") {
				output << temp << " ";
				input >> inputAmount;
				output << inputAmount << endl;
				inpAmo = inputAmount;
				total = pow(2, inpAmo) + 1;
			}
			else if (temp == ".o") {
				output << temp << " "; input >> temp; output << temp << endl;
			}
			else if (temp == ".ilb") {
				output << temp << " ";
				while (inputAmount--) {
					input >> temp; output << temp;
					if (inputAmount != 0) output << " ";
				}
				output << endl;
			}
			else if (temp == ".ob") {
				output << temp << " "; input >> temp; output << "f" << endl;
			}
			else if (temp == ".p") {
				output << temp << " "; input >> temp;
				int max = stoi(temp);
				for (int i = 0; i < max; i++) {
					info t;
					input >> t.literal; t.dash_num = 0; t.one_num = 0;
					input >> temp;
					if (temp == "-") t.dont_care = true;
					else t.dont_care = false;
					for (int i = 0; i < t.literal.size(); i++) {
						if (t.literal[i] == '1') t.one_num++;
						if (t.literal[i] == '-') t.dash_num++;
					}
					initial_input.push_back(t);
				}
			}
			else if (temp == ".e") break;
		}
	}
	else {
		cout << "The file isn't opend correctly.";
	}
}

void fill_in_minterm(vector<info>& initial_input, vector<mint>& minterm) {
	// all minterm we have including don't care
	for (int i = 0; i < initial_input.size(); i++) {
		if (initial_input[i].dash_num != 0) {
			int dash = initial_input[i].dash_num;
			Tree tree(dash + 1);
			tree[1].data = initial_input[i].literal;
			//fill in tree
			for (int j = 1; j < pow(2, dash); j++) {
				for (int l = 0; l < tree[j].data.size(); l++) {
					//tree[tree[j].left].data
					if (tree[j].data[l] == '-') {
						tree[j].data[l] = '0';
						tree[tree[j].left].data = tree[j].data;
						tree[j].data[l] = '1';
						tree[tree[j].right].data = tree[j].data;
						break;
					}
				}
			}
			// get literal & num from tree
			for (int j = pow(2, dash); j < pow(2, dash + 1); j++) {
				mint t; t.literal = tree[j].data; t.dash_num = 0; t.one_num = 0;
				if (initial_input[i].dont_care) t.dont_care = true;
				else t.dont_care = false;
				int sum = 0;
				for (int k = 0; k < tree[j].data.size(); k++) {
					if (tree[j].data[k] == '1') t.one_num++;
					sum += (tree[j].data[k] - '0') * pow(2, tree[j].data.size() - 1 - k);
				}
				t.num.push_back(sum);
				bool already_have = false;
				for (int m = 0; m < minterm.size(); m++) {
					if (minterm[m] == t) {
						already_have = true;
						break;
					}
				}
				if (!already_have) minterm.push_back(t);
			}
		}
		else {
			mint t; t.literal = initial_input[i].literal; t.dash_num = 0; t.one_num = 0;
			if (initial_input[i].dont_care) t.dont_care = true;
			else t.dont_care = false;
			int sum = 0, max = initial_input[i].literal.size();
			for (int k = 0; k < max; k++) {
				if (initial_input[i].literal[k] == '1') t.one_num++;
				sum += (initial_input[i].literal[k] - '0') * pow(2, max - 1 - k);
			}
			t.num.push_back(sum);
			bool already_have = false;
			for (int m = 0; m < minterm.size(); m++) {
				if (minterm[m] == t) {
					already_have = true;
					break;
				}
			}
			if (!already_have) minterm.push_back(t);
		}
	}
}

void fill_in_dotCar(vector<mint>& minterm, vector<mint>& dotCar) {
	for (int i = 0; i < minterm.size(); i++) {
		if (minterm[i].dont_care) dotCar.push_back(minterm[i]);
	}
}

void match(vector<mint>& minterm){
	int time = minterm[0].literal.size();
	while (time > 0) {
		vector<mint> newM;
		vector<int> matched_index;
		for (int i = 0; i < minterm.size() - 1; i++) {
			bool not_match = true;
			for (int j = i + 1; j < minterm.size(); j++) {
				if (minterm[i].dash_num != minterm[j].dash_num || abs(minterm[i].one_num - minterm[j].one_num) != 1) 
					continue; // group difference > 1 & different dash amount is not allowed
				int different = 0, diff_index = 0;
				for (int k = 0; k < minterm[i].literal.size(); k++) {
					if (minterm[i].literal[k] != minterm[j].literal[k]) {
						if (minterm[i].literal[k] != '-') different++;
						else if (minterm[j].literal[k] != '-') {
							different += 2;
							break;
						}
						diff_index = k;
					}
				}
				if (different != 1) continue; // only tolerate one difference
				else {
					mint toPush; toPush.literal = minterm[i].literal; toPush.one_num = 0; toPush.dash_num = 0; 
					toPush.literal[diff_index] = '-';
					// fill in num
					for (int k = 0; k < minterm[i].num.size(); k++) {
						toPush.num.push_back(minterm[i].num[k]);
					}
					for (int k = 0; k < minterm[j].num.size(); k++) {
						toPush.num.push_back(minterm[j].num[k]);
					}
					for (int k = 0; k < toPush.literal.size(); k++) {
						if (toPush.literal[k] == '1') toPush.one_num++;
						if (toPush.literal[k] == '-') toPush.dash_num++;
					}
					newM.push_back(toPush);
					not_match = false;
					for (int b = 0; b < 2; b++) { // do two times
						int ind; 
						bool find = false;
						if (b == 0) ind = i; // first loop
						else ind = j; // second loop
						for (int a = 0; a < matched_index.size(); a++) {
							if (ind == matched_index[a]) {
								find = true;
								break;
							}
						}
						if (!find) matched_index.push_back(ind);
					}
				}
			}
			if (not_match) {
				bool find = false;
				for (int a = 0; a < matched_index.size(); a++) {
					if (matched_index[a] == i) {
						find = true;
						break;
					}
				}
				if(!find) newM.push_back(minterm[i]);
			}
		}
		bool push_last = true;
		for (int i = 0; i < matched_index.size(); i++) {
			if (matched_index[i] == minterm.size() - 1) push_last = false; break;
		}
		if (push_last) newM.push_back(minterm[minterm.size() - 1]);
		minterm = newM;
		// remove duplicate
		for (int i = 0; i < minterm.size() - 1; i++) {
			for (int j = i + 1; j < minterm.size(); j++) {
				if (minterm[i].literal == minterm[j].literal) {
					minterm.erase(minterm.begin() + j);
					j--;
				}
			}
		}
		time--;
	}
	for (int i = 0; i < minterm.size() - 1; i++) {
		for (int j = i + 1; j < minterm.size(); j++) {
			if (minterm[i].literal == minterm[j].literal) {
				minterm.erase(minterm.begin() + j);
				j--;
			}
		}
	}
}

void find_essential(vector<mint>& minterm, vector<string>& result, vector<int>& to_cover, vector<mint>& dotCar){
	int size = pow(2, minterm[0].literal.size()) - 1;
	int* essential_num = new int[size];
	for (int i = 0; i < size; i++) essential_num[i] = 0; // initiallize essential_num
	for (int i = 0; i < minterm.size(); i++) {
		for (int j = 0; j < minterm[i].num.size(); j++) {
			essential_num[minterm[i].num[j]]++;
		}
	}
	vector<int> essential;
	for (int i = 0; i < size; i++) {
		if (essential_num[i] == 1) essential.push_back(i);
		if (essential_num[i] != 0 && essential_num[i] != 1) to_cover.push_back(i);
	}
	// remove don't care from to_cover
	for (int i = 0; i < dotCar.size(); i++) {
		for (int j = 0; j < dotCar[i].num.size(); j++) {
			for (int k = 0; k < to_cover.size(); k++) {
				if (dotCar[i].num[j] == to_cover[k]) {
					to_cover.erase(to_cover.begin() + k);
					k--;
				}
			}
		}
	}
	// remove don't care from essential
	for (int i = 0; i < dotCar.size(); i++) {
		for (int j = 0; j < dotCar[i].num.size(); j++) {
			for (int k = 0; k < essential.size(); k++) {
				if (dotCar[i].num[j] == essential[k]) {
					essential.erase(essential.begin() + k);
					k--;
				}
			}
		}
	}
	for (int i = 0; i < essential.size(); i++) {
		for (int j = 0; j < minterm.size(); j++) {
			bool skip = false;
			for (int k = 0; k < minterm[j].num.size(); k++) {
				if (essential[i] == minterm[j].num[k]) {
					for (int l = 0; l < minterm[j].num.size();l++) {
						for (int m = 0; m < to_cover.size(); m++) {
							if (to_cover[m] == minterm[j].num[l]) {
								to_cover.erase(to_cover.begin() + m);
								m--;
							}
						}
					}
					result.push_back(minterm[j].literal);
					minterm.erase(minterm.begin() + j);
					skip = true;
					break;
				}
			}
			if (skip) break;
		}
	}
}

void petrick(vector<int>& to_cover, vector<mint>& minterm, ofstream& output, vector<string>& result) {
	if (minterm.size() != 0 && to_cover.size() != 0) {
		for (int i = 0; i < minterm.size(); i++) minterm[i].name = 'a' + i;
		vector<string> order;
		for (int k = 0; k < to_cover.size(); k++) {
			string str = "";
			for (int i = 0; i < minterm.size(); i++) {
				for (int j = 0; j < minterm[i].num.size(); j++) {
					if (to_cover[k] == minterm[i].num[j]) {
						str += string(1, minterm[i].name) + " + ";
						break;
					}
				}
			}
			order.push_back(str);
		}
		for (int i = 0; i < order.size(); i++) order[i].erase(order[i].end() - 3, order[i].end() - 1);
		vector<string> time = order;
		while (time.size() != 1) {
			vector<string> temp;
			string str = "", input1 = "", input2 = "";
			stringstream ss_first(time[0]);
			while (ss_first >> input1) {
				stringstream ss_second(time[1]);
				while (ss_second >> input2) {
					str += input1 + input2 + " + ";
					ss_second >> input2;
				}
				ss_first >> input1;
			}
			temp.push_back(str);
			temp[0].erase(temp[0].end() - 3, temp[0].end() - 1);
			for (int k = 2; k < time.size(); k++) {
				temp.push_back(time[k]);
			}
			time = temp;
		}
		stringstream ss(time[0]);
		string str = "", final = "";
		while (ss >> str) {
			for (int i = 0; i < str.size() - 1; i++) {
				for (int j = i + 1; j < str.size(); j++) {
					if (str[i] == str[j]) { str.erase(str.begin() + j); j--; }
				}
			}
			final += str + " + ";
			ss >> str;
		}
		ss.clear(); ss.str(""); time.clear();
		ss.str(final);
		while (ss >> str) {
			time.push_back(str);
			ss >> str;
		}
		// order every terms in alphabetical order
		for (int i = 0; i < time.size() - 1; i++) {
			for (int k = 0; k < time[i].size(); k++) {
				for (int j = 0; j < time[i].size() - 1; j++) {
					if (time[i][j] > time[i][j + 1]) {
						char temp = time[i][j];
						time[i][j] = time[i][j + 1];
						time[i][j + 1] = temp;
					}
				}
			}
		}

		// clear duplicate
		for (int i = 0; i < time.size() - 1; i++) {
			for (int j = i + 1; j < time.size(); j++) {
				if (time[i] == time[j]) {
					time.erase(time.begin() + j);
					j--;
				}
			}
		}
		int index = 0, size = 1000;
		for (int i = 0; i < time.size(); i++) {
			if (time[i].size() < size) {
				index = i;
				size = time[i].size();
			}
		}

		for (int i = 0; i < time[index].size(); i++) { // error maybe no time 0
			for (int j = 0; j < minterm.size(); j++) {
				if (time[index][i] == minterm[j].name) {
					result.push_back(minterm[j].literal);
					break;
				}
			}
		}
	}
}

void final_print(vector<string>& result, ofstream& output)
{
	output << result.size() << endl;
	for (int i = 0; i < result.size(); i++) {
		output << result[i] << " 1" << endl;
	}
	output << ".e";
}

Tree::Tree(int literal_num)
{
	node n; n.data = ""; n.left = -1; n.right = -1;
	tree.push_back(n);
	for (int i = 1; i < pow(2, literal_num - 1); i++) {
		n.data = ""; n.left = i * 2; n.right = i * 2 + 1;
		tree.push_back(n);
	}
	for (int i = pow(2, literal_num - 1); i < pow(2, literal_num); i++) {
		n.data = ""; n.left = -1; n.right = -1;
		tree.push_back(n);
	}
}

node& Tree::operator[](int a)
{
	node t;
	t.data = ""; t.left = -1; t.right = -1;
	while (a >= tree.size()) tree.push_back(t);
	return tree[a];
}

/*
#include<iostream>
#include<fstream>
#include<math.h>
#include<cmath>
#include<sstream>
#include<vector>
#include<sstream>
using namespace std;

typedef struct {
	char name;
	vector<int> num;
	string literal;
	int one_num;
	int dash_num;
	bool dont_care;
}mint;

typedef struct {
	string literal;
	bool dont_care;
	int one_num;
	int dash_num;
}info;

typedef struct {
	string data;
	int left;
	int right;
}node;

typedef struct {
	int num;
	int amount;
}min_count;

class Tree {
	vector<node> tree; // index start from 1
public:
	Tree() { node n; n.data = ""; n.left = -1; n.right = -1; tree.push_back(n); }
	Tree(int literal_num);
	~Tree() {}
	node& operator [](int a);
	int size() { return tree.size(); }
};

bool operator==(const mint& a, const mint& b);
void read_file(ifstream& input, ofstream& output, vector<info>& initial_input);
void fill_in_minterm(vector<info>& initial_input, vector<mint>& minterm);
void fill_in_dotCar(vector<mint>& minterm, vector<mint>& dotCar);
void match(vector<mint>& minterm);
void find_essential(vector<mint>& minterm, vector<string>& result, vector<int>& to_cover, vector<mint>& dotCar);
void petrick(vector<int>& to_cover, vector<mint>& minterm, ofstream& output, vector<string>& result);
void final_print(vector<string>& result, ofstream& output);

int main() {
	ifstream input;
	ofstream output;
	string str;
	cin >> str; input.open(str);
	cin >> str; output.open(str);
	vector<info> initial_input;
	vector<mint> minterm;
	vector<mint> dotCar;
	vector<string> result;
	vector<int> to_cover;
	read_file(input, output, initial_input);
	fill_in_minterm(initial_input, minterm);
	fill_in_dotCar(minterm, dotCar);
	match(minterm);
	find_essential(minterm, result, to_cover, dotCar);
	petrick(to_cover, minterm, output, result);
	final_print(result, output);
}

bool operator==(const mint& a, const mint& b)
{
	if (a.literal == b.literal) return a.num == b.num;
	return false;
}

void read_file(ifstream& input, ofstream& output, vector<info>& initial_input) {
	string fileName;
	if (input.is_open()) {
		string temp = "";
		int inputAmount = 0, inpAmo = 0, total = 0;
		while (input >> temp) {
			// specify the sizes of tree and varName
			if (temp == ".i") {
				output << temp << " ";
				input >> inputAmount;
				output << inputAmount << endl;
				inpAmo = inputAmount;
				total = pow(2, inpAmo) + 1;
			}
			else if (temp == ".o") {
				output << temp << " "; input >> temp; output << temp << endl;
			}
			else if (temp == ".ilb") {
				output << temp << " ";
				while (inputAmount--) {
					input >> temp; output << temp;
					if (inputAmount != 0) output << " ";
				}
				output << endl;
			}
			else if (temp == ".ob") {
				output << temp << " "; input >> temp; output << "f" << endl;
			}
			else if (temp == ".p") {
				output << temp << " "; input >> temp;
				int max = stoi(temp);
				for (int i = 0; i < max; i++) {
					info t;
					input >> t.literal; t.dash_num = 0; t.one_num = 0;
					input >> temp;
					if (temp == "-") t.dont_care = true;
					else t.dont_care = false;
					for (int i = 0; i < t.literal.size(); i++) {
						if (t.literal[i] == '1') t.one_num++;
						if (t.literal[i] == '-') t.dash_num++;
					}
					initial_input.push_back(t);
				}
			}
			else if (temp == ".e") break;
		}
	}
	else {
		cout << "The file isn't opend correctly.";
	}
}

void fill_in_minterm(vector<info>& initial_input, vector<mint>& minterm) {
	// all minterm we have including don't care
	for (int i = 0; i < initial_input.size(); i++) {
		if (initial_input[i].dash_num != 0) {
			int dash = initial_input[i].dash_num;
			Tree tree(dash + 1);
			tree[1].data = initial_input[i].literal;
			//fill in tree
			for (int j = 1; j < pow(2, dash); j++) {
				for (int l = 0; l < tree[j].data.size(); l++) {
					//tree[tree[j].left].data
					if (tree[j].data[l] == '-') {
						tree[j].data[l] = '0';
						tree[tree[j].left].data = tree[j].data;
						tree[j].data[l] = '1';
						tree[tree[j].right].data = tree[j].data;
						break;
					}
				}
			}
			// get literal & num from tree
			for (int j = pow(2, dash); j < pow(2, dash + 1); j++) {
				mint t; t.literal = tree[j].data; t.dash_num = 0; t.one_num = 0;
				if (initial_input[i].dont_care) t.dont_care = true;
				else t.dont_care = false;
				int sum = 0;
				for (int k = 0; k < tree[j].data.size(); k++) {
					if (tree[j].data[k] == '1') t.one_num++;
					sum += (tree[j].data[k] - '0') * pow(2, tree[j].data.size() - 1 - k);
				}
				t.num.push_back(sum);
				bool already_have = false;
				for (int m = 0; m < minterm.size(); m++) {
					if (minterm[m] == t) {
						already_have = true;
						break;
					}
				}
				if (!already_have) minterm.push_back(t);
			}
		}
		else {
			mint t; t.literal = initial_input[i].literal; t.dash_num = 0; t.one_num = 0;
			if (initial_input[i].dont_care) t.dont_care = true;
			else t.dont_care = false;
			int sum = 0, max = initial_input[i].literal.size();
			for (int k = 0; k < max; k++) {
				if (initial_input[i].literal[k] == '1') t.one_num++;
				sum += (initial_input[i].literal[k] - '0') * pow(2, max - 1 - k);
			}
			t.num.push_back(sum);
			bool already_have = false;
			for (int m = 0; m < minterm.size(); m++) {
				if (minterm[m] == t) {
					already_have = true;
					break;
				}
			}
			if (!already_have) minterm.push_back(t);
		}
	}
}

void fill_in_dotCar(vector<mint>& minterm, vector<mint>& dotCar) {
	for (int i = 0; i < minterm.size(); i++) {
		if (minterm[i].dont_care) dotCar.push_back(minterm[i]);
	}
}

void match(vector<mint>& minterm){
	int time = minterm[0].literal.size();
	while (time > 0) {
		vector<mint> newM;
		vector<int> matched_index;
		for (int i = 0; i < minterm.size() - 1; i++) {
			bool not_match = true;
			for (int j = i + 1; j < minterm.size(); j++) {
				if (minterm[i].dash_num != minterm[j].dash_num || abs(minterm[i].one_num - minterm[j].one_num) != 1)
					continue; // group difference > 1 & different dash amount is not allowed
				int different = 0, diff_index = 0;
				for (int k = 0; k < minterm[i].literal.size(); k++) {
					if (minterm[i].literal[k] != minterm[j].literal[k]) {
						if (minterm[i].literal[k] != '-') different++;
						else if (minterm[j].literal[k] != '-') {
							different += 2;
							break;
						}
						diff_index = k;
					}
				}
				if (different != 1) continue; // only tolerate one difference
				else {
					mint toPush; toPush.literal = minterm[i].literal; toPush.one_num = 0; toPush.dash_num = 0;
					toPush.literal[diff_index] = '-';
					// fill in num
					for (int k = 0; k < minterm[i].num.size(); k++) {
						toPush.num.push_back(minterm[i].num[k]);
					}
					for (int k = 0; k < minterm[j].num.size(); k++) {
						toPush.num.push_back(minterm[j].num[k]);
					}
					for (int k = 0; k < toPush.literal.size(); k++) {
						if (toPush.literal[k] == '1') toPush.one_num++;
						if (toPush.literal[k] == '-') toPush.dash_num++;
					}
					newM.push_back(toPush);
					not_match = false;
					for (int b = 0; b < 2; b++) { // do two times
						int ind;
						bool find = false;
						if (b == 0) ind = i; // first loop
						else ind = j; // second loop
						for (int a = 0; a < matched_index.size(); a++) {
							if (ind == matched_index[a]) {
								find = true;
								break;
							}
						}
						if (!find) matched_index.push_back(ind);
					}
				}
			}
			if (not_match) {
				bool find = false;
				for (int a = 0; a < matched_index.size(); a++) {
					if (matched_index[a] == i) {
						find = true;
						break;
					}
				}
				if(!find) newM.push_back(minterm[i]);
			}
		}
		bool push_last = true;
		for (int i = 0; i < matched_index.size(); i++) {
			if (matched_index[i] == minterm.size() - 1) push_last = false; break;
		}
		if (push_last) newM.push_back(minterm[minterm.size() - 1]);
		minterm = newM;
		// remove duplicate
		for (int i = 0; i < minterm.size() - 1; i++) {
			for (int j = i + 1; j < minterm.size(); j++) {
				if (minterm[i].literal == minterm[j].literal) {
					minterm.erase(minterm.begin() + j);
					j--;
				}
			}
		}
		time--;
	}
	for (int i = 0; i < minterm.size() - 1; i++) {
		for (int j = i + 1; j < minterm.size(); j++) {
			if (minterm[i].literal == minterm[j].literal) {
				minterm.erase(minterm.begin() + j);
				j--;
			}
		}
	}
}

void find_essential(vector<mint>& minterm, vector<string>& result, vector<int>& to_cover, vector<mint>& dotCar){
	int size = pow(2, minterm[0].literal.size()) - 1;
	int* essential_num = new int[size];
	for (int i = 0; i < size; i++) essential_num[i] = 0; // initiallize essential_num
	for (int i = 0; i < minterm.size(); i++) {
		for (int j = 0; j < minterm[i].num.size(); j++) {
			essential_num[minterm[i].num[j]]++;
		}
	}
	vector<int> essential;
	for (int i = 0; i < size; i++) {
		if (essential_num[i] == 1) essential.push_back(i);
		if (essential_num[i] != 0 && essential_num[i] != 1) to_cover.push_back(i);
	}
	// remove don't care from to_cover
	for (int i = 0; i < dotCar.size(); i++) {
		for (int j = 0; j < dotCar[i].num.size(); j++) {
			for (int k = 0; k < to_cover.size(); k++) {
				if (dotCar[i].num[j] == to_cover[k]) {
					to_cover.erase(to_cover.begin() + k);
					k--;
				}
			}
		}
	}
	// remove don't care from essential
	for (int i = 0; i < dotCar.size(); i++) {
		for (int j = 0; j < dotCar[i].num.size(); j++) {
			for (int k = 0; k < essential.size(); k++) {
				if (dotCar[i].num[j] == essential[k]) {
					essential.erase(essential.begin() + k);
					k--;
				}
			}
		}
	}
	for (int i = 0; i < essential.size(); i++) {
		for (int j = 0; j < minterm.size(); j++) {
			bool skip = false;
			for (int k = 0; k < minterm[j].num.size(); k++) {
				if (essential[i] == minterm[j].num[k]) {
					for (int l = 0; l < minterm[j].num.size();l++) {
						for (int m = 0; m < to_cover.size(); m++) {
							if (to_cover[m] == minterm[j].num[l]) {
								to_cover.erase(to_cover.begin() + m);
								m--;
							}
						}
					}
					result.push_back(minterm[j].literal);
					minterm.erase(minterm.begin() + j);
					skip = true;
					break;
				}
			}
			if (skip) break;
		}
	}
}

void petrick(vector<int>& to_cover, vector<mint>& minterm, ofstream& output, vector<string>& result) {
	if (minterm.size() != 0 && to_cover.size() != 0) {
		for (int i = 0; i < minterm.size(); i++) minterm[i].name = 'a' + i;
		vector<string> order;
		for (int k = 0; k < to_cover.size(); k++) {
			string str = "";
			for (int i = 0; i < minterm.size(); i++) {
				for (int j = 0; j < minterm[i].num.size(); j++) {
					if (to_cover[k] == minterm[i].num[j]) {
						str += string(1, minterm[i].name) + " + ";
						break;
					}
				}
			}
			order.push_back(str);
		}
		for (int i = 0; i < order.size(); i++) order[i].erase(order[i].end() - 3, order[i].end() - 1);
		vector<string> time = order;
		while (time.size() != 1) {
			vector<string> temp;
			string str = "", input1 = "", input2 = "";
			stringstream ss_first(time[0]);
			while (ss_first >> input1) {
				stringstream ss_second(time[1]);
				while (ss_second >> input2) {
					str += input1 + input2 + " + ";
					ss_second >> input2;
				}
				ss_first >> input1;
			}
			temp.push_back(str);
			temp[0].erase(temp[0].end() - 3, temp[0].end() - 1);
			for (int k = 2; k < time.size(); k++) {
				temp.push_back(time[k]);
			}
			time = temp;
		}
		stringstream ss(time[0]);
		string str = "", final = "";
		while (ss >> str) {
			for (int i = 0; i < str.size() - 1; i++) {
				for (int j = i + 1; j < str.size(); j++) {
					if (str[i] == str[j]) { str.erase(str.begin() + j); j--; }
				}
			}
			final += str + " + ";
			ss >> str;
		}
		ss.clear(); ss.str(""); time.clear();
		ss.str(final);
		while (ss >> str) {
			time.push_back(str);
			ss >> str;
		}
		// order every terms in alphabetical order
		for (int i = 0; i < time.size() - 1; i++) {
			for (int k = 0; k < time[i].size(); k++) {
				for (int j = 0; j < time[i].size() - 1; j++) {
					if (time[i][j] > time[i][j + 1]) {
						char temp = time[i][j];
						time[i][j] = time[i][j + 1];
						time[i][j + 1] = temp;
					}
				}
			}
		}

		// clear duplicate
		for (int i = 0; i < time.size() - 1; i++) {
			for (int j = i + 1; j < time.size(); j++) {
				if (time[i] == time[j]) {
					time.erase(time.begin() + j);
					j--;
				}
			}
		}
		int index = 0, size = 1000;
		for (int i = 0; i < time.size(); i++) {
			if (time[i].size() < size) {
				index = i;
				size = time[i].size();
			}
		}

		for (int i = 0; i < time[index].size(); i++) { // error maybe no time 0
			for (int j = 0; j < minterm.size(); j++) {
				if (time[index][i] == minterm[j].name) {
					result.push_back(minterm[j].literal);
					break;
				}
			}
		}
	}
}

void final_print(vector<string>& result, ofstream& output)
{
	output << result.size() << endl;
	for (int i = 0; i < result.size(); i++) {
		output << result[i] << " 1" << endl;
	}
	output << ".e";
}

Tree::Tree(int literal_num)
{
	node n; n.data = ""; n.left = -1; n.right = -1;
	tree.push_back(n);
	for (int i = 1; i < pow(2, literal_num - 1); i++) {
		n.data = ""; n.left = i * 2; n.right = i * 2 + 1;
		tree.push_back(n);
	}
	for (int i = pow(2, literal_num - 1); i < pow(2, literal_num); i++) {
		n.data = ""; n.left = -1; n.right = -1;
		tree.push_back(n);
	}
}

node& Tree::operator[](int a)
{
	node t;
	t.data = ""; t.left = -1; t.right = -1;
	while (a >= tree.size()) tree.push_back(t);
	return tree[a];
}
*/