#include <fstream>
#include <regex>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

struct Operation {
	static long accumulator;
	string code;
	int value;
	bool unrun = true;
	Operation(string code_, int value_)
		:code{ code_ }, value{ value_ }
	{}
	int run(int index, bool& firstLoop) {
		if (!unrun) {
			firstLoop = false;
		}
		else {
			this->unrun = false;
			if (code == "jmp") {
				return index + value;
			}
			else if (code == "acc") {
				accumulator += value;
			}
		}
		return index + 1;
	}
	void changeToUnrun() {
		this->unrun = true;
	}
	static void resetAccumulator() {
		accumulator = 0;
	}
	bool swapCode() {
		if (code == "nop") {
			code = "jmp";
			return true;
		}
		else if (code == "jmp") {
			code = "nop";
			return true;
		}
		return false;
	}
};

long Operation::accumulator = 0;

vector<Operation> getInput(string fileName) {
	vector<Operation> input = {};
	ifstream infile;
	infile.open(fileName);
	string line;
	regex pattern("([a-z]+?) ([+|-]\\d+)");
	smatch match;
	while (!infile.eof()) {
		getline(infile, line);
		if (regex_search(line, match, pattern)) {
			input.emplace_back(match.str(1), stoi(match.str(2)));
		}
	}
	infile.close();
	return input;
}

int run(vector<Operation>& operations, int index = 0) {
	bool firstLoop = true;
	while (firstLoop) {
		index = operations.at(index).run(index, firstLoop);
		if (index < 0 || index >= operations.size()) break; // terminate
	}
	return Operation::accumulator;
}

int run(vector<Operation>& operations, bool& firstLoop, int index = 0) {
	while (firstLoop) {
		index = operations.at(index).run(index, firstLoop);
		if (index < 0 || index >= operations.size())
			break; // terminate 
	}
	return Operation::accumulator;
}

void changeAllToUnrun(vector<Operation>& operations) {
	for (Operation& op : operations) {
		op.changeToUnrun();
	}
}

int runFixed(vector<Operation>& operations) {
	int accumulator;
	bool firstLoop = true;
	for (Operation& op : operations) {
		if (!op.swapCode()) continue; // not a jmp or nop operation(acc)
		accumulator = run(operations, firstLoop);
		if (firstLoop) { // found fixed - finished without entering a second loop
			return accumulator;
		} else { // reset
			op.swapCode(); // swap back - try something else
			firstLoop = true;
			Operation::resetAccumulator(); // reset accumulator back to 0
			changeAllToUnrun(operations); // have to reset everything to unrun to test another swap
		}
	}
	return -1; // no swap fixes the infinite loop
}

int main() {
	vector<Operation> input = getInput("input.txt");
	cout << "Part 1: " << run(input) << endl;
	cout << "Part 2: " << runFixed(input) << endl;
	return 0;
}