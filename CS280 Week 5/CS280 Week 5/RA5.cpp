//=======================================================
// Name : RA5.cpp
// Description : Made by Hermy Taveras
//=======================================================
#include <iostream>
#include <string>
#include <map>
#include "lex.h"


LexItem id_or_kw(const string& lexeme, int linenum) {
	static map<string, Token> keywords = {
		{"program", PROGRAM}, {"print", PRINT}, {"if", IF}, {"else", ELSE}, {"int", INT}, {"float", FLOAT},
		{"char", CHAR}, {"string", STRING}, {"bool", BOOL}, {"true", TRUE}, {"false", FALSE}
	};

	auto it = keywords.find(lexeme);
	if (it != keywords.end()) {
		if (it->second == TRUE || it->second == FALSE) {
			return LexItem(BCONST, lexeme, linenum);
		}
		return LexItem(it->second, lexeme, linenum);
	}
	return LexItem(IDENT, lexeme, linenum);
}

// Function that converts Token enums to string
std::string TokToS(Token token) {
    switch (token) {
    case PROGRAM: return "PROGRAM"; case IF: return "IF"; case ELSE: return "ELSE";
    case PRINT: return "PRINT"; case INT: return "INT"; case FLOAT: return "FLOAT";
    case BOOL: return "BOOL"; case CHAR: return "CHAR"; case STRING: return "STRING";
    case PLUS: return "PLUS"; case MINUS: return "MINUS"; case MULT: return "MULT";
    case DIV: return "DIV"; case ASSOP: return "ASSOP"; case EQ: return "EQ";
    case NEQ: return "NEQ"; case GTHAN: return "GTHAN"; case LTHAN: return "LTHAN";
    case AND: return "AND"; case OR: return "OR"; case NOT: return "NOT";
    case ADDASSOP: return "ADDASSOP"; case SUBASSOP: return "SUBASSOP"; case MULASSOP: return "MULASSOP";
    case DIVASSOP: return "DIVASSOP"; case REMASSOP: return "REMASSOP";
    case REM: return "REM"; case COMMA: return "COMMA"; case SEMICOL: return "SEMICOL";
    case LPAREN: return "LPAREN"; case RPAREN: return "RPAREN"; case LBRACE: return "LBRACE"; 
    case RBRACE: return "RBRACE"; case DOT: return "DOT"; case RCONST: return "RCONST";
    case ICONST: return "ICONST"; case BCONST: return "BCONST";
    default: return "UNKOWN";
    }
}

//output for Lexemes
ostream& operator<<(ostream& out, const LexItem& tok) { 
    switch (tok.GetToken()) {
    case IDENT:
        out << "IDENT: <" << tok.GetLexeme() << ">";
        break;
    case CCONST:
        out << "CCONST: '" << tok.GetLexeme() << "'";
        break;
    case SCONST:
        out << "SCONST: \"" << tok.GetLexeme() << "\"";
        break;
    case BCONST: case RCONST: case ICONST: 
        out << TokToS(tok.GetToken()) << ": (" << tok.GetLexeme() << ")";
        break;
    case ERR:
        out << "ERR: {" << tok.GetLexeme() << "} error at line no. " << tok.GetLinenum();
        break;
    case PROGRAM: case IF: case ELSE: case PRINT:
    case INT: case FLOAT: case BOOL: case CHAR: case STRING:
        out << TokToS(tok.GetToken()) << ": KEYWORD";
        break;
    case PLUS: case MINUS: case MULT: case DIV: case ASSOP:case EQ: 
    case NEQ: case GTHAN: case LTHAN: case AND: case OR: case NOT:
    case ADDASSOP: case SUBASSOP: case MULASSOP: case DIVASSOP: 
    case REMASSOP: case REM: case COMMA: case SEMICOL: case LPAREN: 
    case RPAREN: case LBRACE: case RBRACE: case DOT:
        out << TokToS(tok.GetToken()) << ": [" << tok.GetLexeme() << "]";
        break;
    default:
        out << "TOKEN NOT RECOGNIZED: " << tok.GetLexeme();
    }
    return out;
}




