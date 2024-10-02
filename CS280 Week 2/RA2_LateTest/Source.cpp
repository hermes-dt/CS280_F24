#include <iostream>
#include <fstream>
#include <ostream>
#include <sstream>
#include <string>

int main() {

	std::string fileName;
	std::ifstream inFile;
	std::string line;

	std::cout << "Enter the name of the file to read from(not case-sensitive): " << std::endl;
	std::cin >> fileName;

	int lineCount = 0, commentCount = 0, wordCount = 0, paragraphCount = 0;
	int boldCount = 0, italicCount = 0, underlineCount = 0;
	

	inFile.open(fileName);

	//Fail check to see if the file was opened or not
	if (!inFile) {
		std::cout << "Failed canot be opened " << fileName << std::endl;
		return 1;
	}
	//Checks to see if the line is a comment if it is the code adds it to the 
	//lineCount and uses the continue command to execute the next iteration (Goes back to the while loop)
	while (getline(inFile, line)) {
		lineCount++;
		if (line[0] == '#') {
			commentCount++;
			continue;
		}
	}
	



	std::cout << "Total lines: " << lineCount << std::endl;
	std::cout << "Commented lines: " << commentCount << std::endl;
	std::cout << "Number of paragraphs: " << paragraphCount << std::endl;
	std::cout << "Total number of words: " << wordCount << std::endl;
	std::cout << "Bold commands: " << boldCount << std::endl;
	std::cout << "Italic commands: " << italicCount << std::endl;
	std::cout << "Underline commands: " << underlineCount << std::endl;

	inFile.close();

		return 0;
}