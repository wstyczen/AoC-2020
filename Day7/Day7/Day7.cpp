#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <map>
#include <iostream>
#include <set>
#include <deque>
#include <algorithm>
using namespace std;

struct Bag {
	string color;
	int nr = 1;
	Bag(string cl, int nr_)
		:color{cl}, nr{nr_}
	{}
};

ostream& operator<<(ostream& os, const Bag& bag) {
	os << bag.nr << " " << bag.color;
	return os;
}

// returns a map of pairs: <key = (string) name of the bag, value: vector<Bag> - bags it contains>
map<string, vector<Bag>> getInput(string fileName) {
	map<string, vector<Bag>> input = {};
	ifstream infile;
	infile.open(fileName);
	string line;
	string name; string containsStr;
	smatch match;
	regex outerPattern{ "([a-z ]+?) bags contain (.+)" };
	regex containsPattern{ "(\\d+) ([a-z ]+?) bag" };
	while (!infile.eof()) {
		getline(infile, line);
		if (regex_search(line, match, outerPattern)) {
			name = match.str(1);
			containsStr = match.str(2);
		}
		vector<Bag> containsVec = {};
		while (regex_search(containsStr, match, containsPattern)) {
			containsVec.push_back(Bag(match.str(2), stoi(match.str(1))));
			containsStr = match.suffix();
		}
		input.emplace(make_pair(name, containsVec));
	}
	infile.close();
	return input;
}

void printInput(const map<string, vector<Bag>>& input) {
	for (auto const& pair : input) {
		cout << pair.first;
		if (pair.second.empty()) cout << " is empty." << endl;
		else cout << " contains:\n";
		for (Bag const& bag : pair.second) {
			cout << "\t -> " << bag << endl;
		}
		cout << endl;
	}
}

int howManyBagsCanHold(const map<string, vector<Bag>>& input, const string& reqColor="shiny gold") {
	set<string> canHold = {};
	deque<string> unchecked = {};
	// add all bags containing the color in question diretly to the 'unchecked' deque
	for (const auto& pair : input) {
		for (const Bag& bag : pair.second) {
			if (bag.color == reqColor) {
				unchecked.push_back(pair.first);
				break;
			}
		}
	}
	while (!unchecked.empty()) {
		string cl = unchecked.front();
		unchecked.pop_front(); // !!! pop doesn't return the element in c++
		if (find(canHold.begin(), canHold.end(), cl) != canHold.end()) continue;
		canHold.insert(cl);
		for (const auto& pair : input) {
			for (const Bag& bag : pair.second) {
				if (bag.color == cl) {
					unchecked.push_back(pair.first);
					break;
				}
			}
		}
	}
	return canHold.size();
}

int howManyBagsContains(const map<string, vector<Bag>>& input, const string& color="shiny gold") {
	int contains = 0;
	for (const Bag& bag : input.at(color)) {
		// it contains the bags themselves and any inside
		contains += bag.nr + bag.nr * howManyBagsContains(input, bag.color);
	}
	return contains;
}

int main() {
	map<string, vector<Bag>> input = getInput("input.txt");
	//printInput(input);
	// for part 1 - it would be much more optimal to reverse the map: bag -> bags containing that bag
	// as is, it is pretty inefficient - this is much better for part 2 though
	cout << "Part 1: " << howManyBagsCanHold(input) << endl;
	cout << "Part 2: " << howManyBagsContains(input) << endl;
	return 0;
}