#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>
#include <regex>
#include <iostream>

class Rules {
	std::unordered_map<int, char> basicRules = {};
	std::unordered_map<int, std::vector<std::vector<int>>> complexRules = {};
	std::vector<std::string> messages;
	std::unordered_map<int, std::vector<std::string>> possibleMatches;
public:
	Rules(std::unordered_map<int, char>& basicRules_, std::unordered_map<int, std::vector<std::vector<int>>>& complexRules_,
		std::vector<std::string>& messages_)
		: basicRules{ basicRules_ }, complexRules{ complexRules_ }, messages{messages_}
	{
		generateMatches();
	}
	void generateMatches() {
		for (const auto& pair : complexRules) {
			generateMatches(pair.first);
		}
	}
	void generateMatches(const int& ruleKey) {
		if (possibleMatches.find(ruleKey) != possibleMatches.end()) return;
		std::vector<std::string> matches{};
		if (basicRules.find(ruleKey) != basicRules.end()) {
			std::string match(1, basicRules[ruleKey]);
			matches.push_back(match);
		} else {
			for (const std::vector<int>& setOfRules : complexRules[ruleKey]) {
				std::vector<std::string> setOfMatches = { "" };
				for (int ruleNr : setOfRules) {
					if (possibleMatches.find(ruleNr) == possibleMatches.end()) {
						generateMatches(ruleNr);
					}
					std::vector<std::string> newMatches = {};
					for (std::string match : setOfMatches) {
						for (std::string suffix : possibleMatches[ruleNr]) {
							newMatches.push_back(match + suffix);
						}
					}
					setOfMatches = newMatches;
				}
				matches.insert(std::end(matches), std::begin(setOfMatches), std::end(setOfMatches));
			}
		}

		std::cout << "-> Key: '" << ruleKey << "' matches:\t";
		for (const std::string& match : matches) {
			std::cout << "'" << match << "' | ";
		}
		std::cout << std::endl;
		possibleMatches[ruleKey] = matches;
	}
	void printPossibleMatches() {
		for (const auto& pair : possibleMatches) {
			std::cout << "-> Key: '" << pair.first << "' matches:\t";
			for (const std::string& match : pair.second) {
				std::cout << "'" << match << "' | ";
			}
			std::cout << std::endl;
		}
	}
	int howManyMatchRule(const int ruleNr=0) {
		int matching = 0;
		for (const std::string& message : messages) {
			for (const std::string& match : possibleMatches[ruleNr]) {
				if (message == match) {
					matching++;
					break;
				}
			}
		}
		return matching;
	}
};

Rules getRules(const std::string& fileName) {
	std::unordered_map<int, char> basicRules = {};
	std::unordered_map<int, std::vector<std::vector<int>>> complexRules = {};
	
	std::vector<std::string> messages{};

	std::smatch match;
	std::regex keyPattern("(\\d+):");
	std::regex basicValuePattern("\"([a-z])\"");
	std::regex noSpaceGroupPattern("([^ ]+)");
	std::ifstream inFile;
	inFile.open(fileName);
	int key{};
	char basic{};
	std::string line;
	while (!inFile.eof()) {
		std::getline(inFile, line);
		if (std::regex_search(line, match, keyPattern)) {
			key = std::stoi(match.str(1));
			if (std::regex_search(line, match, basicValuePattern)) {
				basic = match.str(1)[0];
				basicRules[key] = basic;
			} else {
				std::vector<std::vector<int>> subRules{};
				std::vector<int> subRulesGroup = {};
				std::string s = match.suffix().str();
				while (std::regex_search(s, match, noSpaceGroupPattern)) {
					if (match.str(1) != "|") {
						subRulesGroup.push_back(std::stoi(match.str(1)));
					} else {
						subRules.push_back(subRulesGroup);
						subRulesGroup = {};
					}
					s = match.suffix().str();
				}
				subRules.push_back(subRulesGroup);
				complexRules[key] = subRules;
			}
		}
		else if (!line.empty()) {
			messages.push_back(line);
		}
	}
	inFile.close();
	return Rules(basicRules, complexRules, messages);
}

int main() {
	Rules rules = getRules("input.txt");
	//rules.generateMatches();
	//rules.printPossibleMatches();
	//std::cout << "Part 1:\t" << rules.howManyMatchRule() << std::endl;

	return 0;
}