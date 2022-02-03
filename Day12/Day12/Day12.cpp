#include <vector>
#include <string>
#include <fstream>
#include <regex>
#include <map>
#include <cmath>
#include <iostream>

struct Command {
	char action;
	int value;
	Command(char action_, int value_)
		:action{action_}, value{value_}
	{}
};

std::vector<Command> getCommands(std::string fileName) {
	std::vector<Command> commands = {};
	std::ifstream infile;
	infile.open(fileName);
	std::regex re("^([A-Z])(\\d+)$");
	std::smatch match;
	std::string line;
	while (!infile.eof()) {
		std::getline(infile, line);
		if (std::regex_search(line, match, re)) {
			commands.emplace_back(match.str(1).at(0), std::stoi(match.str(2)));
		}
	}
	infile.close();
	return commands;
}

enum class direction {
	EAST = 0, SOUTH = 1, WEST = 2, NORTH = 3
};

class Point {
	int x;
	int y;
};

class Ship {
	static std::map<direction, char> charToDirection;
	direction dir = direction::EAST;
	int x = 0; int y = 0; // starting position as the point of reference
	// movements: EAST -> x++; WEST -> x--	|	NORTH -> y++; SOUTH -< y--
public:
	// Part 1
	void processCommands(const std::vector<Command>& commands) {
		std::cout << "Position: " << "(" << this->x << ", " << this->y << ")\t|\t" <<
			"Facing direction: " << charToDirection.at(this->dir) << std::endl;
 		for (const Command& cmd : commands) {
			std::cout << "\t-> Action: " << cmd.action << " | " << "value: " << cmd.value << std::endl;
			turn(cmd);
			move(cmd);
			std::cout << "Position: " << "(" << this->x << ", " << this->y << ")\t|\t" <<
				"Facing direction: " << charToDirection.at(this->dir) << std::endl;
		}
	}
	void turn(const Command& cmd) {
		// performs the correct turn if command action equals 'R' or 'L', otherwise nothing changes
		int turn = 0;
		if (cmd.action == 'R') {
			turn = cmd.value / 90;
		}
		else if (cmd.action == 'L') {
			turn = 4 - (cmd.value / 90);
		}
		this->dir = static_cast<direction>((static_cast<int>(this->dir) + turn) % 4);
	}
	void move(const Command& cmd) {
		// if action not in (E, S, W, N, F) no changes are made
		if (cmd.action == 'F') {// to avoid spaghetti code: convert enum direction to char, as in commands and call again
			move(Command(charToDirection.at(this->dir), cmd.value));
		}
		switch (cmd.action) {
			case 'E':
				this->x += cmd.value;
				break;
			case 'S':
				this->y -= cmd.value;
				break;
			case 'W':
				this->x -= cmd.value;
				break;
			case 'N':
				this->y += cmd.value;
				break;
		}
	}
	int getManhattanDistance(int startX = 0, int startY = 0) {
		return std::abs(this->x - startX) + std::abs(this->y - startY);
	}
	// Part 2
	void moveTowardsWaypoint(const Waypoint& waypoint) {

	}
};

std::map<direction, char> Ship::charToDirection = { {direction::EAST, 'E'}, {direction::SOUTH, 'S'},
	{direction::WEST, 'W'}, {direction::NORTH, 'N'} };

class Waypoint {
	int x; // x > 0: EAST, x < 0: WEST
	int y; // y > 0: NORTH, y < 0: SOUTH
	Waypoint(int startX = 10, int startY = 1)
		:x{startX}, y{startY}
	{}
	void rotate(const Command& command) {
		int turn = 0; 
		if (cmd.action == 'R') {
			turn = cmd.value / 90;
		}
		else if (cmd.action == 'L') {
			turn = 4 - (cmd.value / 90); // turn a left rotation to a coresponding right rotation
		}
		while (turn-- > 0) {
			// a simple way to rotate thought up by me after a quick analysis
			// to rotate a pt by 90 degrees right: switch the coordinates and negate the second(y) coordinate
			int subst = this->x;
			this->x = this->y;
			this->y = -subst;
		}
	}
	
};

int main() {
	std::vector<Command> commands = getCommands("input.txt");
	Ship ship;
	ship.processCommands(commands);
	std::cout << "Part 1: " << ship.getManhattanDistance() << std::endl;
	return 0;
}