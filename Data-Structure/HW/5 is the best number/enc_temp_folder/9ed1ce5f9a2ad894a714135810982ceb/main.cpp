#include<iostream>
#include<string>
#include<vector>

using std::vector;
using std::string;
using std::cin;
using std::cout;

int input(double*& a); // input list and return size
void bubble(double* list, int size);
void insertion(double* list, int size);
void selection(double* lista, int size);

int main() {
	double* list;
	int size = 0;
	size = input(list);
	bubble(list, size);
	insertion(list, size);
	selection(list, size);
}

int input(double*& a) {
	/*
	string in, sub = "";
	std::getline(cin, in);
	double counter = 1, start = 0, length = 0, a_index = 0, a_size = 10;
	a = new double[100]; //21 24 28 15 20 19 30 41 20 28 13 12 33 25 7
	for (double i = 0; i < in.size(); i++) { // 12 3
		if (in[i] == ' ') {
			counter++;
			sub = in.substr(start, length);
			a[a_index] = stoi(sub);
			a_index++;
			start = i + 1;
			length = 0;
		}
		length++;
	}
	sub = in.substr(start, length);
	a[a_index] = stoi(sub);
	return counter;
	*/
	double temp;
	vector<double> l;
	while (cin >> temp) {
		l.push_back(temp);
	}
	a = new double[l.size()];
	for (int i = 0; i < l.size(); i++) {
		a[i] = l[i];
	}
	return l.size();

}
void bubble(double* list, int size) {
	double* a = new double[size];
	for (int i = 0; i < size; i++)	a[i] = list[i];
	int counter = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size - i; j++) {
			if (a[j] > a[j + 1]) {
				if (counter == 4) {
					cout << "Bubble: " << a[j + 1] << ", " << a[j] << "; ";
				}
				double temp = a[j + 1];
				a[j + 1] = a[j];
				a[j] = temp;
				counter++;
				if (counter == 5) {
					for (int k = 0; k < size; k++) {
						cout << a[k];
						if (k != size - 1) cout << " ";
					}
					cout << std::endl;
					return;
				}
			}
		}
	}
}
void insertion(double* list, int size) {
	double* a = new double[size];
	for (int i = 0; i < size; i++)	a[i] = list[i];
	int start = 1, counter = 0; // beginning of unsort
	for (; start < size; start++) { // 21 24 28 15 20 19 30 41 20 28 13 12 33 25 7
		for (int i = 0; i < start; i++) {
			if (a[i] > a[start]) {
				double temp = a[start];
				for (int j = start; j > i; j--) {
					a[j] = a[j - 1];
				}
				a[i] = temp;
			}
			if (counter == 4) {
				cout << "Insertion: ";
				for (int j = 0; j < size; j++) {
					cout << a[j];
					if (j != size - 1) cout << " ";
				}
				cout << std::endl;
				return;
			}
		}
		counter++;
	}
}
void selection(double* list, int size) {
	double* a = new double[size];
	for (int i = 0; i < size; i++)	a[i] = list[i];
	int counter = 0;
	double smallest;
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
			double temp = a[start];
			a[start] = a[index];
			a[index] = temp;
		}
		if (counter == 4) {
			cout << "Selection: " << a[start] << "; ";
			for (int j = 0; j < size; j++) {
				cout << a[j];
				if (j != size - 1) cout << " ";
			}
			cout << std::endl;
		}
		counter++;
	}
	for (int i = 0; i < size; i++) {
		cout << a[i];
		if (i != size - 1) cout << " ";
	}
}