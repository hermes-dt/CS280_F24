#include <iostream>
#include <map>
#include <fstream>
#include <cctype>
#include <map>
#include "lex.h"

/*
 * CS280 - Fall 2024
*/


using std::map;

using namespace std;

istream& operator>>(istream& in, LexItem& tok) {

	
	map <string, Token> StrToTok = {
		{"PROGRAM", PROGRAM },
		{"PRINT", PRINT},
		{ "IF", IF },
		{ "ELSE", ELSE },	
		{ "IDENT", IDENT },
		{ "BOOL", BOOL },
		{ "STRING", STRING },
		{ "INT", INT },
		{ "FLOAT", FLOAT },
		{ "CHAR", CHAR },
		{ "ICONST", ICONST },
		{ "RCONST", RCONST },
		{ "SCONST", SCONST },
		{ "BCONST", BCONST},
		{ "CCONST", CCONST },
		
			
		{ "PLUS", PLUS },
		{ "MINUS", MINUS },
		{ "MULT", MULT },
		{ "DIV", DIV },
		{ "ASSOP", ASSOP },
		{ "EQ", EQ },
		{ "NEQ", NEQ },
		{ "GTHAN", GTHAN },
		{ "LTHAN", LTHAN },	
		{ "AND", AND },
		{ "OR", OR },
		{ "NOT", NOT },
		{ "COMMA", COMMA },
		{ "SEMICOL", SEMICOL },
		{ "LPAREN", LPAREN },
		{ "RPAREN", RPAREN },
		{ "LBRACE", LBRACE },
		{ "RBRACE", RBRACE },
		{ "REM", REM },
		{ "ADDASSOP", ADDASSOP },
		{ "SUBASSOP", SUBASSOP },
		{ "MULASSOP", MULASSOP },
		{ "DIVASSOP", DIVASSOP },
		{ "REMASSOP", REMASSOP },
		{ "TRUE", TRUE },
		{ "FALSE", FALSE },
		{ "DOT", DOT },
		{ "ERR", ERR },

		{ "DONE", DONE },
};
	Token tt;
	string tokstr, lexeme, str2;
	int line, strlen, i;
	char ch;
		
	in >> tokstr ;
	if(!in)
	{
		return in;
	}
	auto ttitr = StrToTok.find(tokstr);
	//cout << tokstr << endl;
	if(ttitr != StrToTok.end())
	{
		tt = ttitr->second;
		if(tt == IDENT)
		{
			in >> lexeme >> line;
			tok = id_or_kw(lexeme, line);
		
		}
		else if (tt == ICONST || tt == RCONST )
		{
			in >> lexeme >> line;
			LexItem item(tt, lexeme, line);
			tok = item;
		
		}
		else if (tt == SCONST)
		{
			in >> strlen;
			str2 = ""; i = 0;
			ch = in.get();
			while (ch != '\"')
			{
				ch = in.get();
			}
			ch = in.get();
			while ( (ch != '\"') && (i < strlen))
			{
				str2 += ch;
				ch = in.get();
				i++;
			}
			in >> line;
			LexItem item(tt, str2, line);
			tok = item;
		}
		else 
		{
			in >> lexeme >> line;
			LexItem item(tt, lexeme, line);
			tok = item;
			
		}
		
	}
	
	return in;
}

int main(int argc, char *argv[])
{
	LexItem item(ERR, "", 0);
	LexItem& tokItem = item;
	LexItem kwtok;
	ifstream  file ;

	string str1;
	
	if (argc == 1)
	{
		cerr << "NO SPECIFIED INPUT FILE." << endl;
		return 0;
	}
	else
	{
		if(argv[1][0] == '-')
		{
			cerr << "NO SPECIFIED INPUT FILE." << endl;
			return 0;
		}
		file.open(argv[1]);
		if( file.is_open() == false ) {
			cerr << "CANNOT OPEN THE FILE " << argv[1] << endl;
			return 0;
		}

	}
	while (file >> tokItem)
	{
		cout << tokItem << endl;
	}
    	
    return 0;
}
