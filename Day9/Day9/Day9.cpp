#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

vector<long long> getInput(string fileName) {
	vector<long long> input = {};
	ifstream infile;
	infile.open(fileName);
	string line;
	while (!infile.eof()) {
		getline(infile, line);
		input.push_back(stoll(line));
	}
	infile.close();
	return input;
}

bool sumInPreamble(const vector<long long>& numbers, int preambleLength, int index) {
	if (index < preambleLength) throw;
	for (int i = index - preambleLength; i < index-1; i++) {
		for (int j = i+1; j < index; j++) {
			if (numbers.at(index) == numbers.at(i) + numbers.at(j))
				return true;
		}
	}
	return false;
}

long long oddNumber(const vector<long long>& numbers, int preambleLength) {
	int index = 0;
	for (long long nr : numbers) {
		if (index >= preambleLength) {
			if (!sumInPreamble(numbers, preambleLength, index))
				return numbers.at(index);
		}
		index++;
	}
	return -1;
}

long long sumOfLimitsOfRangeThatSumsTo(const vector<long long>& numbers, long long nr) {
	int sum;
	int first=0;
	int last=0;
	for (int i = 0; i < numbers.size(); i++) { // start
		sum = 0;
		for (int j = i; j < numbers.size(); j++) {
			sum += numbers.at(j);
			if ((sum == nr) && (j - i > last - first)) {
				first = i;
				last = j;
			} else if (sum > nr) {
				break;
			}
		}
	}
	return *min_element(numbers.begin() + first, numbers.begin() + last)
		+ *max_element(numbers.begin() + first, numbers.begin() + last);
}

int main() {
	vector<long long> input = getInput("input.txt");
	long long nr = oddNumber(input, 25);
	cout << "Part 1: " << nr << endl;
	cout << "Part 2: " << sumOfLimitsOfRangeThatSumsTo(input, nr) << endl;
	return 0;
}