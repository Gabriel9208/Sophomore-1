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
	int** matrix;
	int weight;
	void finding(vector<edge>& all, vector<edge>& res, vector<int>& visited);
	void print(vector<edge>& res);
	void justify_result(vector<edge>& res);
public:
	Spanning(int v, int e);
	int matrixGet(int r, int c) { return matrix[r][c]; }
	void addEdge(int src, int des, int w);
	void create();
	void addNodeName(string name);
	void findMin();
	void orderNodeName();
	int nameIndex(string a);
};

int main() {
	int vNum = 0, eNum = 0;
	std::cin >> vNum >> eNum;
	if (vNum != 0 && eNum != 0) {
		Spanning tree(vNum, eNum);
		tree.create();
		tree.findMin();
	}
}

Spanning::Spanning(int v, int e) :vertexAmount(v), edgeAmount(e), weight(0)
{
	matrix = new int* [v];
	nodeName = new string[v];
	for (int i = 0; i < v; i++) {
		nodeName[i] = "";
	}
	for (int i = 0; i < v; i++) {
		matrix[i] = new int[v];
	}
	for (int i = 0; i < v; i++) {
		for (int j = 0; j < v; j++) {
			matrix[i][j] = 2147483647;
		}
	}

}

void Spanning::addEdge(int src, int des, int w)
{
	if (matrix[src][des] > w) {
		matrix[src][des] = w;
		matrix[des][src] = w;
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
	int s = -1, d = -1, w = -1;
	while (in >> src >> des >> weight) {
		for (int i = 0; i < vertexAmount; i++) {
			if (src == nodeName[i]) s = i;
			if (des == nodeName[i]) d = i;
		}
		w = stoi(weight);
		addEdge(s, d, w);
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

void Spanning::finding(vector<edge>& all, vector<edge>& res, vector<int>& vis)
{
	if (res.size() >= vertexAmount - 1) return;

	// find the smallest edge
	int index = 0;
	for (int i = 1; i < all.size(); i++) {
		if (all[i].weight < all[index].weight) index = i;
		else if (all[i].weight == all[index].weight) 
			if (nameIndex(all[i].src) < nameIndex(all[index].src)) index = i;
	}
	res.push_back(all[index]);

	// find new visit node
	int eraseIdx = index;
	bool pushDes = true;
	for (int i = 0; i < vis.size(); i++) {
		if (nameIndex(all[index].des) == vis[i]) {
			pushDes = false;
			break;
		}
	}
	if (pushDes) {
		vis.push_back(nameIndex(all[index].des));
		index = nameIndex(all[index].des);
	}
	else {
		vis.push_back(nameIndex(all[index].src));
		index = nameIndex(all[index].src);
	}
	all.erase(all.begin() + eraseIdx);

	// add to all
	for (int i = 0; i < vertexAmount; i++) {
		if (matrix[index][i] != 2147483647 && index != i) {
			edge temp;
			temp.src = nodeName[index];
			temp.des = nodeName[i];
			if (index > i) {
				string t = temp.src;
				temp.src = temp.des;
				temp.des = t;
			}
			temp.weight = matrix[index][i];
			all.push_back(temp);
		}
	}

	// erase same edge
	for (int i = 0; i < all.size(); i++) {
		for (int j = i + 1; j < all.size(); j++) {
			if (all[i].des == all[j].des && all[i].src == all[j].src) {
				all.erase(all.begin() + j);
				j--;
			}
		}
	}

	// erase both are visited node
	bool desVis = false, srcVis = false;
	for (int i = 0; i < all.size(); i++) {
		for (int j = 0; j < vis.size(); j++) {
			if (nameIndex(all[i].des) == vis[j]) desVis = true;
			if (nameIndex(all[i].src) == vis[j]) srcVis = true;
		}
		if (desVis && srcVis) {
			all.erase(all.begin() + i);
			i--;
		}
		desVis = false;
		srcVis = false;
	}

	finding(all, res, vis);
}

void Spanning::print(vector<edge>& res)
{
	justify_result(res);
	int sumWeight = 0;
	for (int i = 0; i < res.size(); i++) {
		sumWeight += res[i].weight;
	}
	std::cout << sumWeight << "\n";
	for (int i = 0; i < res.size(); i++) {
		std::cout << res[i].src << " " << res[i].des << " " << res[i].weight;
		if (i != res.size() - 1) std::cout << "\n";
	}
}

void Spanning::justify_result(vector<edge>& res)
{
	for (int i = 0; i < res.size(); i++) {
		for (int j = 0; j < res.size() - i - 1; j++) {
			if (res[j].weight > res[j + 1].weight) {
				edge temp = res[j];
				res[j] = res[j + 1];
				res[j + 1] = temp;
			}
			else if (res[j].weight == res[j + 1].weight) {
				if (nameIndex(res[j].src) > nameIndex(res[i].src)) {
					edge temp = res[j];
					res[j] = res[j + 1];
					res[j + 1] = temp;
				}
				else if (nameIndex(res[j].src) == nameIndex(res[i].src)) {
					if (nameIndex(res[j].des) > nameIndex(res[i].des)) {
						edge temp = res[j];
						res[j] = res[j + 1];
						res[j + 1] = temp;
					}
				}
			}
		}
	}
}

void Spanning::findMin()
{
	vector<edge> allEdges;
	vector<edge> resultEdges;
	vector<int> visited;
	for (int i = 0; i < vertexAmount; i++) {
		if (matrix[0][i] != 2147483647 && i != 0) {
			edge temp;
			temp.src = nodeName[0];
			temp.des = nodeName[i];
			temp.weight = matrix[0][i];
			allEdges.push_back(temp);
		}
	}
	visited.push_back(0);
	finding(allEdges, resultEdges, visited);
	print(resultEdges);
	
}
