#include <map>
#include <string>
#include <iterator>
#include <bitset>
#include <fstream>
#include <regex>
#include <iostream>

class Program {
	std::map<int, int> bitmask = {};
	std::map<long long, long long> overwrittenMemory = {};
public:
	Program() {}
	void setBitmask(const std::string& bitmaskStr) {
		this->bitmask.clear();
		for (auto it = bitmaskStr.begin(); it != bitmaskStr.end(); it++) {
			if (*it != 'X') {
				int index = it - bitmaskStr.begin();
				int bit = *it - 48;
				this->bitmask[index] = bit;
			}
		}
	}
	void writeToMemoryP1(const long long& memoryAddress, const long long& value) {
		std::string valueStr = std::bitset<36>(value).to_string();//convert value to string of bits
		for (auto it = bitmask.begin(); it != bitmask.end(); it++) {//apply bitmask
			valueStr[(*it).first] = std::to_string((*it).second)[0];
		}
		//long newValue = std::stol(valueStr, 0, 2);
		long long newValue = std::bitset<36>(valueStr).to_ullong();
		this->overwrittenMemory[memoryAddress] = newValue;
	}
	long long getSumOfAllAddresses() {
		long long sum = 0;
		for (auto it = overwrittenMemory.begin(); it != overwrittenMemory.end(); it++) {
			sum += (*it).second;
		}
		return sum;
	}

	void writeToMemoryP2(const long long& memoryAddress, const long long & value) {
		std::string addressStr = std::bitset<36>(memoryAddress).to_string();//convert value to string of bits
		std::string bitmaskStr(36, 'X');
		for (auto it = bitmask.begin(); it != bitmask.end(); it++) {//apply bitmask
			bitmaskStr[(*it).first] = std::to_string((*it).second)[0];
		}
		writeToMemoryP2(bitmaskStr, addressStr, value);
	}
	void writeToMemoryP2(const std::string& bitmaskStr, const std::string& addressStr, const long long& value, std::string currentStr="") {
		auto it = bitmaskStr.begin();
		if (currentStr.length() > 0) std::advance(it, currentStr.length());
		for (it; it != bitmaskStr.end(); it++) {//apply bitmask
			if ((*it) == 'X') {
				this->writeToMemoryP2(bitmaskStr, addressStr, value, currentStr + "1");
				this->writeToMemoryP2(bitmaskStr, addressStr, value, currentStr + "0");
				break;
			} else if ((*it) == '0') {
				currentStr += addressStr[currentStr.length()];
			} else if ((*it) == '1') {
				currentStr += "1";
			}
		}
		if (currentStr.length() == 36) {
			overwrittenMemory[std::bitset<36>(currentStr).to_ullong()] = value;
		}
	}
};

long long getResult(const std::string& fileName, const int& part=1) {
	Program program;
	std::ifstream infile;
	infile.open(fileName);
	std::string line;
	std::regex maskPattern("^mask = ((X|\\d)+)$");
	std::regex memPattern("^mem\\[(\\d+)\\] = (\\d+)$");
	std::smatch match;
	while (!infile.eof()) {
		std::getline(infile, line);
		if (std::regex_search(line, match, maskPattern)) {
			program.setBitmask(match.str(1));
		} else if (std::regex_search(line, match, memPattern)) {
			if (part == 1) {
				program.writeToMemoryP1(std::stoll(match.str(1)), std::stoll(match.str(2)));
			}
			else if (part == 2) {
				program.writeToMemoryP2(std::stoll(match.str(1)), std::stoll(match.str(2)));
			}
		}
	}
	infile.close();
	return program.getSumOfAllAddresses();
}

int main() {
	std::string fileName = "input.txt";
	std::cout << "Part 1: " << getResult(fileName) << std::endl;
	std::cout << "Part 2: " << getResult(fileName, 2) << std::endl;
	return 0;
}