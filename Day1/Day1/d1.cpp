#include <vector>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

vector<int> getInput(string fileName) {
	ifstream file;
	vector<int> input = {};
	file.open(fileName);
	string line;
	while (!file.eof()) {
		getline(file, line);
		input.push_back(stoi(line));
	}
	file.close();
	return input;
}

int part1(const vector<int>& input) {
	for (auto first = input.begin(); first != input.end(); first++) {
		for (auto second = first+1; second != input.end(); second++) {
			if (*first + *second == 2020) {
				return (*first)*(*second);
			}
		}
	}
	return -1;
}

int part2(const vector<int>& input) {
	for (auto first = input.begin(); first != input.end(); first++) {
		for (auto second = first + 1; second != input.end(); second++) {
			for (auto third = second + 1; third != input.end(); third++) {
				if (*first + *second + *third == 2020) {
					return (*first) * (*second) * (*third);
				}
			}
		}
	}
	return -1;
}

int main() {
	vector<int> input = getInput("input.txt");
	cout << part1(input) << endl;
	cout << part2(input) << endl;
	return 0;
}