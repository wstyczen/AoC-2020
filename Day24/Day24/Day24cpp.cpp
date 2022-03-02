#include <string>
#include <queue>
#include <fstream>
#include <unordered_map>
#include <set>
#include <iostream>
#include <chrono>

struct Point {
	const static std::unordered_map<std::string, Point> moveMap;
	int x; // E(+2), NE\SE(+1), W(-2), NW\SW(-1)
	int y; // NW\NE(+1), SW/SE(-1)
	Point(int x, int y)
		:x{ x }, y{ y }
	{}
	Point(const std::string& directions)
		:Point(0, 0)
	{
		move(directions);
	}
	Point() = default;
	void operator+=(const Point& rhs) {
		x += rhs.x;
		y += rhs.y;
	}
	Point operator-(const Point& rhs) const {
		return Point(x - rhs.x, y - rhs.y);
	}
	Point operator+(const Point& rhs) const {
		return Point(x + rhs.x, y + rhs.y);
	}
	void move(const std::string s) {
		std::string direction{};
		for (const char c : s) {
			direction += std::toupper(c);
			auto iter = moveMap.find(direction);
			if (iter != moveMap.end()) {
				*this += (*iter).second;
				direction.clear();
			}
		}
	}
	int howManyAdjacent(const std::set<Point>& pts) const {
		int adjacent = 0;
		std::for_each(pts.begin(), pts.end(), [&adjacent, this](const Point& pt) {
			Point diff = *this - pt;
			if (std::find_if(moveMap.begin(), moveMap.end(), [&diff](const auto& pair) {return pair.second == diff; }) != moveMap.end())
				adjacent++;
			});
		return adjacent;
	}
};
const std::unordered_map<std::string, Point> Point::moveMap = {
{"E", Point(2, 0)},
{"W", Point(-2, 0)},
{"NE", Point(1, 1)},
{"SW", Point(-1, -1)},
{"NW", Point(-1, 1)},
{"SE", Point(1, -1)},
};
bool operator<(const Point& lhs, const Point& rhs) {
	if (lhs.x < rhs.x)
		return true;
	else if (lhs.x > rhs.x)
		return false;
	else {
		if (lhs.y < rhs.y)
			return true;
	}
	return false;
}
bool operator==(const Point& lhs, const Point& rhs) {
	return (lhs.x == rhs.x && lhs.y == rhs.y);
}
std::ostream& operator<<(std::ostream& os, const Point& pt) {
	os << pt.x << ", " << pt.y;
	return os;
}

std::set<Point> getPoints(const std::string fileName) {
	std::set<Point> pts{};
	std::ifstream inFile;
	inFile.open(fileName);
	std::string line;
	while (!inFile.eof()) {
		std::getline(inFile, line);
		Point pt(line);
		auto it = std::find(pts.begin(), pts.end(), pt);
		if (it == pts.end()) pts.insert(pt);
		else pts.erase(pt);
	}
	inFile.close();
	return pts;
}

std::set<Point> getToBlack(const std::set<Point>& blackPts) {
	std::set<Point> toBlack{};
	for (const Point& pt : blackPts) {
		for (const auto& pair : Point::moveMap) {
			Point adjacentPt = pt + pair.second;
			if (adjacentPt.howManyAdjacent(blackPts) == 2) {
				toBlack.insert(adjacentPt);
			}
		}
	}
	return toBlack;
}
std::set<Point> getToWhite(const std::set<Point>& blackPts) {
	std::set<Point> toWhite{};
	for (const Point& pt : blackPts) {
		int adjacent = pt.howManyAdjacent(blackPts);
		if (adjacent == 0 || adjacent > 2) {
			toWhite.insert(pt);
		}
	}
	return toWhite;
}
void simulate(std::set<Point>& pts) {
	std::set<Point> toWhite = getToWhite(pts);
	std::set<Point> toBlack = getToBlack(pts);
	pts.insert(toBlack.begin(), toBlack.end());
	for (const Point& toWhitePt : toWhite) {
		auto it = pts.find(toWhitePt);
		if (it != pts.end()) {
			pts.erase(it);
		}
	}
	//pts.erase(toWhite.begin(), toWhite.end());
}
void simulate(std::set<Point>& pts, int days) {
	int day = 0;
	while (days-- > 0) {
		std::cout << "Day " << day << ":\t" << pts.size() << std::endl;
		day++;

		simulate(pts);
	}
	std::cout << "Day " << day << ":\t" << pts.size() << std::endl;
}

int main() {
	std::set<Point> blackPts = getPoints("test_input.txt");
	std::cout << "Part 1:\t" << blackPts.size() << std::endl;

	auto before = std::chrono::high_resolution_clock::now();

	simulate(blackPts, 50);

	auto after = std::chrono::high_resolution_clock::now();

	auto runTime = std::chrono::duration_cast<std::chrono::seconds>(after - before);
	std::cout << "At " << runTime.count() << "s\n";

	return 0;
}