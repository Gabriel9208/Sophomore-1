#include<iostream>
#include<string>
#include<fstream>
#include<vector>

using std::vector;
using std::ifstream;
using std::ofstream;
using std::string;
using std::cin;
using std::cout;
using std::endl;

typedef struct {
	string now;
	string next;
	vector<string> inout;
}outData;

class Board_Element {
	string name;
	string inp00_next_state;
	string inp00_output;
	string inp01_next_state;
	string inp01_output;
	string inp10_next_state;
	string inp10_output;
	string inp11_next_state;
	string inp11_output;
public:
	Board_Element() :name(""), inp00_next_state(""), inp00_output(""), inp11_next_state(""), inp11_output(""),
		inp01_next_state(""), inp01_output(""), inp10_next_state(""), inp10_output("") {}

	string get_name() { return name; }
	string get_inp00_next_state() { return inp00_next_state; }
	string get_inp00_output() { return inp00_output; }
	string get_inp01_next_state() { return inp01_next_state; }
	string get_inp01_output() { return inp01_output; }
	string get_inp10_next_state() { return inp10_next_state; }
	string get_inp10_output() { return inp10_output; }
	string get_inp11_next_state() { return inp11_next_state; }
	string get_inp11_output() { return inp11_output; }

	void set_name(string a) { name = a; }
	void set_inp00_next_state(string a) { inp00_next_state = a; }
	void set_inp00_output(string a) { inp00_output = a; }
	void set_inp01_next_state(string a) { inp01_next_state = a; }
	void set_inp01_output(string a) { inp01_output = a; }
	void set_inp10_next_state(string a) { inp10_next_state = a; }
	void set_inp10_output(string a) { inp10_output = a; }
	void set_inp11_next_state(string a) { inp11_next_state = a; }
	void set_inp11_output(string a) { inp11_output = a; }
};
class Board {
	bool two_in;
	int num_terms;
	int num_states;
	string reset_state;
	vector<Board_Element> content;
public:
	Board() :num_terms(0), num_states(0), reset_state(""), two_in(false) {}

	int get_num_terms() { return num_terms; }
	int get_num_states() { return num_states; }
	string get_reset_state() { return reset_state; }
	bool get_two_in() { return two_in; }

	void set_num_terms(const int& a) { num_terms = a; }
	void set_num_states(const int& a) { num_states = a; }
	void set_reset_state(const string& a) { reset_state = a; }
	void set_two_in(bool a) { two_in = a; }

	void content_add(Board_Element& a) { content.push_back(a); };
	int content_find_state(string a);
	void content_set_inp00_next_state(int index, string a) { content[index].set_inp00_next_state(a); }
	void content_set_inp00_output(int index, string a) { content[index].set_inp00_output(a); }
	void content_set_inp01_next_state(int index, string a) { content[index].set_inp01_next_state(a); }
	void content_set_inp01_output(int index, string a) { content[index].set_inp01_output(a); }
	void content_set_inp10_next_state(int index, string a) { content[index].set_inp10_next_state(a); }
	void content_set_inp10_output(int index, string a) { content[index].set_inp10_output(a); }
	void content_set_inp11_next_state(int index, string a) { content[index].set_inp11_next_state(a); }
	void content_set_inp11_output(int index, string a) { content[index].set_inp11_output(a); }
	int content_size() { return content.size(); }
	Board_Element content_get(int index) { return content[index]; }
	void content_delete(string a, string remain);

	void order_content(); // let content is in alphabetical(identify by name) order
	void output_kiss(ofstream& out);
	void output_dot(int argc, char** argv);
};

class TwoD_Element {
	bool compatible;
	string row;
	string col;
	vector<string> inp00_relation;
	vector<string> inp01_relation;
	vector<string> inp10_relation;
	vector<string> inp11_relation;
public:
	TwoD_Element() :compatible(true), row(""), col("") {}

	void set_row(string a) { row = a; }
	void set_col(string a) { col = a; }
	void set_inp00_relation(string a, string b) { inp00_relation.push_back(a); inp00_relation.push_back(b); }
	void set_inp01_relation(string a, string b) { inp01_relation.push_back(a); inp01_relation.push_back(b); }
	void set_inp10_relation(string a, string b) { inp10_relation.push_back(a); inp10_relation.push_back(b); }
	void set_inp11_relation(string a, string b) { inp11_relation.push_back(a); inp11_relation.push_back(b); }
	void set_compatible(bool a) { compatible = a; }

	string get_row() { return row; }
	string get_col() { return col; }
	vector<string> get_inp00_relation() { return inp00_relation; }
	vector<string> get_inp01_relation() { return inp01_relation; }
	vector<string> get_inp10_relation() { return inp10_relation; }
	vector<string> get_inp11_relation() { return inp11_relation; }
	bool get_compatible() { return compatible; }
};
class TwoD {
	bool two_in;
	int size;
	TwoD_Element** map;
	vector<string> namelist;
public:
	TwoD(Board& board);

	void element_compare();
	void set_map(Board& board);
	int find_name_in_list(string a);
	void run(Board& board);
	void merge(Board& board);
};

bool read_file(ofstream& out, Board& board, int argc, char** argv);

int main(int argc, char** argv) {
	ofstream out;
	Board board;
	if (read_file(out, board, argc, argv)) {
		board.order_content();
		TwoD td(board);
		td.run(board);
		board.output_kiss(out);
		board.output_dot(argc, argv);
	}
	return 0;
}

bool read_file(ofstream& out, Board& board, int argc, char** argv) {
	string name;
	ifstream in(argv[1]);
	out.open(argv[2]);
	in >> name;
	out << ".start_kiss" << endl << ".i ";
	if (!in.is_open()) return false;
	int temp = -1;
	int index = -1;
	while (in >> name) {
		if (name[0] == '.') {
			switch (name[1]) {
			case 'i':
				in >> name;
				temp = stoi(name);
				if (temp == 2) 
					board.set_two_in(true);
				out << temp << endl << ".o ";
				break;
			case 'o':
				in >> name;
				out << stoi(name) << endl << ".p ";
				break;
			case 'p':
				in >> name;
				board.set_num_terms(stoi(name));
				break;
			case 's':
				in >> name;
				board.set_num_states(stoi(name));
				break;
			case 'r':
				in >> name;
				board.set_reset_state(name);
				break;
			}
		}
		else {
			bool twoIn = board.get_two_in();
			switch (name[0]) {
			case '0':
				if (twoIn && name[1] == '1') {
					in >> name;
					index = board.content_find_state(name);
					if (index == -1) {
						Board_Element temp;
						temp.set_name(name);
						in >> name;
						temp.set_inp01_next_state(name);
						in >> name;
						temp.set_inp01_output(name);
						board.content_add(temp);
					}
					else {
						in >> name;
						board.content_set_inp01_next_state(index, name);
						in >> name;
						board.content_set_inp01_output(index, name);
					}
				}
				else {
					in >> name;
					index = board.content_find_state(name);
					if (index == -1) {
						Board_Element temp;
						temp.set_name(name);
						in >> name;
						temp.set_inp00_next_state(name);
						in >> name;
						temp.set_inp00_output(name);
						board.content_add(temp);
					}
					else {
						in >> name;
						board.content_set_inp00_next_state(index, name);
						in >> name;
						board.content_set_inp00_output(index, name);
					}
				}
				break;
			case '1':
				if (twoIn) {
					switch (name[1]) {
					case '0':
						in >> name;
						index = board.content_find_state(name);
						if (index == -1) {
							Board_Element temp;
							temp.set_name(name);
							in >> name;
							temp.set_inp10_next_state(name);
							in >> name;
							temp.set_inp10_output(name);
							board.content_add(temp);
						}
						else {
							in >> name;
							board.content_set_inp10_next_state(index, name);
							in >> name;
							board.content_set_inp10_output(index, name);
						}
						break;
					case '1':
						in >> name;
						index = board.content_find_state(name);
						if (index == -1) {
							Board_Element temp;
							temp.set_name(name);
							in >> name;
							temp.set_inp11_next_state(name);
							in >> name;
							temp.set_inp11_output(name);
							board.content_add(temp);
						}
						else {
							in >> name;
							board.content_set_inp11_next_state(index, name);
							in >> name;
							board.content_set_inp11_output(index, name);
						}
					}
				}
				else {
					in >> name;
					index = board.content_find_state(name);
					if (index == -1) {
						Board_Element temp;
						temp.set_name(name);
						in >> name;
						temp.set_inp01_next_state(name);
						in >> name;
						temp.set_inp01_output(name);
						board.content_add(temp);
					}
					else {
						in >> name;
						board.content_set_inp01_next_state(index, name);
						in >> name;
						board.content_set_inp01_output(index, name);
					}
				}
				break;
			}
		}
	}
}

int Board::content_find_state(string a)
{
	int size = content.size();
	if (size == 0) return -1;
	for (int i = 0; i < size; i++) {
		if (content[i].get_name() == a) return i;
	}
	return -1;
}

void Board::content_delete(string a, string remain)
{
	bool del = false;
	for (int i = 0; i < content.size(); i++) {
		if (content[i].get_name() == a) {
			content.erase(content.begin() + i);
			del = true;
			num_states--;
			if (two_in) {
				num_terms -= 2;
			}
			num_terms -= 2;
			break;
		}
	}
	if (del) {
		for (int i = 0; i < content.size(); i++) {
			if (content[i].get_inp00_next_state() == a) content[i].set_inp00_next_state(remain);
			if (content[i].get_inp01_next_state() == a) content[i].set_inp01_next_state(remain);
			if (two_in) {
				if (content[i].get_inp10_next_state() == a) content[i].set_inp10_next_state(remain);
				if (content[i].get_inp11_next_state() == a) content[i].set_inp11_next_state(remain);
			}
		}
	}

}

void Board::order_content() {
	int size = content.size();
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size - 1 - i; j++) {
			if (content[j].get_name().compare(content[j + 1].get_name()) > 0) {
				Board_Element temp = content[j];
				content[j] = content[j + 1];
				content[j + 1] = temp;
			}
		}
	}
}

void Board::output_kiss(ofstream& out)
{
	out << num_terms << endl << ".s " << num_states << endl << ".r " << reset_state << endl;
	if (two_in) {
		for (int i = 0; i < content.size(); i++) {
			out << "00 " << content[i].get_name() << " " << content[i].get_inp00_next_state() << " " << content[i].get_inp00_output() << endl;
			out << "01 " << content[i].get_name() << " " << content[i].get_inp01_next_state() << " " << content[i].get_inp01_output() << endl;
			out << "10 " << content[i].get_name() << " " << content[i].get_inp10_next_state() << " " << content[i].get_inp10_output() << endl;
			out << "11 " << content[i].get_name() << " " << content[i].get_inp11_next_state() << " " << content[i].get_inp11_output() << endl;
		}
	}
	else {
		for (int i = 0; i < content.size(); i++) {
			out << "0 " << content[i].get_name() << " " << content[i].get_inp00_next_state() << " " << content[i].get_inp00_output() << endl;
			out << "1 " << content[i].get_name() << " " << content[i].get_inp01_next_state() << " " << content[i].get_inp01_output() << endl;
		}
	}
	out << ".end_kiss";
}

void Board::output_dot(int argc, char** argv)
{
	ofstream output;
	output.open(argv[3]);
	vector<outData> list;
	output << "digraph STG{" << endl << "rankdir=LR;" << endl << endl << "INIT [shape = point];" << endl;
	for (int i = 0; i < content.size(); i++) {
		string temp = content[i].get_name();
		output << temp << " [label=\"" << temp << "\"];" << endl;
	}
	output << endl << "INIT -> " << content[0].get_name() << ";" << endl;
	if (two_in) {
		for (int i = 0; i < content.size(); i++) {
			bool find = false;
			string str;
			outData temp;
			temp.now = content[i].get_name();
			temp.next = content[i].get_inp00_next_state();
			str = "00/" + content[i].get_inp00_output();
			temp.inout.push_back(str);
			list.push_back(temp);
			for (int j = 0; j < list.size(); j++) {
				if (list[j].now == content[i].get_name() && list[j].next == content[i].get_inp01_next_state()) {
					str = "01/" + content[i].get_inp01_output();
					list[j].inout.push_back(str);
					find = true;
					break;
				}
			}
			if (!find) {
				temp.next = content[i].get_inp01_next_state();
				str = "01/" + content[i].get_inp01_output();
				temp.inout.clear();
				temp.inout.push_back(str);
				list.push_back(temp);
			}
			find = false;
			for (int j = 0; j < list.size(); j++) {
				if (list[j].now == content[i].get_name() && list[j].next == content[i].get_inp10_next_state()) {
					str = "10/" + content[i].get_inp10_output();
					list[j].inout.push_back(str);
					find = true;
					break;
				}
			}
			if (!find) {
				temp.next = content[i].get_inp10_next_state();
				str = "10/" + content[i].get_inp10_output();
				temp.inout.clear();
				temp.inout.push_back(str);
				list.push_back(temp);
			}
			find = false;
			for (int j = 0; j < list.size(); j++) {
				if (list[j].now == content[i].get_name() && list[j].next == content[i].get_inp11_next_state()) {
					str = "11/" + content[i].get_inp11_output();
					list[j].inout.push_back(str);
					find = true;
					break;
				}
			}
			if (!find) {
				temp.next = content[i].get_inp11_next_state();
				str = "11/" + content[i].get_inp11_output();
				temp.inout.clear();
				temp.inout.push_back(str);
				list.push_back(temp);
			}
		}
	}
	else {
		for (int i = 0; i < content.size(); i++) {
			bool find = false;
			string str;
			outData temp;
			temp.now = content[i].get_name();
			temp.next = content[i].get_inp00_next_state();
			str = "0/" + content[i].get_inp00_output();
			temp.inout.push_back(str);
			list.push_back(temp);
			for (int j = 0; j < list.size(); j++) {
				if (list[j].now == content[i].get_name() && list[j].next == content[i].get_inp01_next_state()) {
					str = "1/" + content[i].get_inp01_output();
					list[j].inout.push_back(str);
					find = true;
					break;
				}
			}
			if (!find) {
				temp.next = content[i].get_inp01_next_state();
				str = "1/" + content[i].get_inp01_output();
				temp.inout.clear();
				temp.inout.push_back(str);
				list.push_back(temp);
			}
		}
	}
	for (int i = 0; i < list.size(); i++) {
		output << list[i].now << " -> " << list[i].next << " [label=\"";
		for (int j = 0; j < list[i].inout.size(); j++) {
			output << list[i].inout[j];
			if (j != list[i].inout.size() - 1) output << ",";
		}
		output << "\"];" << endl;;
	}
	output << "}";
}

void TwoD::set_map(Board& board) {
	bool twoIn = board.get_two_in();
	// fill in name list
	for (int i = 0; i < size; i++) {
		namelist.push_back(board.content_get(i).get_name());
	}
	// fill in map, quick screening
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < i; j++) {
			string content_i_inp00_next_state = board.content_get(i).get_inp00_next_state();
			string content_i_inp01_next_state = board.content_get(i).get_inp01_next_state();
			string content_j_inp00_next_state = board.content_get(j).get_inp00_next_state();
			string content_j_inp01_next_state = board.content_get(j).get_inp01_next_state();
			string content_i_inp10_next_state = "";
			string content_i_inp11_next_state = "";
			string content_j_inp10_next_state = "";
			string content_j_inp11_next_state = "";
			if (twoIn) {
				content_i_inp10_next_state = board.content_get(i).get_inp10_next_state();
				content_i_inp11_next_state = board.content_get(i).get_inp11_next_state();
				content_j_inp10_next_state = board.content_get(j).get_inp10_next_state();
				content_j_inp11_next_state = board.content_get(j).get_inp11_next_state();
			}
			map[i][j].set_row(namelist[i]);
			map[i][j].set_col(namelist[j]);
			map[i][j].set_inp00_relation(content_i_inp00_next_state, content_j_inp00_next_state);
			map[i][j].set_inp01_relation(content_i_inp01_next_state, content_j_inp01_next_state);
			if (twoIn) {
				map[i][j].set_inp10_relation(content_i_inp10_next_state, content_j_inp10_next_state);
				map[i][j].set_inp11_relation(content_i_inp11_next_state, content_j_inp11_next_state);
			}
			if (board.content_get(i).get_inp00_output() != board.content_get(j).get_inp00_output() ||
				board.content_get(i).get_inp01_output() != board.content_get(j).get_inp01_output()) {
				map[i][j].set_compatible(false);
				continue;
			}
			if (twoIn) {
				if (board.content_get(i).get_inp10_output() != board.content_get(j).get_inp10_output() ||
					board.content_get(i).get_inp11_output() != board.content_get(j).get_inp11_output()) {
					map[i][j].set_compatible(false);
					continue;
				}
			}
			if (board.content_get(board.content_find_state(content_i_inp00_next_state)).get_inp00_output() !=
				board.content_get(board.content_find_state(content_j_inp00_next_state)).get_inp00_output() ||
				board.content_get(board.content_find_state(content_i_inp00_next_state)).get_inp01_output() !=
				board.content_get(board.content_find_state(content_j_inp00_next_state)).get_inp01_output()) {
				map[i][j].set_compatible(false);
				continue;
			}
			if (twoIn) {
				if (board.content_get(board.content_find_state(content_i_inp10_next_state)).get_inp10_output() !=
					board.content_get(board.content_find_state(content_j_inp10_next_state)).get_inp10_output() ||
					board.content_get(board.content_find_state(content_i_inp10_next_state)).get_inp11_output() !=
					board.content_get(board.content_find_state(content_j_inp10_next_state)).get_inp11_output()) {
					map[i][j].set_compatible(false);
					continue;
				}
			}
			if (board.content_get(board.content_find_state(content_i_inp01_next_state)).get_inp00_output() !=
				board.content_get(board.content_find_state(content_j_inp01_next_state)).get_inp00_output() ||
				board.content_get(board.content_find_state(content_i_inp01_next_state)).get_inp01_output() !=
				board.content_get(board.content_find_state(content_j_inp01_next_state)).get_inp01_output()) {
				map[i][j].set_compatible(false);
			}
			if (twoIn) {
				if (board.content_get(board.content_find_state(content_i_inp11_next_state)).get_inp10_output() !=
					board.content_get(board.content_find_state(content_j_inp11_next_state)).get_inp10_output() ||
					board.content_get(board.content_find_state(content_i_inp11_next_state)).get_inp11_output() !=
					board.content_get(board.content_find_state(content_j_inp11_next_state)).get_inp11_output()) {
					map[i][j].set_compatible(false);
				}
			}
		}
	}
	/*for (int i = 0; i < size; i++) {
		for (int j = 0; j < i; j++) {
			if (map[i][j].get_compatible())cout << "true" << endl;
			else cout << "false" << endl;
		}
	}
	*/
}

int TwoD::find_name_in_list(string a)
{
	for (int i = 0; i < size; i++) {
		if (namelist[i] == a) return i;
	}
	return -1;
}

void TwoD::run(Board& board)
{
	set_map(board);
	element_compare();
	merge(board);
}

void TwoD::merge(Board& board) {
	int sizet = board.content_size();
	for (int i = 0; i < sizet; i++) {
		for (int j = 0; j < i; j++) {
			if (map[i][j].get_compatible()) {
				board.content_delete(map[i][j].get_row(), map[i][j].get_col());
			}
		}
	}
}

TwoD::TwoD(Board& board)
{
	two_in = board.get_two_in();
	size = board.content_size();
	map = new TwoD_Element * [10];
	for (int i = 0; i < size; i++) {
		map[i] = new TwoD_Element[10];
	}
}

void TwoD::element_compare() {
	bool success = false;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < i; j++) {
			if (map[i][j].get_compatible()) {
				string str1 = map[i][j].get_inp00_relation()[0];
				string str2 = map[i][j].get_inp00_relation()[1];
				if (str1 != str2) {
					int index1 = find_name_in_list(str1);
					int index2 = find_name_in_list(str2);
					if (index2 < index1) {
						int temp = index1;
						index1 = index2;
						index2 = temp;
					}
					if (!map[index2][index1].get_compatible()) {
						map[i][j].set_compatible(false);
						success = true;
						continue;
					}
				}
				str1 = map[i][j].get_inp01_relation()[0];
				str2 = map[i][j].get_inp01_relation()[1];
				if (str1 != str2) {
					int index1 = find_name_in_list(str1);
					int index2 = find_name_in_list(str2);
					if (index2 < index1) {
						int temp = index1;
						index1 = index2;
						index2 = temp;
					}
					if (!map[index2][index1].get_compatible()) {
						map[i][j].set_compatible(false);
						success = true;
						continue;
					}
				}
				if (two_in) {
					str1 = map[i][j].get_inp10_relation()[0];
					str2 = map[i][j].get_inp10_relation()[1];
					if (str1 != str2) {
						int index1 = find_name_in_list(str1);
						int index2 = find_name_in_list(str2);
						if (index2 < index1) {
							int temp = index1;
							index1 = index2;
							index2 = temp;
						}
						if (!map[index2][index1].get_compatible()) {
							map[i][j].set_compatible(false);
							success = true;
							continue;
						}
					}
					str1 = map[i][j].get_inp11_relation()[0];
					str2 = map[i][j].get_inp11_relation()[1];
					if (str1 != str2) {
						int index1 = find_name_in_list(str1);
						int index2 = find_name_in_list(str2);
						if (index2 < index1) {
							int temp = index1;
							index1 = index2;
							index2 = temp;
						}
						if (!map[index2][index1].get_compatible()) {
							map[i][j].set_compatible(false);
							success = true;
						}
					}
				}
			}
		}
	}
	if (success) element_compare();
}