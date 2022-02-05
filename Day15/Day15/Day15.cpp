#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <chrono> // to measure runtime

class MemoryGame {
	std::unordered_map<int, long long> lastSpoken;
	long long turn=1;
	const std::vector<int> startingNrs;
public:
	MemoryGame(std::vector<int> startingNrs_)
		: startingNrs{startingNrs_}
	{}
	int play(const long long& turns) {
		int current = 0;
		int next = 0;
		for (auto nr : startingNrs) { // go through starting nrs
			lastSpoken[nr] = turn;
			current = nr;
			turn++;
			next = 0;
		}
		while (turn <= turns) {
			current = next;
			if (lastSpoken.find(current) == lastSpoken.end()) {
				next = 0;
			} else {
				next = turn - lastSpoken[current];
			}
			lastSpoken[current] = turn;
			turn++;
		}
		return current;;
	}
};

int main() {
	auto start = std::chrono::high_resolution_clock::now();

	std::vector<int> input = { 0,1,5,10,3,12,19 };

	MemoryGame mg(input);
	std::cout << "Part 1: " << mg.play(2020) << std::endl;
	// Part 2 takes ~80s to run - but I don't really have any ideas to optimize it by that much
	std::cout << "Part 2: " << mg.play(30000000) << std::endl;

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
	std::cout << "Runtime: " << duration.count() << "s" << std::endl;

	return 0;
}