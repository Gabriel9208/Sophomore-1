#include<iostream>
#include<string>
#include<vector>

using std::string;
using std::cin;
using std::cout;
using std::vector;

int input(vector<int> a); // input list and return size
void bubble(vector<int> list, int size);
void insertion(vector<int> list, int size);
void selection(vector<int> list, int size);

int main() {
	vector<int> list;
	int size = 0;
	size = input(list);
	bubble(list, size);
	insertion(list, size);
	selection(list, size);
}

int input(vector<int> a) {
	int in;
	while (cin >> in) {
		a.push_back(in);
	}
	return a.size();
}
void bubble(vector<int> list, int size) {
	int* a = new int[size];
	for (int i = 0; i < size; i++)	a[i] = list[i];
	int counter = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size - 1; j++) {
			if (counter == 4) {
				if (a[j] > a[j + 1]) {
					cout << "Bubble: " << a[j + 1] << ", " << a[j] << "; ";
				}
			}
			int temp = a[j + 1];
			a[j + 1] = a[j];
			a[j] = temp;
			counter++;
			if (counter == 5) {
				for (int i = 0; i < size; i++) {
					cout << a[i];
					if (i != size - 1) cout << " ";
				}
				cout << std::endl;
				return;
			}
		}
	}
}
void insertion(vector<int> list, int size) {
	int* a = new int[size];
	for (int i = 0; i < size; i++)	a[i] = list[i];
	int start = 1, counter = 0; // beginning of unsort
	for (; start < size; start++) {
		for (int i = 0; i < start; i++) {
			if (a[i] > a[start]) {
				int temp = a[start];
				for (int j = start; j > i; j--) {
					a[j] = a[j - 1];
				}
				a[i] = temp;
			}
			if (counter == 4) {
				cout << "Insertion: ";
				for (int i = 0; i < size; i++) {
					cout << a[i];
					if (i != size - 1) cout << " ";
				}
				cout << std::endl;
				return;
			}
		}
		counter++;
	}
}
void selection(vector<int> list, int size) {
	int* a = new int[size];
	for (int i = 0; i < size; i++)	a[i] = list[i];
	int smallest;
	for (int start = 0; start < size; start++) {
		int index = -1;
		smallest = a[start];
		for (int i = start; i < size; i++) {
			if (a[i] < smallest) {
				smallest = a[i];
				index = i;
			}
		}
		if (index != -1) {
			int temp = a[start];
			a[start] = a[index];
			a[index] = temp;
		}
		if (start == 4) {
			cout << "Selection: " << a[start] << "; ";
			for (int i = 0; i < size; i++) {
				cout << a[i];
				if (i != size - 1) cout << " ";
			}
			cout << std::endl;
		}
	}
	for (int i = 0; i < size; i++) {
		cout << a[i];
		if (i != size - 1) cout << " ";
	}
}
