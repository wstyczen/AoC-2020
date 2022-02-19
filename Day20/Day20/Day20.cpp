#include <vector>
#include <string>
#include <fstream>
#include <regex>
#include <iostream>
#include <algorithm>
#include <iterator>

struct IntPair {
	const static int uninitiallizedValue;
	int first;
	int second;
	IntPair(int frst, int scnd)
		:first{frst}, second{scnd}
	{}
	IntPair()
		:first{ uninitiallizedValue }, second{ uninitiallizedValue }
	{}
	bool isInitialized() const {
		return (first != uninitiallizedValue && second != uninitiallizedValue);
	}
};
const int IntPair::uninitiallizedValue = INT_MIN;
bool operator==(const IntPair& lhs, const IntPair& rhs) {
	if (!(lhs.isInitialized() && rhs.isInitialized())) return false;
	return (lhs.first == rhs.first && lhs.second == rhs.second);
}
void operator+=(IntPair& lhs, const IntPair& rhs) {
	lhs.first += rhs.first;
	lhs.second += rhs.second;
}
IntPair operator+(const IntPair& lhs, const IntPair& rhs) {
	return IntPair(lhs.first + rhs.first, lhs.second + rhs.second);
}
bool operator<(const IntPair& lhs, const IntPair& rhs) {
	if (lhs.second == rhs.second)
		return (lhs.first < rhs.first);
	return (lhs.second > rhs.second);
}
std::ostream& operator<<(std::ostream& os, const IntPair& pair) {
	os << "(" << pair.first << ", " << pair.second << ")";
	return os;
}

class Tile {
	int id;
	std::vector<std::vector<char>> layout;
	IntPair position;
public:
	Tile(const int id_, std::vector<std::vector<char>>& layout_)
		: id{id_}, layout{layout_}
	{}

	Tile(const Tile& other)
		: id{other.getId()}, layout{other.getLayout()}, position{other.getPosition()}
	{}

	int getId() const  {
		return id;
	}
	std::vector<std::vector<char>> &getLayout() {
		return layout;
	}
	const std::vector<std::vector<char>>& getLayout() const {
		return layout;
	}
	const IntPair& getPosition() const {
		return position;
	}
	IntPair& getPosition() {
		return position;
	}

	IntPair getPositionByVal() const {
		return position;
	}
	std::vector<std::vector<char>> getLayoutByVal() const {
		return layout;
	}

	bool placed() const {
		return position.isInitialized();
	}
	void setPosition(const IntPair& newPosition) {
		position = newPosition;
	}
	std::vector<std::vector<char>> getLayoutWithoutBorders() {
		std::vector<std::vector<char>> withoutBorders = std::vector<std::vector<char>>(layout.begin() + 1, layout.end() - 1);
		std::for_each(withoutBorders.begin(), withoutBorders.end(), [](auto& row) {row = std::vector<char>(row.begin() + 1, row.end() - 1); });
		return withoutBorders;
	}
};
std::ostream& operator<<(std::ostream& os, const Tile& tile) {
	os << "Tile " << tile.getId() << "\t" << tile.getPosition() << std::endl; 
	for (const std::vector<char>& row : tile.getLayout()) {
		for (char c : row) {
			os << c;
		}
		os << std::endl;
	}
	return os;
}
bool operator<(const Tile& lhs, const Tile& rhs) {
	return (lhs.getPosition() < rhs.getPosition());
}

template<typename El>
void rotate(std::vector<std::vector<El>>& matrix, const int degrees = 90) {
	if (degrees % 90) return;
	int turns = (degrees >= 0) ? (degrees / 90) : (4 + (degrees / 90));

	// operates on the assumption that it is a SQUARE matrix, ie. it has the same nr of rows and columns - n
	int n = matrix.size();

	auto cycle = [](El& a, El& b, El& c, El& d) {
		int temp = a;
		a = b;
		b = c;
		c = d;
		d = temp;
	};
	while (turns-- > 0)
		for (int i = 0; i < n / 2; i++)
			for (int j = 0; j < (n + 1) / 2; j++)
				cycle(matrix[i][j], matrix[n - 1 - j][i], matrix[n - 1 - i][n - 1 - j], matrix[j][n - 1 - i]);

}
template<typename El>
void mirror(std::vector<std::vector<El>>& matrix) {
	std::for_each(matrix.begin(), matrix.end(), [](auto& row) {std::reverse(row.begin(), row.end());});
} 
template<typename El>
int countOccurences(std::vector<std::vector<El>>& matrix, const El element) {
	int count = 0;
	std::for_each(matrix.begin(), matrix.end(), [&count, element](auto& row) { for (char c : row) if (c == element) count++; });
	return count;
}

class Image {
	std::vector<Tile> tiles;
public:
	Image(std::vector<Tile>& tiles_)
		: tiles{tiles_}
	{}
	Tile& getTile(int id_) {
		for (Tile& tile : tiles) {
			if (tile.getId() == id_) return tile;
		}
		return tiles.at(0);
	}
	bool isOccupied(const IntPair& position) {
		for (const Tile& tile : tiles) {
			if (tile.placed()) {
				if (position == tile.getPosition()) return true;
			}
		}
		return false;
	}
	bool placeNextTo(const Tile& placedTile, Tile& tile) {
		auto getCol = [](const std::vector<std::vector<char>>& layout_, const int index) {
			std::vector<char> col{};
			std::for_each(layout_.begin(), layout_.end(),
				[=, &col](const std::vector<char>& row) { col.push_back(row.at(index)); });
			return col;
		};
		auto getRow = [](const std::vector<std::vector<char>>& layout_, const int index) {
			return layout_.at(index);
		};

		int n = placedTile.getLayout().size();

		IntPair newPosition = IntPair(placedTile.getPosition().first, placedTile.getPosition().second);
		IntPair Top = newPosition + IntPair(0, 1);
		IntPair Right = newPosition + IntPair(1, 0);
		IntPair Bottom = newPosition + IntPair(0, -1);
		IntPair Left = newPosition + IntPair(-1, 0);
		int count = 0; // 0 for unflipped, 1 for flipped (with mirror()) tile
		while (count++ < 2) {
			int turns = 4;
			while (turns-- > 0) {
				if (!isOccupied(Top)) {
					if (getRow(placedTile.getLayout(), 0) == getRow(tile.getLayout(), n - 1)) {
						tile.setPosition(Top);
						return true;
					}
				}
				if (!isOccupied(Bottom)) {
					if (getRow(placedTile.getLayout(), n - 1) == getRow(tile.getLayout(), 0)) {
						tile.setPosition(Bottom);
						return true;
					}
				}
				if (!isOccupied(Right)) {
					if (getCol(placedTile.getLayout(), n - 1) == getCol(tile.getLayout(), 0)) {
						tile.setPosition(Right);
						return true;
					}
				}
				if (!isOccupied(Left)) {
					if (getCol(placedTile.getLayout(), 0) == getCol(tile.getLayout(), n - 1)) {
						tile.setPosition(Left);
						return true;
					}
				}
				rotate(tile.getLayout());
			}
			mirror(tile.getLayout());
		}
		
		return false;
	}
	bool place(Tile& tile) {
		for (const Tile& placedTile : tiles) {
			if (!placedTile.placed()) continue;
			if (placeNextTo(placedTile, tile)) return true;
		}
		return false;
	}
	bool allPlaced() {
		for (const Tile& tile : tiles) {
			if (!tile.placed())
				return false;
		}
		//std::for_each(tiles.begin(), tiles.end(), [](const Tile& tile) {if (!tile.placed()) return false; });
		return true;
	}
	void placeTiles() {
		// set first tile to position (0, 0) -> place other tiles around it;
		tiles.at(0).setPosition(IntPair(0, 0));
		while (!allPlaced()) {
			bool placedSth = false;
			for (auto tileIt = tiles.begin(); tileIt != tiles.end(); tileIt++) {
				if (place(*tileIt)) placedSth = true;
			}
			if (!placedSth) break;	
		}
		std::sort(tiles.begin(), tiles.end());
	}
	long long getProductOfCornerIds() {
		std::vector<int> xs{};
		std::vector<int> ys{};
		for (const Tile& tile : tiles) {
			xs.push_back(tile.getPosition().second);
			ys.push_back(tile.getPosition().first);
		};
		int maxX = *std::max_element(xs.begin(), xs.end());
		int minX = *std::min_element(xs.begin(), xs.end());
		int maxY = *std::max_element(ys.begin(), ys.end());
		int minY = *std::min_element(ys.begin(), ys.end());
		long long product = 1;
		for (const Tile& tile : tiles) {
			IntPair pair = tile.getPosition();
			if (pair == IntPair(maxY, maxX) || pair == IntPair(maxY, minX) || pair == IntPair(minY, maxX) || pair == IntPair(minY, minX)) {
				product *= tile.getId();
			}
		}
		return product;
	}

	std::vector<std::vector<char>> getImage() {
		std::vector<std::vector<char>> image{};
		std::vector<std::vector<char>> row = {};
		auto addHorizontally = [&row](const auto& matrix) {
			auto rowIt = row.begin();
			auto matrixIt = matrix.begin();
			while (rowIt != row.end() && matrixIt != matrix.end()) {
				(*rowIt).insert((*rowIt).end(), (*matrixIt).begin(), (*matrixIt).end());
				rowIt++; matrixIt++;
			}
		};
		int rowIndex = tiles.at(0).getPosition().second;
		for (Tile& tile : tiles) {
			if (row.empty()) {
				row = tile.getLayoutWithoutBorders();
			} else {
				if (tile.getPosition().second != rowIndex) {
					rowIndex = tile.getPosition().second;
					for (const auto& vec : row) {
						image.push_back(vec);
					}
					row = tile.getLayoutWithoutBorders();
				} else {
					addHorizontally(tile.getLayoutWithoutBorders());
				}
			}
		}
		for (const auto& vec : row) {
			image.push_back(vec);
		}
		return image;
	}
	void substituteMonsters(std::vector<std::vector<char>>& image, const std::vector<IntPair>& locations, const char match = '#', const char subChar = 'o') {
		const int rows = image.size();
		const int cols = image.at(0).size();
		int locI;
		int locJ;
		for (int side = 0; side < 2; side++) {
			mirror(image);
			for (int rotation = 0; rotation < 4; rotation++) {
				rotate(image);
				for (int i = 0; i < rows; i++) {
					for (int j = 0; j < cols; j++) {
						bool foundMonster = true;
						for (const IntPair& loc : locations) {
							locI = loc.first + i;
							locJ = loc.second + j;
							if ((locI >= rows || locJ >= cols) || (image.at(locI).at(locJ) != match)) {
								foundMonster = false;
								break;
							}
						}
						if (foundMonster) {
							for (const IntPair& loc : locations) {
								locI = loc.first + i;
								locJ = loc.second + j;
								image.at(locI).at(locJ) = subChar;
							}
						}
					}
				}
			}
		}
	}

};

Image getImage(const std::string fileName) {
	std::vector<Tile> tiles{};

	std::ifstream inFile;
	inFile.open(fileName);
	std::string line;
	std::regex nrPattern("\\d+");
	std::smatch match;

	int id{};
	std::vector<std::vector<char>> layout;

	while (!inFile.eof()) {
		std::getline(inFile, line);
		if (line.empty()) {
			tiles.push_back(Tile(id, layout));
			layout.clear();
		}
		else if (std::regex_search(line, match, nrPattern)) {
			id = std::stoi(match.str());
		} else {
			layout.push_back(std::vector<char>(line.begin(), line.end()));
		}
	}
	inFile.close();

	return Image(tiles);
}
std::vector<IntPair> getLocations(const std::string fileName, const char key='#') {
	std::vector<IntPair> locations{};
	std::ifstream inFile;
	inFile.open(fileName);
	std::string line;
	int row = 0;
	while (!inFile.eof()) {
		std::getline(inFile, line);
		int col = 0;
		for (char c : line) {
			if (c == key)
				locations.push_back(IntPair(row, col));
			col++;
		}
		row++;
	}
	inFile.close();
	return locations;
}

int main() {
	Image image = getImage("input.txt");
	image.placeTiles();

	std::cout << "Part 1:\t" << image.getProductOfCornerIds() << std::endl;

	std::vector<IntPair> locations = getLocations("monsterImageP2.txt");
	std::vector<std::vector<char>> procImage = image.getImage();
	image.substituteMonsters(procImage, locations);

	std::cout << "Part 2:\t" << countOccurences(procImage, '#') << std::endl;

	return 0;
}