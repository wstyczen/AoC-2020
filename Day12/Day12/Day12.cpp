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
protected:
	int x; // x > 0: EAST, x < 0: WEST
	int y; // y > 0: NORTH, y < 0: SOUTH
public:
	Point(int x_, int y_)
		: x{ x_ }, y{ y_ }
	{ }
	void move(const Command& cmd) {
		// if action not in (E, S, W, N) no changes are made
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
	virtual void rotate(const Command& cmd) = 0;
	int getX() const {
		return this->x;
	}
	int getY() const {
		return this->y;
	}
};

class Ship : public Point {
	static std::map<direction, char> charToDirection;
	direction dir = direction::EAST;
	// starting position as the point of reference

public:
	Ship(int x_=0, int y_=0)
		:Point(x_, y_)
	{}
	// Part 1
	virtual void rotate(const Command& cmd) override {
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
			Point::move(Command(charToDirection.at(this->dir), cmd.value));
		} else Point::move(cmd);
	}
	void processCommands(const std::vector<Command>& commands) {
		/*std::cout << "Position: " << "(" << this->x << ", " << this->y << ")\t|\t" <<
			"Facing direction: " << charToDirection.at(this->dir) << std::endl;*/
		for (const Command& cmd : commands) {
			//std::cout << "\t-> Action: " << cmd.action << " | " << "value: " << cmd.value << std::endl;
			rotate(cmd);
			move(cmd);
			//std::cout << "Position: " << "(" << this->x << ", " << this->y << ")\t|\t" <<
			//	"Facing direction: " << charToDirection.at(this->dir) << std::endl;
		}
	}
	// Part 2
	void moveTowards(const Point& pt, int times) {
		this->x += pt.getX() * times;
		this->y += pt.getY() * times;
	}
	// both
	int getManhattanDistance(int startX = 0, int startY = 0) {
		return std::abs(this->x - startX) + std::abs(this->y - startY);
	}
};

std::map<direction, char> Ship::charToDirection = { {direction::EAST, 'E'}, {direction::SOUTH, 'S'},
	{direction::WEST, 'W'}, {direction::NORTH, 'N'} };

class Waypoint : public Point {
public:
	Waypoint(int startX = 10, int startY = 1)
		: Point(startX, startY)
	{}
	virtual void rotate(const Command& cmd) override {
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

int part1(const std::vector<Command>& commands) {
	Ship ship;
	ship.processCommands(commands);
	return ship.getManhattanDistance();
}

int part2(const std::vector<Command>& commands) {
	Ship ship;
	Waypoint wpt;
	for (const Command& cmd : commands) {
		if (cmd.action == 'F') {
			ship.moveTowards(wpt, cmd.value);
		}
		else if (cmd.action == 'R' || cmd.action == 'L') {
			wpt.rotate(cmd);
		}
		else { // move (E, S, W, N)
			wpt.move(cmd);
		}
	}
	return ship.getManhattanDistance();
}

int main() {
	std::vector<Command> commands = getCommands("input.txt");
	
	std::cout << "Part 1: " << part1(commands) << std::endl;
	std::cout << "Part 2: " << part2(commands) << std::endl;

	return 0;
}