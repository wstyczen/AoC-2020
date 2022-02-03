#include <iostream>
#include <regex>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <math.h>
#include <iterator>

class AirportTravel {
private:
	int departTimestamp;
	std::map<int, int> busIdsPositions;
	std::vector<std::string> allIds;
public:
	AirportTravel(int departTime_, std::vector<std::string> allIds_)
		:departTimestamp{ departTime_ }, allIds{ allIds_ }
	{
		this->generatePositions();
	};

	bool isNumber(const std::string& str)
	{
		for (char const& c : str) {
			if (std::isdigit(c) == 0) return false;
		}
		return true;
	}

	void generatePositions() {
		int i = 0;
		for (std::string id : allIds) {
			if (this->isNumber(id)) {
				this->busIdsPositions.emplace(std::stoi(id), i);
			}
			i++;
		}
	}

	int getPart1() {
		int chosenId = -1;
		int shortestWait = 0;
		for (const auto& myPair : this->busIdsPositions) {
			int id = myPair.first;
			int wait = ceil((double)this->departTimestamp / id) * id - this->departTimestamp;
			if (chosenId == -1 || wait < shortestWait) {
				chosenId = id;
				shortestWait = wait;
			}
		}
		return chosenId * shortestWait;
	}

	bool checkForPart2(int t) {
		for (auto it = this->busIdsPositions.begin(); it != this->busIdsPositions.end(); ++it) {
			int wait = ceil((double)t / it->first) * it->first - t;
			if (wait != it->second) {
				return false;
			}
		}
		return true;
	}

	int getPart2() {
		bool found = false;
		long t = 1068779;
		while (true) {
			if (checkForPart2(t))
				break;
			t++;
		}
		return t;
	}
};

static AirportTravel getInput(std::string fileName) {
	int departTimestamp = -1;
	std::vector<std::string> allIds = {};
	std::string delimiter = ",";
	std::string line;
	std::ifstream inputStream;
	inputStream.open(fileName);
	if (inputStream.is_open()) {
		while (!inputStream.eof()) {
			std::getline(inputStream, line, '\n');
			if (departTimestamp == -1) {
				departTimestamp = stoi(line);
			}
			else {
				std::string token;
				int tokenIndex = 0;
				while ((tokenIndex = line.find(delimiter)) != -1) {
					token = line.substr(0, tokenIndex);
					line.erase(0, tokenIndex + delimiter.length());
					allIds.push_back(token);
				}
				allIds.push_back(line);
			}
		}
		inputStream.close();
	}

	return AirportTravel(departTimestamp, allIds);
}

int main() {
	//AirportTravel ap1 = getInputP1("Day13-test.txt");
	//cout << ap1.getPart1() << endl;
	AirportTravel ap2 = getInput("Day13-test2.txt");
	std::cout << ap2.getPart1() << std::endl;
	std::cout << ap2.getPart2() << std::endl;
	return 0;
}

