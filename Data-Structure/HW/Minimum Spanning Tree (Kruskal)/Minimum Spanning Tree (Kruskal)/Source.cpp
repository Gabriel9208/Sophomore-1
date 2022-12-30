#include<iostream>
#include<vector>
#include<string>
#include<sstream>

using std::string;
using std::vector;

typedef struct {
	string src;
	string des;
	int weight;
}edge;

class Spanning {
	string* nodeName;
	int vertexAmount;
	int edgeAmount;
	vector<vector<int>> visited;
	vector<edge> pQueue;
	vector<edge> result;

	void addNodeName(string name);
	void orderNodeName();
	int nameIndex(string a);
	void addPQ(edge a);
	void qPop();
public:
	Spanning(int v, int e);
	void create();
	void findMS();
	void print();
};

int main() {
	int vNum = 0, eNum = 0;
	std::cin >> vNum >> eNum;
	if (vNum != 0 && eNum != 0) {
		Spanning tree(vNum, eNum);
		tree.create();
		tree.findMS();
		tree.print();
	}
}

Spanning::Spanning(int v, int e) :vertexAmount(v), edgeAmount(e)
{
	nodeName = new string[v];
	for (int i = 0; i < v; i++) {
		nodeName[i] = "";
	}
}

void Spanning::create()
{
	string src, des, weight, ss = "";
	while (std::cin >> src >> des >> weight) {
		ss += src + " " + des + " " + weight + " ";
		addNodeName(src);
		addNodeName(des);
	}
	orderNodeName();
	std::stringstream in(ss);
	edge temp;
	while (in >> temp.src >> temp.des >> temp.weight) {
		if (nameIndex(temp.src) > nameIndex(temp.des)) {
			string t = temp.src;
			temp.src = temp.des;
			temp.des = t;
		}
		addPQ(temp);
	}
	for (int i = 0; i < vertexAmount; i++) {
		vector<int> t;
		t.push_back(i);
		visited.push_back(t);
	}
}

void Spanning::addNodeName(string name)
{
	for (int i = 0; i < vertexAmount; i++) {
		if (nodeName[i] == name) return;
		if (nodeName[i] == "") {
			nodeName[i] = name;
			return;
		}
	}
}

void Spanning::orderNodeName()
{
	for (int i = 0; i < vertexAmount; i++) {
		for (int j = 0; j < vertexAmount - i - 1; j++) {
			if (nodeName[j] > nodeName[j + 1]) {
				string temp = nodeName[j];
				nodeName[j] = nodeName[j + 1];
				nodeName[j + 1] = temp;
			}
		}
	}
}

int Spanning::nameIndex(string a)
{
	for (int i = 0; i < vertexAmount; i++) {
		if (nodeName[i] == a) return i;
	}
	return -1;
}

void Spanning::addPQ(edge a)
{
	pQueue.push_back(a);
	if (pQueue.size() == 1) return;
	for (int i = pQueue.size() - 1; i > 0; i--) {
		if (pQueue[i].weight < pQueue[i - 1].weight) {
			edge t = pQueue[i];
			pQueue[i] = pQueue[i - 1];
			pQueue[i - 1] = t;
		}
		else if (pQueue[i].weight == pQueue[i - 1].weight) {
			if (nameIndex(pQueue[i].src) < nameIndex(pQueue[i - 1].src)) {
				edge t = pQueue[i];
				pQueue[i] = pQueue[i - 1];
				pQueue[i - 1] = t;
			}
			else if (nameIndex(pQueue[i].src) == nameIndex(pQueue[i - 1].src)) {
				if (nameIndex(pQueue[i].des) < nameIndex(pQueue[i - 1].des)) {
					edge t = pQueue[i];
					pQueue[i] = pQueue[i - 1];
					pQueue[i - 1] = t;
				}
				else return;
			}
			else return;
		}
		else return;
	}
}

void Spanning::findMS()
{
	for (int k = 0; k < edgeAmount; k++) {
		int toMerge = -1, toDiscard = -1;
		bool srcVis = false, desVis = false;
		for (int i = 0; i < visited.size(); i++) {
			srcVis = false; desVis = false;
			for (int j = 0; j < visited[i].size(); j++) {
				if (nameIndex(pQueue[0].src) == visited[i][j]) {
					toMerge = i;
					srcVis = true;
				}
				if (nameIndex(pQueue[0].des) == visited[i][j]) {
					toDiscard = i;
					desVis = true;
				}
			}
			if (srcVis && desVis) break;
		}
		if (!srcVis || !desVis) {
			result.push_back(pQueue[0]);
			if (toMerge != -1 && toDiscard != -1) {
				if (toMerge > toDiscard) {
					int t = toMerge;
					toMerge = toDiscard;
					toDiscard = t;
				}
				for (int j = 0; j < visited[toDiscard].size(); j++) {
					visited[toMerge].push_back(visited[toDiscard][j]);
				}
				visited.erase(visited.begin() + toDiscard);
			}
		}
		qPop();
	}
}

void Spanning::print()
{
	int sum = 0;
	for (int i = 0; i < result.size(); i++) {
		sum += result[i].weight;
	}
	std::cout << sum << std::endl;
	for (int i = 0; i < result.size(); i++) {
		std::cout << result[i].src << " " << result[i].des << " " << result[i].weight;
		if (i != result.size() - 1) std::cout << std::endl;
	}
}
void Spanning::qPop()
{
	pQueue.erase(pQueue.begin());
}