#include<iostream>
#include<utility>
using namespace std;

// problem dynamic array implement error
template <typename T>
class Stack {
	T* stack;
	int size;
	int element_amount;
	int top;
public:
	Stack() { stack = new T[100]; size = 100; element_amount = 0; top = 0; }
	Stack(const Stack& a);
	~Stack() { delete stack; }
	void push(T a);
	void pop();
	int get_element_amount() { return element_amount; }
	int find(T a);
	T get_top() { return stack[top]; }
	int get_top_num() { return top; }

	void operator =(Stack a);
};

class Map {
	int** map;
	int width;
public:
	Map(int a);
	~Map() {}
	void fill_in();
	int& get_element (int x, int y) { return map[y][x]; }
	int& get_element(pair<int,int> a) { return map[a.second][a.first]; }
	int get_width() { return width; }
	void print() {
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < width; j++) {
				cout << map[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
};

bool go(Stack<pair<int, int>>& have_gone, Map& map, pair<int, int>& pos);

int main() {
	// set up map
	int size = 0;
	cin >> size;
	Map map(size);
	map.fill_in();

	Stack<pair<int,int>> have_been;
	pair<int, int> pos = make_pair(0, 0);
	string result = "";
	if (go(have_been, map, pos)) { // print result
		int max = have_been.get_top_num();
		for (int i = 0; i < max; i++) {
			pos = have_been.get_top();
			have_been.pop();
			if (pos.first - have_been.get_top().first == 0) {
				if (pos.second - have_been.get_top().second == 1) result += "S";
				else result += "N";
			}
			else {
				if (pos.first - have_been.get_top().first == 1) result += "E";
				else result += "W";
			}
		}

		for (int i = result.size() - 1; i >= 0; i--) {
			cout << result[i];
		}
		cout << endl;
	}
	else cout << "error" << endl;
	
}

bool go(Stack<pair<int, int>>& have_gone, Map& map, pair<int, int>& pos) {
	if(pos == make_pair(map.get_width() - 1,map.get_width() - 1)) return true;
	if (pos.second - 1 >= 0 && map.get_element(pos.first, pos.second - 1) == 1) {// go up
		int temp = have_gone.find(make_pair(pos.first, pos.second - 1));
		if (temp == -1) { // not find duplicate
			pos = make_pair(pos.first, pos.second - 1);
			have_gone.push(pos);
			if (go(have_gone, map, pos)) return true;
		}
	}
	if (pos.first + 1 < map.get_width() && map.get_element(pos.first + 1, pos.second) == 1) {// go right
		int temp = have_gone.find(make_pair(pos.first + 1, pos.second));
		if (temp == -1) { // not find duplicate
			pos = make_pair(pos.first + 1, pos.second);
			have_gone.push(pos);
			if (go(have_gone, map, pos)) return true;
		}
	}
	if (pos.second + 1 < map.get_width() && map.get_element(pos.first, pos.second + 1) == 1) {// go down
		int temp = have_gone.find(make_pair(pos.first, pos.second + 1));
		if (temp == -1) { // not find duplicate
			pos = make_pair(pos.first, pos.second + 1);
			have_gone.push(pos);
			if (go(have_gone, map, pos)) return true;
		}
	}
	if (pos.first - 1 >= 0 && map.get_element(pos.first - 1, pos.second) == 1) {// go left
		int temp = have_gone.find(make_pair(pos.first - 1, pos.second));
		if (temp == -1) { // not find duplicate
			pos = make_pair(pos.first - 1, pos.second);
			have_gone.push(pos);
			if (go(have_gone, map, pos)) return true;
		}
	}
	have_gone.pop();
	pos = have_gone.get_top();
	return false;
}

template<typename T>
Stack<T>::Stack(const Stack& a)
{
	stack = new T[a.size];
	size = a.size;
	element_amount = a.element_amount;
	top = a.top;
}

template<typename T>
void Stack<T>::push(T a)
{
	// overflow
	if (top + 1 == size) {
		size *= 2;
		T* newS = new T[size];
		T* temp;
		for (int i = 0; i < element_amount; i++) {
			newS[i] = stack[i];
		}
		temp = stack;
		stack = newS;
		delete temp;
	}
	top++;
	element_amount++;
	stack[top] = a;

	for (int i = 0; i <= top; i++) {
		cout << stack[i].first << " " << stack[i].second << endl;
	}
	cout << endl;
}

template<typename T>
void Stack<T>::pop()
{
	top--;
	element_amount--;
}

template<typename T>
int Stack<T>::find(T a)
{
	for (int i = 0; i <= top; i++) {
		if (stack[i] == a) return i;
	}
	return -1;
}

template<typename T>
void Stack<T>::operator=(Stack a)
{
	delete stack;
	stack = new T[a.size];
	size = a.size;
	element_amount = a.element_amount;
	top = a.top;
}

Map::Map(int a)
{
	width = a;
	map = new int* [width];
	for (int i = 0; i < width; i++) {
		map[i] = new int[width];
	}
}

void Map::fill_in()
{
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < width; j++) {
			cin >> map[i][j];
		}
	}
}


/*
5
1 1 1 1 0
0 0 0 1 0
0 1 1 1 0
0 1 0 0 0
0 1 1 1 1
*/