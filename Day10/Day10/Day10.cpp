#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
using namespace std;

vector<int> getInput(string fileName) {
	vector<int> input = {};
	ifstream infile;
	infile.open(fileName);
	string line;
	while (infile.is_open() && !infile.eof()) {
		getline(infile, line);
		input.push_back(stoi(line));
	}
	infile.close();
	return input;
}

int part1(const vector<int>& sortedAdapters) {
	int jumps[3] = { 0, 0, 0 };
	int currentJoltage = 0;
	for (int nr : sortedAdapters) {
		if (nr == 0) continue;
		jumps[nr - currentJoltage-1]++;
		currentJoltage = nr;
	}
	return jumps[0] * jumps[2];
}

// adds 0 and the final adapter(max + 3) and sorts adapters
void processAdapters(vector<int>& adapters) {
	adapters.push_back(0);
	sort(adapters.begin(), adapters.end());
	adapters.push_back(adapters.at(adapters.size() - 1) + 3);
}

unsigned long long part2(const vector<int>& sortedAdapters, map<int, unsigned long long>& cache, int index=0) {
	if (index == sortedAdapters.size() - 1) {
		return 1;
	}
	if (cache.find(index) != cache.end()) {
		return cache[index];
	}
	unsigned long long total = 0;
	for (int i = index+1; i <= index + 3 && i < sortedAdapters.size(); i++) {
		if (sortedAdapters.at(i) <= sortedAdapters.at(index) + 3) {
			total += part2(sortedAdapters, cache, i);
		}
	}
	cache[index] = total;
	return total;
}

int main() {
	vector<int> adapters = getInput("input.txt");
	processAdapters(adapters); // The functions below operate on the assumption that the adapters vector is sorted, with added elements;
	cout << "Part 1: " << part1(adapters) << endl;
	map<int, unsigned long long> cache = {};
	cout << "Part 2: " << part2(adapters, cache) << endl;
	return 0;
}