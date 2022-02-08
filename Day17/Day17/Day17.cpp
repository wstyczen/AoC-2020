#include <string>
#include <vector>
#include <fstream>
#include <cmath>
#include <iostream>


//P1
std::vector<std::vector<std::vector<int>>> getInput(std::string fileName, int cycles) {
	std::vector<std::vector<std::vector<int>>> cubes = {};
	std::vector<std::vector<int>> input = {};
	std::ifstream inFile;
	inFile.open(fileName);
	std::string line;
	while (!inFile.eof()) {
		std::getline(inFile, line);
		std::vector<int> row = {};
		for (char c : line) {
			(c == '#') ? row.push_back(1) : row.push_back(0);
		}
		input.push_back(row);
	}
	inFile.close();
	for (int z = 0; z < 1 + cycles * 2; z++) {
		std::vector<std::vector<int>> slice = {};
		for (int y = 0; y < input.size() + cycles * 2; y++) {
			slice.push_back(std::vector<int>(input.at(0).size() + cycles * 2, 0));
		}
		cubes.push_back(slice);
	}

	for (int i = 0; i < input.size(); i++) {
		for (int j = 0; j < input.at(0).size(); j++) {
			cubes.at(std::ceil(cycles)).at(i + cycles - 1).at(j + cycles) = input.at(i).at(j);
		}
	}

	return cubes;
}

bool isWithin(const std::vector<std::vector<std::vector<int>>>& cubes, int z, int y, int x) {
	return !((z < 0 || z >= cubes.size()) || (y < 0 || y >= cubes.at(0).size()) || (x < 0 || x >= cubes.at(0).at(0).size()));
}

int getNeighboursActive(const std::vector<std::vector<std::vector<int>>>& cubes, int z, int y, int x) {
	int active = 0;
	for (int k = z - 1; k <= z + 1; k++) {
		for (int j = y - 1; j <= y + 1; j++) {
			for (int i = x - 1; i <= x + 1; i++) {
				if (isWithin(cubes, k, j, i) && (k != z || j != y || i != x) && cubes.at(k).at(j).at(i)) {
					active++;
				}
			}
		}
	}
	return active;
}

void printState(std::vector<std::vector<std::vector<int>>>& cubes, int cycle) {
	//// print state
	std::cout << "Cycle " << cycle << ":\n\n";
	for (int z = 0; z < cubes.size(); z++) {
		std::cout << "-> Pane " << (z + 1) << ":\n";
		for (int y = 0; y < cubes.at(0).size(); y++) {
			for (int x = 0; x < cubes.at(0).at(0).size(); x++) {
				char c = (cubes.at(z).at(y).at(x)) ? '#' : '.';
				std::cout << c;
			}
			std::cout << std::endl;
		}
	}
	std::cout << ":\n\n";
	// end print
}

void simulate(std::vector<std::vector<std::vector<int>>>& cubes, int cycles) {
	int cycle = 0;
	while (cycle++ < cycles) {
		std::vector<std::vector<std::vector<int>>> newCubes = {};
		//init new Vector
		for (int z = 0; z < cubes.size(); z++) {
			std::vector<std::vector<int>> slice = {};
			for (int y = 0; y < cubes.at(0).size(); y++) {
				slice.push_back(std::vector<int>(cubes.at(0).at(0).size(), 0));
			}
			newCubes.push_back(slice);
		}
		for (int z = 0; z < cubes.size(); z++) {
			for (int y = 0; y < cubes.at(0).size(); y++) {
				for (int x = 0; x < cubes.at(0).at(0).size(); x++) {
					int neighboursActive = getNeighboursActive(cubes, z, y, x);
					if (cubes.at(z).at(y).at(x)) { // active
						if (neighboursActive == 2 || neighboursActive == 3) newCubes[z][y][x] = 1;
						else newCubes[z][y][x] = 0;
					}
					else { // inactive
						if (neighboursActive == 3) newCubes[z][y][x] = 1;
						else newCubes[z][y][x] = 0;
					}
				}
			}
		}
		cubes = newCubes;
	}
	printState(cubes, 6);
}

int countActive(std::vector<std::vector<std::vector<int>>>& cubes) {
	int activeCount = 0;
	for (auto pane : cubes) {
		for (auto row : pane) {
			for (int cube : row) {
				if (cube) activeCount++;
			}
		}
	}
	return activeCount;
}

int main() {
	std::string fileName = "input.txt";
	int cycles = 6;
	std::vector<std::vector<std::vector<int>>> cubes = getInput(fileName, cycles);
	simulate(cubes, cycles);
	std::cout << "Active after " << cycles << " cycles: " << countActive(cubes) << std::endl;
	return 0;
}