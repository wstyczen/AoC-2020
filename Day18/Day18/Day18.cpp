#include <string>
#include <vector>
#include <iterator>
#include <fstream>
#include <iostream>
#include <regex>
#include <algorithm>

void removeUnneededParenthesis(std::string& s) {
	std::regex additionPattern("\\((\\d+)\\)");
	std::smatch match;
	while (std::regex_search(s, match, additionPattern)) {
		s = match.prefix().str() + match.str(1) + match.suffix().str();
	}
}
bool resolveAdditions(std::string& s) {
	bool added = false;
	std::regex additionPattern("(\\d+) \\+ (\\d+)");
	std::smatch match;
	while (std::regex_search(s, match, additionPattern)) {
		added = true;
		long long sum = std::stoll(match.str(1)) + std::stoll(match.str(2));
		s = match.prefix().str() + std::to_string(sum) + match.suffix().str();
		removeUnneededParenthesis(s);
	}
	return added;
}
bool resolveMultiplications(std::string& s) {
	bool multiplied = false;
	std::regex multiplicationPattern("(\\d+) \\* (\\d+)");
	std::smatch match;
	while (std::regex_search(s, match, multiplicationPattern)) {
		long long sum = std::stoll(match.str(1)) * std::stoll(match.str(2));
		s = match.prefix().str() + std::to_string(sum) + match.suffix().str();
		multiplied = true;
	}
	return multiplied;
}
bool resolveParenthesis(std::string& s) {
	bool found = false;
	std::regex pattern("\\(([^()]+)\\)"); // parenthesis that does not contain other parenthesis'
	std::smatch match;
	while (std::regex_search(s, match, pattern)) {
		found = true;
		std::string paren = match.str(1);
		resolveAdditions(paren);
		resolveMultiplications(paren);
		s = match.prefix().str() + paren + match.suffix().str();
	}
	return found;
}

class Expression {
	std::string expr;
public:
	Expression(std::string s)
		:expr{s}
	{}
	std::string getExpr() {
		return expr;
	}
	// Part 1
	long long getResult() {
		int i = 0;
		return getResult(expr, i);
	}
	void performOper(long long& result, const int& val, const char& oper) {
		if (oper == '+') {
			result += val;
		}
		else if (oper == '*') {
			result *= val;
		}
	}
	long long getResult(std::string s, int& i) {
		long long result{};
		char oper = '+'; // when parenthesis is at the begining -> add to result
		while (i < s.size()) {
			char c = s.at(i);
			if (std::isspace(c)) {
				i++;
				continue;
			} else if (std::isdigit(c)) {
				int digit = c - '0';
				if (!result) result = digit;
				else {
					performOper(result, digit, oper);
				}
			} else if (c == '+' || c == '*') {
				oper = c;
			} else if (c == '(') {
				i += 1;
				long long value = getResult(s, i);
				performOper(result, value, oper);
			} else if (c == ')') {
				return result;
			}
			i++;
		}


		return result;
	}
	// Part 2
	long long getAdvancedResult() {
		bool finished = false;
		while (!finished) {
			finished = true;
			bool added = resolveAdditions(expr);
			bool foundPar = resolveParenthesis(expr);
			if (foundPar || added) finished = false;
			removeUnneededParenthesis(expr);
		}
		bool multiplied = resolveMultiplications(expr);
		return std::stoll(expr);
	}

};

long long getSumOfResults(const std::string& fileName, const int part=1) {
	long long sum = 0;
	std::ifstream inFile;
	inFile.open(fileName);
	std::string line;
	while (!inFile.eof()) {
		std::getline(inFile, line);
		Expression exp(line);
		long long res = (part==1) ? exp.getResult() : exp.getAdvancedResult();
		//std::cout << " -> '" << line << "' becomes " << res << std::endl;
		sum += res;
	}
	inFile.close();
	return sum;
}

int main() {
	std::cout << "Part 1: " << getSumOfResults("input.txt") << std::endl;
	std::cout << "Part 2: " << getSumOfResults("input.txt", 2) << std::endl;

	return 0;
}