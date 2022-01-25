#include <set>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
using namespace std;

int part1(string fileName) {
	int sum = 0;
	ifstream infile;
	infile.open(fileName);
	string line;
	set<char> answers = {};
	while (!infile.eof()) {
		getline(infile, line);
		if (line.empty()) {
			sum += answers.size();
			answers.clear();
		}
		else {
			answers.insert(line.begin(), line.end());
		}
	}
	infile.close();
	return sum;
}

int part2(string fileName) {
	int sum = 0;
	ifstream infile;
	infile.open(fileName);
	string line;
	set<char> answers = {};
	bool newGroup = true; // to check whether the line is a start of a new group
	//needed to ascertain whether to just insert or intersect with previous
	while (!infile.eof()) {
		getline(infile, line);
		if (line.empty()) {
			sum += answers.size();
			answers.clear();
			newGroup = true;
		} else {
			if (newGroup) {
				answers.insert(line.begin(), line.end());
				newGroup = false;
			}
			else if (!answers.empty()) {
				sort(line.begin(), line.end());
				set<char> intersection;
				// to perform set_intersection - both containers must be sorted !!!
				// sets are automatically sorted -> sort(...) cannot be used on them !!!
				set_intersection(answers.begin(), answers.end(), line.begin(), line.end(), inserter(intersection, intersection.begin()));
				answers = intersection;
			}
		}
	}
	infile.close();
	return sum;
}

int main() {
	cout << "Part 1:\t" << part1("input.txt") << endl;
	cout << "Part 2:\t" << part2("input.txt") << endl;
}