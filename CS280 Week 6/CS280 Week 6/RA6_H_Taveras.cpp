//=======================================================
// Name : RA5.cpp
// Description : Made by Hermy Taveras
//=======================================================
#include <iostream>
#include <string>
#include <map>
#include "RA6_H_Taveras.h"


std::string RemoveVowels(std::string str, int ind) {
	char chr = str[ind]; //Grabs the first leter in the string and consumes it
	const std::map<char, int> vowels = { //Map of all the vowels
		{'u', 0}, {'U', 0}, {'a', 0}, {'A', 0}, {'e', 0}, {'E', 0}, {'i', 0}, {'I', 0}, {'o', 0}, {'O', 0},
	};
	auto it = vowels.find(chr); //looks for vowels in the map

	if (ind >= str.size()) { //base case for when the end of string it reached
		return "";
	}
	else if (it != vowels.end()) { //skips any vowels found
		return RemoveVowels(str, ind + 1);
	}
	else { //adds everything else
		return chr + RemoveVowels(str, ind + 1);
	}


}