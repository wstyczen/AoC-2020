#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;

struct Ticket {
	string str;
	int row;
	int col;
	Ticket(string str_)
		: str{str_}
	{
		initRowAndCol();
	}
	void initRowAndCol(int rowLen = 7, int colLen = 3) {
		int low = 0;
		int high = pow(2, rowLen) - 1;
		auto it = this->str.begin();
		while (low != high) {
			if (*it == 'F') { // front
				high = (high + low)/2;
			}
			else if (*it == 'B') { // back
				low = (high + low) / 2 + 1;
			} else throw;
			it++;
		}
		this->row = high;
		low = 0; high = pow(2, colLen) - 1;
		while (low != high) {
			if (*it == 'L') {
				high = (high + low) / 2;
			}
			else if (*it == 'R') {
				low = (high + low) / 2 + 1;
			} else throw;
			it++;
		}
		this->col = high;
	}
	int getId() const {
		return this->row*8 + this->col;
	}
};

ostream& operator<<(ostream& os, const Ticket& ticket) {
	os << ticket.str << ":\tr: " << ticket.row << "\tc: " << ticket.col << "\tid: " << ticket.getId();
	return os;
}

int getHighestId(string fileName) {
	int highestId = -1;
	ifstream infile;
	infile.open(fileName);
	string line;
	while (!infile.eof()) {
		getline(infile, line);
		Ticket ticket = Ticket(line);
		//cout << ticket << endl;
		if (ticket.getId() > highestId) highestId = ticket.getId();
	}
	infile.close();
	return highestId;
}

int getMissingId(string fileName) {
	vector<int> ids = {};
	ifstream infile;
	infile.open(fileName);
	string line;
	while (!infile.eof()) {
		getline(infile, line);
		Ticket ticket = Ticket(line);
		ids.push_back(ticket.getId());
	}
	infile.close();
	sort(ids.begin(), ids.end());
	int prev = 0; int current = 0;
	auto it = ids.begin();
	while (it != ids.end()) {
		current = *it;
		if (prev && current) {
			if (prev == current - 2) return current - 1;
		}
		prev = *it;
		it++;
	}
	return -1;
}

int main() {
	cout << "Part 1:\t" << getHighestId("input.txt") << endl;
	cout << "Part 2:\t" << getMissingId("input.txt") << endl;
	return 0;
}