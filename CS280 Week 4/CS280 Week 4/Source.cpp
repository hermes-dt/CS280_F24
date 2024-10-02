#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <sstream>
#include <map>

using namespace std;

// Checks if the word is a valid decimal literal
bool isDecimalLiteral(const string& word) {
    static regex pattern("^[1-9][0-9]$");
    return regex_match(word, pattern);
}

// Checks if the word is a valid octal literal
bool isOctalLiteral(const string& word) {
    static regex pattern("^0[0-7]+$");
    return regex_match(word, pattern);
}

// Checks if the word is a valid hexadecimal literal
bool isHexLiteral(const string& word) {
    static regex pattern("^0[xX][0-9a-fA-F]+$");
    return regex_match(word, pattern);
}

// Checks if the word is a valid binary literal
bool isBinaryLiteral(const string& word) {
    static regex pattern("^0[bB][01]+$");
    return regex_match(word, pattern);
}

// Checks if the word is an incorrect binary literal (starts with 0b or 0B but isn't valid)
bool isIncorrectBinaryLiteral(const string& word) {
    return (word.length() > 2 && (word.substr(0, 2) == "0b" || word.substr(0, 2) == "0B") && !isBinaryLiteral(word));
}

int main(int argc, char* argv[]) {
    // Checks if a filename was provided
    if (argc < 2) {
        cout << "NO SPECIFIED INPUT FILE NAME." << endl;
        return 1;
    }

    string filename = argv[1];
    ifstream file(filename);

    // Checks if the file can be opened
    if (!file.is_open()) {
        cout << "CANNOT OPEN THE FILE " << filename << endl;
        return 1;
    }

bool printAll = false, printBin = false, printDec = false, printHex = false, printOct = false;
  for (int i = 2; i < argc; i++) {
        string arg = argv[i];
        if (arg == "-all") printAll = true;
        else if (arg == "-bin") printBin = true;
        else if (arg == "-dec") printDec = true;
        else if (arg == "-hex") printHex = true;
        else if (arg == "-oct") printOct = true;
        else {
            cout << "UNRECOGNIZED FLAG " << arg << endl;
            return 1;
        }
    }

    string word;
    int lineCount = 0, wordCount = 0, decimalCount = 0, hexCount = 0, octalCount = 0, binaryCount = 0;
    
    std::map <std::string, int> binNum;
    std::map <std::string, int> decNum;
    std::map <std::string, int> hexNum;
    std::map <std::string, int> octNum;


    string line;
    while (getline(file, line)) {
        lineCount++;
        istringstream iss(line);

        while (iss >> word) {
            wordCount++;

            // Checks the type of literal & increments the appropriate counter
            if (isDecimalLiteral(word)) {
                decimalCount++;
                decNum[word]++;
            }
            else if (isHexLiteral(word)) {
                hexCount++;
                hexNum[word]++;
            }
            else if (isOctalLiteral(word)) {
                octalCount++;
                octNum[word]++;
            }
            else if (isBinaryLiteral(word)) {
                binaryCount++;
                binNum[word]++;
            }
            else if (isIncorrectBinaryLiteral(word)) {
                cout << "Incorrect Binary Literal " << word << " at Line: " << lineCount << endl;
            }
            else if (regex_match(word, regex("^0[0-7][8-9%][0-7]$"))) {
                cout << "Incorrect Octal Literal " << word << " at Line: " << lineCount << endl;
            }
            else if (regex_match(word, regex("^0[xX][0-9a-fA-F][g-zG-Z&]+[0-9a-fA-F]$"))) {
                cout << "Incorrect Hexadecimal Literal " << word << " at Line: " << lineCount << endl;
            }
            else if (regex_match(word, regex("^[+-]?[0-9]\.[0-9]$|^[+-]?[0-9]+[a-zA-Z]+$|^[0-9]+[+\-][0-9]+$|^[0-9]+[a-zA-Z][0-9]*$"))) {
                cout << "Incorrect Decimal Literal " << word << " at Line: " << lineCount << endl;
            }
        }
    }

    // Checks if the file was empty
    if (lineCount == 0) {
        cout << "File is empty." << endl;
        return 0;
    }

    // Prints outputs
    cout << "Total Number of Lines: " << lineCount << endl
        << "Number of Words: " << wordCount << endl
        << "Number of Decimal Literals: " << decimalCount << endl
        << "Number of Hexadecimal Literals: " << hexCount << endl
        << "Number of Octal Literals: " << octalCount << endl
        << "Number of Binary Literals: " << binaryCount << endl;

    if (printAll || printBin) {
        std::cout << std::endl;
        std::cout << "List of Binary-Literals and their number of occurrences:" << std::endl;
        for (const auto& pair : binNum) {
            std::cout << pair.first << ": " << pair.second << std::endl;
        }
    }
    if (printAll || printDec) {
        std::cout << std::endl;
        std::cout << "List of Decimal-Literals and their number of occurrences:" << std::endl;
        for (const auto& pair : decNum) {
            std::cout << pair.first << ": " << pair.second << std::endl;
        }
    }
    if (printAll || printHex) {
        std::cout << std::endl;
        std::cout << "List of Hex-Literals and their number of occurrences:" << std::endl;
        for (const auto& pair : hexNum) {
            std::cout << pair.first << ": " << pair.second << std::endl;
        }
    }
    if (printAll || printOct) {
        std::cout << std::endl;
        std::cout << "List of Octal-Literals and their number of occurrences:" << std::endl;
        for (const auto& pair : octNum) {
            std::cout << pair.first << ": " << pair.second << std::endl;
        }
    }


    return 0;
}