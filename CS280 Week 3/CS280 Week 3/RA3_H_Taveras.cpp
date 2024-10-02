//=======================================================
// Name : RA3_H_Taveras.cpp
// Description : Reading files to detect for integer
// literals and outputting the amount of words, lines and
// errors
//=======================================================
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <regex>

//A function to check if a word respresents an Interger Literal
static std::string what_type(const std::string& word) {
	if (word[0] == '0') {
		if (word.size() > 1 && (word[1] == 'x' || word[1] == 'X')) {
			return "hex";
		}
		else if (word.size() > 1 && (word[1] == 'b' || word[1] == 'B')) {
			return "binary";
		}
		else {
			return "octal";
		}
	}
	else if (std::isdigit(word[0])) {
		return "decimal";
	}
	else {
		return "non-literal";
	}
}

int main(int argc, char* argv[]) {

	//Checks if a file name was provided before assigning the file's name to a string
	if (argc == (1)) {
		std::cout << "NO SPECIFIED INPUT FILE NAME." << std::endl;
			return 1;
	}
	
	std::string theFile = argv[1];
	std::ifstream rFile;
	std::string linia;


	int decimals = 0, octals = 0, hexs = 0, binarys = 0, lines = 0, words = 0;
	
	//regular expression definitions
	std::regex literaldec("[1-9][0-9]*");
	std::regex literaloct("0[0-7]+$");  
	std::regex literalhex("0[xX][0-9a-fA-F]+$");
	std::regex literalbin("0[bB][01]+$");

	

	//Opens File
	rFile.open(theFile, std::ios_base::in);

	//If the file isn't opening
	if (!rFile) {
		std::cout << "CANNOT OPEN THE FILE " << theFile << std::endl;
			return 1;
	}

	//Reads each line in the file
	while (getline(rFile, linia)) {
		lines++;
		std::istringstream breakup(linia); //Breaks up he strings drawn from each line into a stream of words
		std::string word;
		//Check each word from the line
		while (breakup >> word) {
			words++;
			std::string type = what_type(word);
			if (type == "decimal") {
				if (std::regex_match(word, literaldec)) {
					decimals++;
				}
				else {
					std::cout << "Incorrect Decimal Literal " << word << " at Line: " << lines << std::endl;
				}
			}
			else if (type == "octal") {
				if (std::regex_match(word, literaloct)) {
					octals++;
				}
				else {
					std::cout << "Incorrect Octal Literal " << word << " at Line: " << lines << std::endl;
				}
			}
			else if (type == "hex") {
				if (std::regex_match(word, literalhex)) {
					hexs++;
				}
				else {
					std::cout << "Incorrect Hexadecimal Literal " << word << " at Line: " << lines << std::endl;
				}
			}
			else if (type == "binary") {
				if (std::regex_match(word, literalbin)) {
					binarys++;
				}
				else {
					std::cout << "Incorrect Binary Literal " << word << " at Line: " << lines << std::endl;
				}
			}
		
		}
	}

	//Checks if the file is empty without consuming a any lines from the file
	if (lines == 0) {
		std::cout << "File is empty." << std::endl;
		return 0;
	}

	//Outputs the data
	std::cout << "Total Number of Lines: " << lines << std::endl;
	std::cout << "Number of Words: " << words << std::endl;
	std::cout << "Number of Decimal Literals: " << decimals << std::endl;
	std::cout << "Number of Hexadecimal Literals: " << hexs << std::endl;
	std::cout << "Number of Octal Literals: " << octals << std::endl;
	std::cout << "Number of Binary Literals: " << binarys << std::endl;

	return 0;
}
