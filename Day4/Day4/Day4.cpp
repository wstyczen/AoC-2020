#include <string>
#include <fstream>
#include <vector>
#include <regex>
#include <map>
#include <iostream>
#include <algorithm>
using namespace std;

struct Data {
	string key;
	string value;
	Data(string key_, string val_)
		: key{key_}, value{val_}
	{}
};

bool hasAllFields(const vector<Data>& data) {
	map<string, bool> keys = { {"byr",false}, {"iyr", false}, {"eyr", false}, {"hgt", false}, {"hcl", false}, {"ecl", false}, {"pid", false} };
	for (auto it = data.begin(); it != data.end(); it++) {
		keys[(*it).key] = true;
	}
	return all_of(keys.begin(), keys.end(), [](auto pair) {return pair.second; });
}

bool validate(const vector<Data>& data) {
	if (!hasAllFields(data)) return false;
	for (auto it = data.begin(); it != data.end(); it++) {
		string key = (*it).key;
		string val = (*it).value;
		if (key == "byr") {
			if (stoi(val) < 1920 || stoi(val) > 2002) return false;
		} else if (key == "iyr") {
			if (stoi(val) < 2010 || stoi(val) > 2020) return false;
		} else if (key == "eyr") {
			if (stoi(val) < 2020 || stoi(val) > 2030) return false;
		} else if (key == "hgt") {
			smatch match;
			regex pattern{ "(\\d+)(cm|in)" };
			if (regex_search(val, match, pattern)) {
				int nr = stoi(match.str(1));
				if (match.str(2) == "cm") {
					if (nr > 193 || nr < 150) return false;
				}
				else if (match.str(2) == "in") {
					if (nr > 76 || nr < 59) return false;
				}
			} else {
				return false;
			}
			return true;
		} else if (key == "hcl") {
			smatch match;
			regex pattern{ "^#[a-f0-9]{6}$" };
			if (!regex_search(val, match, pattern)) return false;
		} else if (key == "ecl") {
			vector<string> valid = { "amb", "blu", "brn", "gry", "grn", "hzl", "oth" };
			if (!(find(valid.begin(), valid.end(), val) != valid.end())) return false;
		} else if (key == "pid") {
			smatch match;
			regex pattern{ "^[0-9]{9}$" };
			if (!regex_search(val, match, pattern)) return false;
		}
	}
	return true;
}

int countValid(string fileName, bool (*isValid)(const vector<Data>&)) {
	int validCount = 0;
	ifstream infile;
	infile.open(fileName);
	string line;
	vector<Data> data = {};

	smatch match;
	regex pattern{ "([a-z]+):([^\\s]+)" }; // ^ - not; \s - a whitespace 

	while (!infile.eof()) {
		getline(infile, line);
		if (line.empty()) { // NEEDS AN EMPTY LINE AT THE END OF THE LINE FOR THE LAST ENTRY TO BE PROCESSED
			if (!data.empty()) {
				if (isValid(data)) {
					validCount++;
				}
				data.clear();
			}
		} else {
			for (sregex_iterator it = sregex_iterator(line.begin(), line.end(), pattern); it != sregex_iterator(); it++) {
				match = *it;
				data.push_back(Data(match.str(1), match.str(2)));
			}
		}
	}
	infile.close();
	return validCount;
}

int main() {
	cout << "Part 1: " << countValid("input.txt", *hasAllFields) << endl;
	cout << "Part 2: " << countValid("input.txt", *validate) << endl;
	return 0;
}