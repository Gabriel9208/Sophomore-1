#include<iostream>
#include<fstream>
#include<math.h>
#include<sstream>
using namespace std;

typedef struct leaf{
	string variable;
	int Else = -1;
	int Then = -1;
	int father = -1;
}leaf;

void fillRec(leaf* tree, string func, int level, string* varName);
int cal(string func, int* value, string* varName, int level);
void delSameEdge(leaf* tree, int level);
void modifySame(leaf* tree, int level, int delIndex, int toIndex);
void mergeSameNode(leaf* tree, int level);
void printDot(leaf* tree, int level, ofstream& out);

int main() {
	ifstream input;
	ofstream output;
	string fileName;
	cin >> fileName;
	input.open(fileName);
	cin >> fileName;
	output.open(fileName);
	if (input.is_open()) {
		string temp = "";
		string function = "";
		int inputAmount = 0;
		int inpAmo = 0;
		leaf* tree = nullptr;
		string* varName = nullptr;
		int total = 0;
		while (input >> temp){
			// specify the sizes of tree and varName
			if (temp == ".i") {
				input >> inputAmount;
				inpAmo = inputAmount;
				total = pow(2, inpAmo) + 1;
				tree = new leaf[total];
				varName = new string[inpAmo + 1];

			}
			else if (temp == ".o") input >> temp;
			else if (temp == ".ilb") {
				int tp = 1;
				// get the variables' names
				while (inputAmount--) {
					input >> varName[tp];
					tp++;
				}
				// fill in the variable name of each node which has name
				for (int i = 1; i < total - 1; i++) {
					int temp = 0;
					while (1) {
						if (i < pow(2, temp)) {
							tree[i].variable = varName[temp];
							temp = 0;
							break;
						}
						temp++;
					}
				}
				// fill in Else and Then for each nodes which is not end in 0 or 1
				// no problem
				int index = 1;
				for (int i = 2; index < pow(2, inpAmo - 1); i++, index++) {
					tree[index].Else = i;
					tree[i].father = index;
					i++;
					tree[index].Then = i;
					tree[i].father = index;
				}
				/*
				for (int i = 1; i < pow(2, inpAmo - 1); i++) {
					cout << i << "\n" << tree[i].Else << endl << tree[i].Then << endl << endl;
				}
				*/
			}
			else if (temp == ".ob") input >> temp;
			else if (temp == ".p") {
				input >> inputAmount;
				while (inputAmount--) {
					input >> temp;
					for (int i = 0; i < temp.size(); i++) {
						switch (temp[i]) {
						case '1':
							function += varName[i + 1];
							break;
						case '0':
							function += varName[i + 1] + "\'";
							break;
						}
					}
					if (inputAmount != 0) {
						function += " + ";
						input >> temp;
					}
				}
			}
			else if (temp == ".e") break;
		}
		fillRec(tree, function, inpAmo, varName);
		delSameEdge(tree, inpAmo);
		mergeSameNode(tree, inpAmo);
		delSameEdge(tree, inpAmo);
		mergeSameNode(tree, inpAmo);
		printDot(tree, inpAmo, output);
	}
	else {
		cout << "The file isn't opend correctly.";
	}
}

//level = variable amount
void fillRec(leaf* tree, string func, int level, string* varName) {
	int index = 1;
	for (int i = 0; i < level - 1; i++) index += pow(2, i);
	int* value = new int[level];
	for (index; index < pow(2, level); index++) {
		leaf temp = tree[index];
		int subIndex = index;
		for (int i = 0; i < level - 1; i++) {
			if (tree[temp.father].Else == subIndex) value[level - 2 - i] = 0;
			else if (tree[temp.father].Then == subIndex) value[level - 2 - i] = 1;
			subIndex = temp.father;
			temp = tree[temp.father];
		}
		value[level - 1] = 0;
		tree[index].Else = cal(func, value, varName, level);
		value[level - 1] = 1;
		tree[index].Then = cal(func, value, varName, level);
		//cout << index << endl << tree[index].Else << endl << tree[index].Then<<endl<<endl;
	}
	/*
	for (int i = 1; i < pow(2, level); i++) {
		cout << i << "\n" << tree[i].Else << endl << tree[i].Then << endl << endl;
	}
	*/
}

//variable name con only be a character
int cal(string func, int* value, string* varName, int level) {
	stringstream ss(func); // ab + c
	string cur;
	int litSum = 1;
	int bigSum = 0;
	while (ss >> cur) {
		if (cur == "+") continue;
		for (int i = 1; i < cur.size(); i += 2)  cur.insert(i, " ");
		for (int i = 0; i < cur.size(); i++) {
			if (cur[i] == '\'') {
				cur.insert(i - 1, "\'");
				cur.erase(i, 2);
			}
		}
		stringstream term(cur);
		while (term >> cur) {// a b + c
			if (cur.size() == 1) {
				for (int j = 1; j < level + 1; j++) {
					if (varName[j] == cur) {
						litSum *= value[j - 1];
						break;
					}
				}
			}
			else {
				for (int j = 1; j < level + 1; j++) {
					if (varName[j] + "\'" == cur) {
						if (value[j - 1] == 1) litSum *= 0;
						if (value[j - 1] == 0) litSum *= 1;
						break;
					}
				}
			}
		}
		bigSum += litSum;
		litSum = 1;
	}

	if (bigSum == 0) return 0;
	return pow(2,level);
}

void delSameEdge(leaf* tree, int level) {
	for (int i = 1; i < pow(2, level); i++) {
		if (tree[i].Else == tree[i].Then) {
			modifySame(tree, level, i, tree[i].Else);
			tree[i].variable = "none"; // name of the node deleted is renamed to "none"
		}
	}
	for (int i = 1; i < pow(2, level); i++) {
		if (tree[i].variable == "none") continue;
		if (tree[i].Else == tree[i].Then) {
			modifySame(tree, level, i, tree[i].Else);
			tree[i].variable = "none"; // name of the node deleted is renamed to "none"
		}
	}
}

void modifySame(leaf* tree, int level, int delIndex, int toIndex) {
	for (int i = 1; i < pow(2, level); i++) {
		if (tree[i].Else == delIndex) tree[i].Else = toIndex;
		if (tree[i].Then == delIndex) tree[i].Then = toIndex;
	}
}

void mergeSameNode(leaf* tree, int level) {
	for (int i = 0; i < pow(2, level); i++) {
		for (int j = i + 1; j < pow(2, level); j++) {
			if (tree[i].variable == tree[j].variable &&
				tree[i].Else == tree[j].Else && tree[i].Then == tree[j].Then) {
				modifySame(tree, level, j, i);
				tree[j].variable = "none"; // name of the node deleted is renamed to "none"
			}
		}
	}
	for (int i = 0; i < pow(2, level); i++) {
		if (tree[i].variable == "none") continue;
		for (int j = i + 1; j < pow(2, level); j++) {
			if (tree[j].variable == "none") continue;
			if (tree[i].variable == tree[j].variable &&
				tree[i].Else == tree[j].Else && tree[i].Then == tree[j].Then) {
				modifySame(tree, level, j, i);
				tree[j].variable = "none"; // name of the node deleted is renamed to "none"
			}
		}
	}
}

void printDot(leaf* tree, int level, ofstream& out) {
	out << "digraph ROBDD {";
	bool newRow = true;
	string curVar = "";
	for (int i = 1; i < pow(2, level); i++) {
		if (tree[i].variable == "none") continue;
		if (tree[i].variable != curVar && i != 1) { newRow = true; out << "}"; }
		if (newRow) {
			out << "\n{rank=same " << i;
			curVar = tree[i].variable;
			newRow = false;
		}
		else {
			out << " " << i;
		}
	}
	out << "}\n\n";
	out << "0 [label=0, shape=box]\n";
	for (int i = 1; i < pow(2, level); i++) {
		if (tree[i].variable != "none")
			out << i << " [label=\"" << tree[i].variable << "\"]\n";
	}
	out << pow(2, level)<<" [label=1, shape=box]\n\n";
	for (int i = 1; i < pow(2, level); i++) {
		if (tree[i].variable != "none") {
			out << i << " -> " << tree[i].Else << " [label=\"0\", style=dotted]\n";
			out << i << " -> " << tree[i].Then << " [label=\"1\", style=solid]\n";
		}
	}
	out << "}";
}
