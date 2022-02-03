#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <chrono> // to measure runtime
using namespace std;

vector<vector<char>> getInput(string fileName) {
	vector<vector<char>> input = {};
	ifstream infile;
	infile.open(fileName);
	string line;
	while (infile.is_open() && !infile.eof()) {
		getline(infile, line);
		input.push_back(vector<char>(line.begin(), line.end()));
	}
	infile.close();
	return input;
}

bool isWithin(const vector<vector<char>>& input, int i, int j) {
	return (i >= 0 && i < input.size() && j >= 0 && j < input.at(0).size());
}

int getCountOfAdjacentOccupied(const vector<vector<char>>& input, int i, int j) {
	int occupied = 0;
	for (int x = i - 1; x <= i + 1; x++) {
		for (int y = j - 1; y <= j + 1; y++) {
			if (isWithin(input, x, y) && (x != i || y != j)) {
				if (input.at(x).at(y) == '#') {// occupied
					occupied++;
				}
			}
		}
	}
	return occupied;
}

int countOccupied(const vector<vector<char>>& input) {
	int occupied = 0;
	for (vector<char> row : input) {
		for (char c : row) {
			if (c == '#') {
				occupied++;
			}
		}
	}
	return occupied;
}

int isFirstOccupied(const vector<vector<char>>& input, int i, int j, int di, int dj) {
	int newI = i + di;
	int newJ = j + dj;
	while (isWithin(input, newI, newJ)) {
		if (input.at(newI).at(newJ) == '#')
			return true;
		else if (input.at(newI).at(newJ) == 'L')
			return false;
		newI += di;
		newJ += dj;
	}
	return false;
}

int howManyOccupiedSeen(const vector<vector<char>>& input, int i, int j, const vector<vector<int>>& directions) {
	int occupiedSeen = 0;
	for (const vector<int> dir : directions) {
		if (isFirstOccupied(input, i, j, dir.at(0), dir.at(1))) {
			occupiedSeen++;
		}
	}
	return occupiedSeen;
}

void simulateTillStable(vector<vector<char>>& input, string part) {
	bool changed = true;
	vector<vector<char>> newState = {};
	vector<char> thisRow = {};
	vector<vector<int>> directions = { {1,0}, {1, 1}, {0,1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1} };
	while (changed) {
		newState.clear();
		newState.reserve(input.size());// allocates the memory - to help with efficiency
		changed = false;
		for (int i = 0; i < input.size(); i++) {
			thisRow.clear();
			thisRow.reserve(input.at(0).size());// allocates the memory - to help with efficiency
			for (int j = 0; j < input.at(0).size(); j++) {
				char space = input.at(i).at(j);
				if (space == '.') {
					thisRow.push_back(space);
				}
				else {
					int occupiedCount = 0;
					int shiftToEmptyReq = 0;
					if (part == "1" || part == "p1" || part == "part 1") {
						occupiedCount = getCountOfAdjacentOccupied(input, i, j);
						shiftToEmptyReq = 4;
					}
					else if (part == "2" || part == "p2" || part == "part 2") {
						occupiedCount = howManyOccupiedSeen(input, i, j, directions);
						shiftToEmptyReq = 5;
					}
					else {
						throw - 1;
					}
					if (space == 'L' && occupiedCount == 0) {
						thisRow.push_back('#');
						changed = true;
					}
					else if (space == '#' && occupiedCount >= shiftToEmptyReq) {
						thisRow.push_back('L');
						changed = true;
					}
					else {
						thisRow.push_back(space);
					}
				}
			}
			newState.push_back(thisRow);
		}
		input = newState;
	}
}

int main() {
	auto start = chrono::high_resolution_clock::now();

	vector<vector<char>> input = getInput("input.txt");

	//simulateTillStable(input, "p1");
	//cout << "Part 1: " << countOccupied(input) << endl;
	

	// part 2 needs about 10s to run - needs some optimization
	simulateTillStable(input, "p2");
	cout << "Part 2: " << countOccupied(input) << endl;


	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
	cout << "Runtime: " << duration.count() << "s" << endl;
	return 0;
}