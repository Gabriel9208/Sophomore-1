#include<iostream>
#include<utility>
using namespace std;

typedef struct {
	string name;
	int arrive_time;
	int process_time;
}person;

typedef struct {
	string name;
	int leave_time;
	int counter_num;
}leave_person;

class Queue {
	person* queue;
	int front;
	int rear;
	int size;
	int element_amount;
	int num;
public:
	Queue() { queue = new person[10]; size = 10; element_amount = 0; front = 0; rear = 0; }
	~Queue() { delete queue; }
	void push(person h);
	void pop();
	int get_element_amount() { return element_amount; }
	person top() { if (element_amount == 0) return person(); return queue[front]; }
	void wait(leave_person* toLeave, int time, int& leave_amount, int& size_of_toLeave);
	void set_num(int n) { num = n; }
};

int main() {
	int counterNum = 0;
	cin >> counterNum;
	Queue* counter = new Queue[counterNum];
	// initialize num of counter
	for (int i = 0; i < counterNum; i++) counter[i].set_num(i);
	string name = " ";
	int arrive_time = 0, process_time = 0;
	bool skip = false;
	while (1) {
		person human; human.name = "Last"; human.arrive_time = 100000; human.process_time = 0;
		if (cin >> name >> arrive_time >> process_time) { human.name = name; human.arrive_time = arrive_time; human.process_time = process_time; }
		else skip = true;
		leave_person* toLeave = new leave_person[10]; // who to leave
		int size_of_toLeave = 10;
		int leave_amount = 0; // how many people leave

		// some counter have someone leave;
		for (int j = 0; j < counterNum; j++)
			counter[j].wait(toLeave, human.arrive_time, leave_amount, size_of_toLeave);

		// line up
		pair<int,int> less_people = make_pair(0,1000);
		for (int j = 0; j < counterNum; j++)
			if (counter[j].get_element_amount() < less_people.second) less_people = make_pair(j, counter[j].get_element_amount());

		counter[less_people.first].push(human);

		for (int i = 0; i < leave_amount; i++) {
			for (int j = 0; j < leave_amount - 1 - i; j++) {
				if (toLeave[j].leave_time > toLeave[j + 1].leave_time) {
					leave_person t = toLeave[j];
					toLeave[j] = toLeave[j + 1];
					toLeave[j + 1] = t;
				}
			}
		}

		for (int i = 0; i < leave_amount; i++) {
			cout << toLeave[i].name << " " << toLeave[i].leave_time << " " << toLeave[i].counter_num << endl;
		}
		if (skip) break;
	}
}

void Queue::push(person h) {
	if (rear + 1 == size) {
		person* newQ = new person[size * 2];
		int index = 0;
		for (int i = front; i < rear + 1; i++, index++) {
			newQ[index] = queue[i];
		}
		person* temp = queue;
		queue = newQ;
		delete temp;
	}
	if (element_amount != 0) {
		rear++;
		queue[rear] = h;
		element_amount++;
	}
	else {
		queue[rear] = h;
		element_amount++;
	}
}
void Queue::pop() {
	front++; 
	element_amount--;
	if (front > rear) {
		front = 0;
		rear = 0;
		element_amount = 0;
	}
}
void Queue::wait(leave_person* toLeave, int time, int& leave_amount, int& size_of_toLeave){
	bool leave = false;
	int toAdd = 0;
	while (element_amount != 0) {
		// overflow
		if (leave_amount == size_of_toLeave) {
			size_of_toLeave = leave_amount * 2;
			leave_person* newQ = new leave_person[size_of_toLeave];
			for (int i = 0;  i < leave_amount; i++) newQ[i] = toLeave[i]; 
			leave_person* temp = toLeave;
			toLeave = newQ;
			delete temp;
		}
		// pop front to the leave array
		if (top().arrive_time + top().process_time <= time) {
			leave_person t;
			t.name = top().name; t.leave_time = top().arrive_time + top().process_time; t.counter_num = num;
			toAdd += t.leave_time;
			toLeave[leave_amount] = t;
			leave_amount++;
			pop();
			for (int i = front; i <= rear; i++) queue[i].arrive_time += toAdd - queue[front].arrive_time;
		}
		else break; // end function
	}
}