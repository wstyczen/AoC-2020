#include <string>
#include <vector>
#include <fstream>
#include <iterator>
#include <iostream>
using namespace std;

vector<vector<char>> getInput(string fileName) {
	vector<vector<char>> input = {};
	ifstream infile;
	infile.open(fileName);
	string line;
	while (!infile.eof()) {
		getline(infile, line);
		input.push_back(vector<char>(line.begin(), line.end()));
	}
	infile.close();
	return input;
}

struct Position {
	int x;
	int y;
	int dx;
	int dy;
	Position()
		: x{0}, y{0}
	{}
	void move() {
		this->x += this->dx;
		this->y += this->dy;
	}
	void setSlope(int newDx, int newDy) {
		this->dx = newDx;
		this->dy = newDy;
	}
};

int getTreesEncountered(const vector<vector<char>>& input, int slopeX=3, int slopeY=1) {
	int treeCount = 0;
	Position pos = Position();
	pos.setSlope(slopeX, slopeY);
	while (pos.y < input.size()) {
		if (input.at(pos.y).at(pos.x) == '#') { // a tree
			treeCount++;
		}
		pos.move();
		pos.x %= input.at(0).size();
	}
	return treeCount;
}

long long part2(const vector<vector<char>>& input, const vector<vector<int>>& slopes) {
	long long result = 1;
	for (auto slope = slopes.begin(); slope != slopes.end(); slope++) {
		result *= getTreesEncountered(input, (*slope).at(0), (*slope).at(1));
	}
	return result;
}

int main() {
	vector<vector<char>> input = getInput("input.txt");
	cout << "Part 1: " << getTreesEncountered(input) << endl;
	vector<vector<int>> slopes = { {1, 1}, {3, 1}, {5, 1}, {7, 1}, {1, 2} };
	cout << "Part 2: " << part2(input, slopes) << endl;
	return 0;
}