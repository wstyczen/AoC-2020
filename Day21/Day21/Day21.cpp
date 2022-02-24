#include <string>
#include <fstream>
#include <regex>
#include <vector>
#include <algorithm>
#include <set>
#include <unordered_map>
#include <iostream>

class Food {
public:
	const std::vector<std::string> ingredients;
	const std::vector<std::string> allergens;
	Food(const std::vector<std::string>& ingredients_, const std::vector<std::string>& allergens_)
		: ingredients{ingredients_}, allergens{allergens_}
	{}
	bool containsIngredient(const std::string& ingredient) const {
		return std::find(ingredients.begin(), ingredients.end(), ingredient) != ingredients.end();
	}
	bool containsAllergen(const std::string& allergen) const {
		return std::find(allergens.begin(), allergens.end(), allergen) != allergens.end();
	}
};

std::vector<std::string> getWords(std::string s) {
	std::vector<std::string> words{};
	std::regex wordPattern("([a-z]+)");
	std::smatch match;
	while (std::regex_search(s, match, wordPattern)) {
		words.push_back(match.str(1));
		s = match.suffix().str();
	}
	return words;
}
std::vector<Food> getInput(const std::string fileName) {
	std::vector<Food> foods{};
	const std::vector<std::string> ingredients{};
	const std::vector<std::string> allergens{};
	std::regex splitPattern("\\(contains ");
	std::ifstream inFile;
	std::smatch match;
	inFile.open(fileName);
	std::string line;
	while (!inFile.eof()) {
		std::getline(inFile, line);
		if (std::regex_search(line, match, splitPattern)) {
			std::string prefix = match.prefix().str();
			std::string suffix = match.suffix().str();
			foods.push_back(Food(getWords(prefix), getWords(suffix)));
		}
	}
	inFile.close();
	return foods;
}
std::unordered_map<std::string, std::set<std::string>> getAllergensMap(const std::vector<Food>& foods) {
	std::unordered_map<std::string, std::set<std::string>> allergensMap{};
	for (const Food& food : foods) {
		for (const std::string& allergen : food.allergens) {
			std::set<std::string> ingredients = std::set<std::string>(food.ingredients.begin(), food.ingredients.end());
			if (allergensMap.find(allergen) == allergensMap.end()) {
				allergensMap[allergen] = ingredients;
			} else {
				std::set<std::string> intersection{};
				std::set<std::string> possibleIngredients = allergensMap[allergen];
				std::set_intersection(
					possibleIngredients.begin(), possibleIngredients.end(),
					ingredients.begin(), ingredients.end(),
					std::inserter(intersection, intersection.begin())
				);
				allergensMap[allergen] = intersection;
			}
		}
	}
	return allergensMap;
}
std::set<std::string> getUnsafeIngredients(const std::vector<Food>& foods) {
	const std::unordered_map<std::string, std::set<std::string>> allergensMap = getAllergensMap(foods);
	std::set<std::string> unsafeIngredients{};
	for (const auto& pair : allergensMap) {
		unsafeIngredients.insert(pair.second.begin(), pair.second.end());
	}
	return unsafeIngredients;
}
int getCountOfSafeIngredientsAppearances(const std::vector<Food>& foods) {
	std::set<std::string> unsafeIngredients = getUnsafeIngredients(foods);
	int appearances = 0;
	for (const Food& food : foods) {
		for (const std::string& ingredient : food.ingredients) {
			if (unsafeIngredients.find(ingredient) == unsafeIngredients.end()) appearances++;
		}
	}
	return appearances;
}
std::unordered_map<std::string, std::string> getDecodedAllergensMap(const std::vector<Food>& foods) {
	std::unordered_map<std::string, std::set<std::string>> allergensMap = getAllergensMap(foods);
	std::unordered_map<std::string, std::string> decodedAllergensMap{};
	while (!allergensMap.empty()) {
		for (const auto& pair : allergensMap) {
			if (pair.second.size() == 1) {
				decodedAllergensMap[pair.first] = *(pair.second.begin());
			}
		}
		for (const auto& pair : decodedAllergensMap) { // erase the keys(allergens)
			allergensMap.erase(pair.first);
		}
		for (auto& pair : allergensMap) {
			for (const auto& dPair : decodedAllergensMap) {
				pair.second.erase(dPair.second);
			}
		}

	}
	return decodedAllergensMap;
}
std::vector<std::string> getCanonicalDangerousIngredientList(const std::vector<Food>& foods) {
	std::unordered_map<std::string, std::string> decodedAllergensMap = getDecodedAllergensMap(foods);
	std::vector<std::string> CDIL{};
	std::set<std::string> allergens{};
	std::for_each(decodedAllergensMap.begin(), decodedAllergensMap.end(), [&allergens](const auto& pair) {allergens.insert(pair.first); });
	std::for_each(allergens.begin(), allergens.end(), [&decodedAllergensMap, &CDIL](auto allergen) {CDIL.push_back(decodedAllergensMap[allergen]); });
	return CDIL;
}
std::string toString(const std::vector<std::string>& vec) {
	std::string stringForm = "";
	for (auto vecIter = vec.begin(); vecIter != vec.end(); vecIter++) {
		if (vecIter != vec.begin()) stringForm += ",";
		stringForm += *vecIter;
	}
	return stringForm;
}

int main() {
	std::vector<Food> foods = getInput("input.txt");
	
	std::cout << "Part 1:\t" << getCountOfSafeIngredientsAppearances(foods) << std::endl;

	std::cout << "\nIngredients and their respective allergens:\n";
	std::unordered_map<std::string, std::string> decodedAllergensMap = getDecodedAllergensMap(foods);
	for (const auto& pair : decodedAllergensMap) {
		std::cout << pair.second << " -> " << pair.first << std::endl;
	}

	std::cout << "\nPart 2:\t" << toString(getCanonicalDangerousIngredientList(foods)) << std::endl;

	return 0;
}