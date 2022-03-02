#include <list>
#include <algorithm>
#include <string>
#include <iterator>
#include <iostream>
#include <sstream>

class Cups {
	friend std::ostream& operator<<(std::ostream& os, const Cups& cups);
	int moveCount = 0;
	std::list<int>::iterator current;
	std::list<int> cups;
public:
	Cups(std::list<int>& cups_)
	:cups{cups_}
	{
		current = cups.begin();
	}
	std::list<int> pickUp(int nr = 3) {
		std::list<int> pickedUp{};
		auto pos = std::next(current, 1);
		while (nr-- > 0) {
			if (pos == cups.end()) {
				pos = cups.begin();
			}
			auto next = std::next(pos, 1);
			pickedUp.insert(pickedUp.end(), *pos);
			cups.erase(pos);
			pos = next;
		}
		return pickedUp;
	}
	std::list<int>::iterator getNext(int value) {
		while (--value >= 0) {
			if (value == 0) value = 9;
			auto pos = std::find(cups.begin(), cups.end(), value);
			if (pos != cups.end()) return pos;
		}
		return cups.end();
	}
	void move() {
		moveCount++;
		std::list<int> picked = pickUp();
		std::list<int>::iterator destination = getNext(*current);
		cups.insert(std::next(destination, 1), picked.begin(), picked.end());
		current = (std::next(current, 1) != cups.end()) ? (std::next(current, 1)) : cups.begin();
	}
	void move(int times) {
		while (times-- > 0) {
			move();
		}
	}
	int getLabels() {
		std::stringstream stream{};
		auto one = std::find(cups.begin(), cups.end(), 1);
		auto iter = one;
		while (std::next(iter, 1) != one) {
			iter = (std::next(iter, 1) != cups.end()) ? std::next(iter, 1) : cups.begin();
			if (*iter != 1) stream << *iter;
		}
		return std::stoi(stream.str());
	}
	// Part 2
	void addCups(const int upperLimit) {
		int cupIndex = 9;
		while (cupIndex++ < upperLimit) {
			cups.push_back(cupIndex);
		}
	}
	long part2(const int upperLimit = 1000000, const int moves = 10000000) {
		if (cups.size() < upperLimit) addCups(upperLimit);
		move(moves);
		auto index = std::find(cups.begin(), cups.end(), 1);
		return (*std::next(index, 1)) * (*std::next(index, 2));
	}
};

std::ostream& operator<<(std::ostream& os, const Cups& cups) {
	os << "-- move " << cups.moveCount << " --\n";
	os << "cups:\t";
	for (auto cupIter = cups.cups.begin(); cupIter != cups.cups.end(); cupIter++) {
		if (cupIter == cups.current) os << "(" << *cupIter << ")";
		else os << *cupIter;
		os << " ";
	}
	os << std::endl;
	return os;
}

Cups getInput(const int input = 398254716) {
	std::string inputStr = std::to_string(input);
	std::list<int> cups{};
	std::for_each(inputStr.begin(), inputStr.end(), [&cups](char c) {cups.push_back((int)(c - '0')); });
	return Cups(cups);
}

int main() {
	Cups cups = getInput();

	//cups.move(100);
	//std::cout << "Part 1: " << cups.getLabels() << std::endl;

	cups = getInput();
	std::cout << "Part 2: " << cups.part2() << std::endl;

	return 0;
}