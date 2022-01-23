#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <regex>
using namespace std;

struct Requirement {
	char character;
	int lower;
	int higher;
	Requirement(char c, int low, int high)
		:character{c}, lower{low}, higher{high}
	{}
};
ostream& operator<<(ostream& os, const Requirement& req)
{
	os << "Requirements: " << req.character << " | " << req.lower << "-" << req.higher;
	return os;
}

struct Password {
	Requirement requirement;
	string password;
	Password(char c, int low, int high, string pass)
		: requirement{Requirement(c, low, high)}, password{pass}
	{}
};
ostream& operator<<(ostream& os, const Password& password)
{
	os << "Password: '" << password.password << "', " << password.requirement;
	return os;
}

bool isValidPart1(const Password& password) {
	int count = 0;
	for (char c : password.password) {
		if (c == password.requirement.character) {
			count++;
		}
	}
	return (count >= password.requirement.lower && count <= password.requirement.higher);
}
bool isValidPart2(const Password& password) {
	int count{};
	char first = password.password[password.requirement.lower - 1];
	char second = password.password[password.requirement.higher - 1];
	char key = password.requirement.character;
	return (first == key && second != key) || (first != key && second == key);
}

vector<Password> getInput(string fileName) {
	vector<Password> input = {};
	ifstream file;
	file.open(fileName);
	string line;
	while (!file.eof()) {
		getline(file, line);
		regex reg{ "(\\d+)-(\\d+) ([a-z]): ([a-z]+)" };
		smatch match;
		if (regex_search(line, match, reg)) {
			input.push_back(Password(match.str(3)[0], stoi(match.str(1)), stoi(match.str(2)), match.str(4)));
		}
	}
	file.close();
	return input;
}
void printInput(const vector<Password>& input) {
	for (auto password : input) {
		cout << password << endl;
	}
}

int getValidCount(const vector<Password>& passwords, bool (*isValid)(const Password&)) {
	int validCount = 0;
	for (auto it = passwords.begin(); it != passwords.end(); it++) {
		Password password = *it;
		if (isValid(password))
			validCount++;
	}
	return validCount;
}

int main() {
	vector<Password> input = getInput("input.txt");
	//printInput(input);
	cout << "Part 1: " << getValidCount(input, &isValidPart1) << endl;
	cout << "Part 2: " << getValidCount(input, &isValidPart2) << endl;
	return 0;
}