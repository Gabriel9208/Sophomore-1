#include<iostream>
#include<string>
#include<vector>

using std::string;
using std::cin;
using std::cout;
using std::vector;

int input(vector<int>& a); // input list and return size
void bubble(vector<int>& list);
void insertion(vector<int>& list);
void selection(vector<int>& list);

int main() {
	vector<int> list;
	int size = 0;
	size = input(list);
	bubble(list);
	insertion(list);
	selection(list);
}

int input(vector<int>& a) {
	int in;
	while (cin >> in) {
		a.push_back(in);
	}
	return a.size();
}
void bubble(vector<int>& list) {
	int* a = new int[list.size()];
	for (int i = 0; i < list.size(); i++)	a[i] = list[i];
	int counter = 0;
	for (int i = 0; i < list.size(); i++) {
		for (int j = 0; j < list.size() - 1; j++) {
			if (counter == 4) {
				if (a[j] > a[j + 1]) {
					cout << "Bubble: " << a[j + 1] << ", " << a[j] << "; ";
				}
			}
			if (a[j] > a[j + 1]) {
				int temp = a[j + 1];
				a[j + 1] = a[j];
				a[j] = temp;
				counter++;
			}
			if (counter == 5) {
				for (int i = 0; i < list.size(); i++) {
					cout << a[i];
					if (i != list.size() - 1) cout << " ";
				}
				cout << std::endl;
				return;
			}
		}
	}

	/*
	int i, j, counter = 0;
	for (i = 0; i < list.size() - 1; i++) {
		// Last i elements are already in place
		for (j = 0; j < list.size() - i - 1; j++) {
			if (a[j] > a[j + 1]) {				
				int temp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = temp;
				if (counter == 4) {
					cout << "Bubble: " << a[j] << ", " << a[j + 1] << "; ";
					for (int i = 0; i < list.size(); i++) {
						cout << a[i];
						if (i != list.size() - 1) cout << " ";
					}
					cout << std::endl;
				}
				counter++;
			}
		}
	}
	*/
}
void insertion(vector<int>& list) { 
	int* a = new int[list.size()];
	for (int i = 0; i < list.size(); i++)	a[i] = list[i];
	for (int start = 1; start < list.size(); start++) {
		for (int i = 0; i < start; i++) {
			if (a[i] > a[start]) { // 2 3 4 5 7 start = 4, i=3,  j=4,j>3
				int temp = a[start];
				for (int j = start; j > i; j--) {
					a[j] = a[j - 1];
				}
				a[i] = temp;
				if (start == 4) {
					cout << "Insertion: ";
					for (int i = 0; i < list.size(); i++) {
						cout << a[i];
						if (i != list.size() - 1) cout << " ";
					}
					cout << std::endl;
					return;
				}
				break;
			}
		}
	}
	
	/*
    for (int i = 1; i < list.size(); i++)
    {
        int key = a[i], j = i - 1;
        while (j >= 0 && a[j] > key)  {
            a[j + 1] = a[j];
            j = j - 1;
        }
        a[j + 1] = key;
		if (i == 4) {
			cout << "Insertion: ";
			for (int i = 0; i < list.size(); i++) {
				cout << a[i];
				if (i != list.size() - 1) cout << " ";
			}
			cout << std::endl;
			return;
		}
    }
	*/
}
void selection(vector<int>& list) {
	int* a = new int[list.size()];
	for (int i = 0; i < list.size(); i++)	a[i] = list[i];
	for (int start = 0; start < list.size(); start++) {
		int index = -1, smallest = a[start];
		for (int i = start; i < list.size(); i++) {
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
			for (int i = 0; i < list.size(); i++) {
				cout << a[i];
				if (i != list.size() - 1) cout << " ";
			}
			cout << std::endl;
		}
	}
	for (int i = 0; i < list.size(); i++) {
		cout << a[i];
		if (i != list.size() - 1) cout << " ";
	}
	
	/*
	for (int i = 0; i < list.size() - 1; i++)
	{
		// Find the minimum element in
		// unsorted array
		int min_idx = i;
		for (int j = i + 1; j < list.size(); j++)
			if (a[j] < a[min_idx])
				min_idx = j;

		// Swap the found minimum element
		// with the first element
		if (min_idx != i) {
			int temp = a[min_idx];
			a[min_idx] = a[i];
			a[i] = temp;
		}
		if (i == 4) {
			cout << "Selection: " << a[i] << "; ";
			for (int i = 0; i < list.size(); i++) {
				cout << a[i];
				if (i != list.size() - 1) cout << " ";
			}
			cout << std::endl;
		}
	}
	for (int i = 0; i < list.size(); i++) {
		cout << a[i];
		if (i != list.size() - 1) cout << " ";
	}
	*/
}
