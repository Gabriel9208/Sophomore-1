//using modified DFS
#include<iostream>
#include<vector>
#include<exception>

using std::vector;

class Graph {
public:
	int nodeAmount;
	vector<int>* neigh;
	Graph(int a) { nodeAmount = 0; if (a >= 0) nodeAmount = a; neigh = new vector<int>[nodeAmount]; }
	void add(int src, int des){ neigh[src].push_back(des); }
	bool findCycle(int start);
	bool finding(bool* rec, bool* vis, int start);
};

int main() {
	int totalNode = 0, src, des;
	std::cin >> totalNode;
	Graph graph(totalNode);
	while (std::cin >> src >> des) 
		graph.add(src % totalNode, des % totalNode);
	if (graph.findCycle(0)) std::cout << "Cycle";
	else std::cout << "No Cycle";
	
}

bool Graph::findCycle(int start)
{
	bool* visited = new bool[nodeAmount];
	bool* recur = new bool[nodeAmount];
	for (int i = 0; i < nodeAmount; i++) {
		visited[i] = false;
		recur[i] = false;
	}
	for (int i = 0; i < nodeAmount; i++) {
		if(!visited[i] && finding(recur, visited, i)) return true;
	}
	return false;
}

bool Graph::finding(bool* rec, bool* vis, int start)
{
	if (!vis[start]) {
		vis[start] = true;
		rec[start] = true;
		for (int i = 0; i < neigh[start].size(); i++) 
			if ((!vis[neigh[start][i]] && finding(rec, vis, neigh[start][i])) || rec[neigh[start][i]]) return true;
	}
	rec[start] = false;
	return false;
}
