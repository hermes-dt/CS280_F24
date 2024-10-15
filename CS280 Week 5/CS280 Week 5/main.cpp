//=======================================================
// Name : Lex.cpp
// Description : Made by Hermy Taveras
//=======================================================
#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include "lex.h"
#include <cstring>
#include <algorithm>



int main(int argc, char* argv[]) {

    if (argc == 1) {
        cout << "No specified input file.\n";
        return 1;
    }

    bool flagNum = false, flagBool = false, flagChar = false, flagStr = false, flagId = false, flagKw = false, flagAll = false;
    int linenum = 1, tokCount = 0, IorK = 0, numCount = 0, boolCount = 0, SorC = 0;
    set<string> strings, chars, booleans, identifiers;
    set<double> numerics;
    map<Token, set<string>> keywords;


    ifstream infile(argv[1]);
    if (!infile) {
        cout << "CANNOT OPEN THE FILE " << argv[1] << "\n";
        return 1;
    }
    if (argc > 2) {
        for (int i = 2; i < argc; i++) {
            if (argv[i][0] == '-') {
                if (strcmp(argv[i], "-all") == 0) {
                    flagAll = true;
                }
                else if (strcmp(argv[i], "-num") == 0) {
                    flagNum = true;
                }
                else if (strcmp(argv[i], "-bool") == 0) {
                    flagBool = true;
                }
                else if (strcmp(argv[i], "-char") == 0) {
                    flagChar = true;
                }
                else if (strcmp(argv[i], "-str") == 0) {
                    flagStr = true;
                }
                else if (strcmp(argv[i], "-id") == 0) {
                    flagId = true;
                }
                else if (strcmp(argv[i], "-kw") == 0) {
                    flagKw = true;
                }
                else {
                    cout << "Unrecognized flag {" << argv[i] << "}\n";
                    return 1;
                }
            }
            else if (strcmp(argv[i], "num") == 0) { flagNum = true; }
            else if (strcmp(argv[i], "id") == 0) { flagId = true; }
            else if (strcmp(argv[i], "bool") == 0) { flagBool = true; }
            else if (strcmp(argv[i], "char") == 0) { flagChar = true; }
            else if (strcmp(argv[i], "kw") == 0) { flagKw = true; }
            else if (strcmp(argv[i], "str") == 0) { flagStr = true; }
            else {
                cout << "Only one file name is allowed.\n";
                return 1;
            }
        }
    }

    LexItem token;

    while ((token = getNextToken(infile, linenum)).GetToken() != DONE) {
        if (flagAll) {
            cout << token << endl;
        }
        switch (token.GetToken()) {
        case IDENT:
            identifiers.insert(token.GetLexeme());
            tokCount++;
            break;
        case PROGRAM: case IF: case ELSE: case PRINT:
        case INT: case FLOAT: case BOOL: case CHAR: case STRING:
            keywords[token.GetToken()].insert(token.GetLexeme());
            tokCount++;
            break;
        case ICONST: case RCONST:
            numerics.insert(stod(token.GetLexeme()));
            tokCount++;
            break;
        case BCONST:
            boolCount++;
            tokCount++;
            booleans.insert(token.GetLexeme());
            break;
        case SCONST:
            SorC++;
            tokCount++;
            strings.insert(token.GetLexeme());
            break;
        case CCONST:
            SorC++;
            tokCount++;
            chars.insert(token.GetLexeme());
            break;
        case PLUS: case MINUS: case MULT: case DIV: case ASSOP: case EQ:
        case NEQ: case ADDASSOP: case SUBASSOP: case MULASSOP:
        case DIVASSOP: case REMASSOP: case REM: case GTHAN: case LTHAN:
        case AND: case OR: case NOT: case COMMA: case SEMICOL:
        case LPAREN: case RPAREN: case LBRACE: case RBRACE: case DOT:
            tokCount++;
            // No need to count these separately for now, but included to avoid warnings.
            break;
        case ERR:
            return 1;  // Stop on error
        default:
            cerr << "Unhandled token: " << token << endl;
            return 1;  // Stop on unexpected token
        }
    }

    //Checks if the file is empty without consuming a any lines from the file
    if (tokCount == 0) {
        cout << "Empty file." << endl;
        return 0;
    }
    linenum--;

    IorK = identifiers.size() + keywords.size();
    numCount = numerics.size();

    cout << "\nLines: " << linenum << endl;
    cout << "Total Tokens: " << tokCount << endl;
    cout << "Identifiers and Keywords: " << IorK << endl;
    cout << "Numerics: " << numCount << endl;
    cout << "Booleans: " << boolCount << endl;
    cout << "Strings and Characters: " << SorC << endl;

    if (flagNum && !numerics.empty()) {
        cout << "NUMERIC CONSTANTS:\n";
        bool first = true;
        for (const auto& kw : numerics) {
            if (!first) cout << ", ";
            cout << kw;
            first = false;
        }
        cout << "\n";
    }
    if (flagBool && !booleans.empty()) {
        cout << "BOOLEAN CONSTANTS:\n";
        bool first = true;
        for (const auto& bl : booleans) {
            if (!first) cout << ", ";
            cout << bl;
            first = false;
        }
        cout << "\n";
    }
    if (flagChar && !chars.empty()) {
        cout << "CHARACTER CONSTANTS:\n";
        bool first = true;
        for (const auto& chr : chars) {
            if (!first) cout << ", ";
            cout << chr;
            first = false;
        }
        cout << "\n";
    }
    if (flagStr && !strings.empty()) {
        cout << "STRINGS:\n";
        bool first = true;
        for (const auto& str : strings) {
            if (!first) cout << ", ";
            cout << str << "\"";
            first = false;
        }
        cout << "\n";
    }
    if (flagId && !identifiers.empty()) {
        cout << "IDENTIFIERS:\n";
        bool first = true;
        for (const auto& id : identifiers) {
            if (!first) cout << ", ";
            cout << id;
            first = false;
        }
        cout << "\n";
    }
    if (flagKw && !keywords.empty()) {
        cout << "KEYWORDS:\n";
        bool first = true;
        for (const auto& kw_pair : keywords) {
            for (const auto& lexeme : kw_pair.second) {
                if (!first) cout << ", ";
                cout << lexeme;
                first = false;
            }
            
        }
        cout << "\n";
        return 0;
    }
}
