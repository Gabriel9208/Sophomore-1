#include<iostream>
#include<string>

using std::string;
using std::cin;
using std::cout;

int input(int*& a); // input list and return size
void bubble(int* list, int size);
void insertion(int* list, int size);
void selection(int* lista, int size);

int main() {
	int* list, size = 0;
	size = input(list);
	bubble(list, size);
	insertion(list, size);
	selection(list, size);
}

int input(int*& a) {
	string in, sub = "";
	std::getline(cin, in);
	int counter = 1, start = 0, length = 0, a_index = 0, a_size = 10;
	a = new int[100]; //21 24 28 15 20 19 30 41 20 28 13 12 33 25 7
	for (int i = 0; i < in.size(); i++) { // 12 3
		if (in[i] == ' ') {
			counter++;
			sub = in.substr(start, length);
			//dynamic allocation
			/*if (a_index = a_size - 1) {
				int* temp = new int[a_size * 2];
				for (int j = 0; j < a_size; j++) {
					temp[j] = a[j];
				}
				a_size *= 2;
				delete[] a;
				a = temp;
			}
			*/
			a[a_index] = stoi(sub);
			a_index++;
			start = i + 1;
			length = 0;
		}
		length++;
	}
	sub = in.substr(start, length);
	a[a_index] = stoi(sub);
	/*for (int i = 0; i < counter; i++) {
		cout << a[i];
		if (i != counter - 1) cout << " ";
	}
	cout << std::endl;
	*/
	return counter;
}
void bubble(int* list, int size) {
	int* a = new int[size];
	for (int i = 0; i < size; i++)	a[i] = list[i];
	int counter = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size - i; j++) {
			if (a[j] > a[j + 1]) {
				if (counter == 4) {
					cout << "Bubble: " << a[j + 1] << ", " << a[j] << "; ";
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
}	
void insertion(int* list, int size) {
	int* a = new int[size];
	for (int i = 0; i < size; i++)	a[i] = list[i];
	int start = 1, counter = 0; // beginning of unsort
	for (; start < size; start++) { // 21 24 28 15 20 19 30 41 20 28 13 12 33 25 7
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
void selection(int* list, int size) {
	int* a = new int[size];
	for (int i = 0; i < size; i++)	a[i] = list[i];
	int counter = 0, smallest = 100000, index = -1;
	int start = 0;// beginning of unsort
	for (; start < size; start++) {
		for (int i = start; i < size; i++) {
			if (a[i] < smallest) {
				smallest = a[i];
				index = i;
			}
		}
		int temp = a[start];
		a[start] = a[index];
		a[index] = temp;
		if (counter == 4) {
			cout << "Selection: " << a[start] << "; ";
			for (int i = 0; i < size; i++) {
				cout << a[i];
				if (i != size - 1) cout << " ";
			}
			cout << std::endl;
		}
		counter++;
		if (start < size - 1) smallest = a[start + 1];
	}
	for (int i = 0; i < size; i++) {
		cout << a[i];
		if (i != size - 1) cout << " ";
	}
}
