#include <deque>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>

class Player {
	std::string name;
	std::deque<int> deck;
public:
	Player(const std::string name, const std::deque<int> deck)
		: name{name}, deck{deck}
	{}
	int draw() {
		int card = deck.front();
		deck.pop_front();
		return card;
	}
	void addToDeck(const std::vector<int>& cards) {
		deck.insert(deck.end(), cards.begin(), cards.end());
	}
	bool lost() {
		return deck.empty();
	}
	int getScore() {
		int score = 0;
		int multiplier = deck.size();
		for (auto it = deck.begin(); it != deck.end(); it++) {
			score += (*it) * multiplier;
			multiplier--;
		}
		return score;
	}
};

class Game {
	int round = 1;
	std::vector<Player> players{};
public:
	Game(const std::vector<Player>& players)
		:players{players}
	{}
	void playARound() { // assuming there are two players
		int firstCard = players.at(0).draw();
		int secondCard = players.at(1).draw();
		if (firstCard > secondCard) {
			players.at(0).addToDeck(std::vector<int>({ firstCard, secondCard }));
		} else if (secondCard > firstCard) {
			players.at(1).addToDeck(std::vector<int>({secondCard, firstCard}));
		}
		round++;
	}
	int play() {
		while (!players.at(0).lost() && !players.at(1).lost()) {
			playARound();
			if (players.at(0).lost()) {
				return players.at(1).getScore();
			} else if (players.at(1).lost()) {
				return players.at(0).getScore();
			}
		}
		return 0;
	}
};

Game getInput(const std::string fileName) {
	std::ifstream inFile;
	std::string line;
	inFile.open(fileName);
	std::vector<Player> players{};
	std::string name;
	std::deque<int> deck{};
	while (!inFile.eof()) {
		std::getline(inFile, line);
		auto index = line.find(':');
		if (line.empty()) {
			players.emplace_back(name, deck);
			deck.clear();
		} else if (index != -1) {
			name = line.substr(index);
		} else {
			deck.emplace_back(std::stoi(line));
		}
	}
	players.emplace_back(name, deck);
	inFile.close();
	return Game(players);
}

int main() {
	Game game = getInput("input.txt");
	
	std::cout << game.play() << std::endl;

	return 0;
}