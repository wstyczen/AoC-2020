#include <vector>
#include <fstream>
#include <map>
#include <string>
#include <regex>
#include <sstream>
#include <iostream>
#include <algorithm>

struct Range {
	int low;
	int high;
	Range(int low_, int high_)
		: low{ low_ }, high{high_}
	{}
};

class Input {
	const std::map<std::string, std::vector<Range>> validRanges;
	const std::vector<int> yourTicket;
	std::vector<std::vector<int>> tickets;
public:
	Input(const std::map<std::string, std::vector<Range>>& validRanges_,
	const std::vector<int>& yourTicket_, const std::vector<std::vector<int>>& tickets_)
		:validRanges{validRanges_}, yourTicket{yourTicket_}, tickets{tickets_}
	{}
	//p1
	bool isValid(const int& nr) {
		for (auto pair : validRanges) {
			for (const Range& range : pair.second) {
				if (nr >= range.low && nr <= range.high) {
					return true;
				}
			}
		}
		return false;
	}
	int getInvalidValue(const std::vector<int>& ticket) {
		int invalidValue = 0;
		for (int nr : ticket) {
			if (!isValid(nr))
				invalidValue += nr;
		}
		return invalidValue;
	}
	int getTicketScanningErrorRate() {
		int TSER = 0;
		for (const std::vector<int>& ticket : tickets) {
			TSER += getInvalidValue(ticket);
		}
		return TSER;
	}
	//p2
	void discardInvalidTickets() {
		std::vector<std::vector<int>> validTickets = {};
		for (auto ticket : tickets) {
			if (!getInvalidValue(ticket)) {
				validTickets.push_back(ticket);
			}
		}
		tickets = validTickets;
	}
	bool isInRanges(const int& nr, const std::vector<Range>& ranges){
		for (const Range& range : ranges) {
			if (nr >= range.low && nr <= range.high) {
				return true;
			}
		}
		return false;
	}
	std::map<int, std::vector<std::string>> getViable() {
		discardInvalidTickets(); // discard all invalid tickets
		std::map<int, std::vector<std::string>> viable = {};
		for (int i = 0; i < yourTicket.size(); i++) {
			viable[i+1] = {};
			for (auto pair : validRanges) {
				if (isInRanges(yourTicket.at(i), pair.second)) {
					viable[i+1].push_back(pair.first);
				}
			}
		}
		for (auto ticket : tickets) {
			for (int i = 0; i < ticket.size(); i++) {
				std::vector<std::string> newViable = {};
				for (std::string s : viable[i+1]) {
					if (isInRanges(ticket.at(i), validRanges.at(s))) {
						newViable.push_back(s);
					}
				}
				viable[i+1] = newViable;
			}
		}
		return viable;
	}
	std::map<int, std::string> getFinal() {
		std::map<int, std::vector<std::string>> viable = getViable();
		std::map<int, std::string> final_ = {};
		int size = viable.size();
		while (final_.size() < size) {
			std::vector<std::string> newFinal = {};
			auto it = viable.begin();
			while (it != viable.end()) {
				if (it->second.size() == 1) {
					final_[it->first] = it->second.at(0);
					newFinal.push_back(it->second.at(0));
					viable.erase(it++);
				} else it++;
			}
			for (auto &pair : viable) {
				for (std::string s : newFinal) {
					auto it = std::find(pair.second.begin(), pair.second.end(), s);
					if (it != pair.second.end()) {
						pair.second.erase(it);
					}
				}
			}
		}
		return final_;
	}
	long long getDepartureMult(std::string word="departure") {
		std::map<int, std::string> final_ = getFinal();
		long long mult = 1;
		for (auto pair : final_) {
			if (pair.second.find(word) != -1) {
				mult *= yourTicket.at(pair.first - 1);
			}
		}
		return mult;
	}
};

Input getInput(std::string fileName) {
	std::map<std::string, std::vector<Range>> validRanges{};
	std::vector<int> yourTicket = {};
	std::vector<std::vector<int>> tickets = {};

	std::ifstream infile;
	infile.open(fileName);
	std::string line;
	std::smatch match;
	std::regex rangePattern("(\\d+)-(\\d+) or (\\d+)-(\\d+)");

	while (!infile.eof()) {
		std::getline(infile, line);
		if (line.empty()) continue;
		else if (line.find("or") != -1) {
			std::string key = line.substr(0, line.find(":"));
			if (std::regex_search(line, match, rangePattern)) {
				validRanges[key] = std::vector<Range>({Range(std::stoi(match.str(1)), std::stoi(match.str(2))),
					Range(std::stoi(match.str(3)), std::stoi(match.str(4)))});
			}
		} else if (line.find(",") != -1) {
			std::vector<int> ticket = {};
			std::string nr;
			std::stringstream ticketStream(line);
			while (std::getline(ticketStream, nr, ',')) {
				ticket.push_back(std::stoi(nr));
			}
			if (yourTicket.empty()) {
				yourTicket = ticket;
			} else {
				tickets.push_back(ticket);
			}
		}
	}
	infile.close();
	return Input(validRanges, yourTicket, tickets);
}

int main() {
	Input input = getInput("input.txt");
	std::cout << "Part 1:\t" << input.getTicketScanningErrorRate() << std::endl;
	std::cout << "Part 2:\t" << input.getDepartureMult() << std::endl;
	return 0;
}