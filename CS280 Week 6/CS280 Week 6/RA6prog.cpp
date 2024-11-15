#include <iostream>
#include <string>
#include <fstream>
#include "RA6_H_Taveras.h"
using namespace std;


string RemoveVowels(string str, int curr);

int main(int argc, char *argv[])
{
	string instr;
	string result;
	
	ifstream  file ;

	if (argc == 1)
	{
		cerr << "NO SPECIFIED INPUT FILE." << endl;
		return 0;
	}
	else
	{
		file.open(argv[1]);
		if( file.is_open() == false ) {
			cerr << "CANNOT OPEN THE FILE " << argv[1] << endl;
			return 0;
		}

	}
	
	 getline(file, instr);
		
	//getline(cin, instr);
	
	result = RemoveVowels(instr, 0);
	
	cout << "The input string " << "\"" <<instr << "\"" << " with all vowels are removed: " << "\"" << result <<  "\"" << endl;
	return 0;
}
