#include <iostream>
#include<string>

int main() {
	std::string firstname, lastname;
	int sectionNum;
	
	std::cout << "Please enter your first name >>" << std::endl;
	std::cin >> firstname;
	std::cout << "Please enter you last name >>" << std::endl;
	std::cin >> lastname;
	std::cout << "Please enter your CS 280 section number" << std::endl;
	std::cin >> sectionNum;

	std::cout << "Welcome " << firstname << " to CS 280 section " << sectionNum << "." << std::endl;

	return 0;
}