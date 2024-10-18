//=======================================================
// Name : Lex.cpp
// Description : A lexical analysizer that reads code 
// from a text file for the programming language called
// Mini C-Like(MCL). Made by Hermy Taveras
//=======================================================
#include <iostream>
#include <string>
#include "lex.h"
#include <algorithm>
#include <cctype>

// Function that checks the if a lexeme is a Keyword or Identifier
LexItem id_or_kw(const string& lexeme, int linenum) {
    static map<string, Token> keywords = {
        {"program", PROGRAM}, {"print", PRINT}, {"if", IF}, {"else", ELSE}, {"int", INT}, {"float", FLOAT},
        {"char", CHAR}, {"string", STRING}, {"bool", BOOL}, {"true", TRUE}, {"false", FALSE}
    };
    // Converts the lexeme to lowercase in order to search through the map of keywords
    string lowercase = lexeme;
    transform(lowercase.begin(), lowercase.end(), lowercase.begin(), ::tolower);

    auto it = keywords.find(lowercase);
    if (it != keywords.end()) {
        if (it->second == TRUE || it->second == FALSE) {
            return LexItem(BCONST, it->first, linenum);
        }
        return LexItem(it->second, it->first, linenum);
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

// output for Lexemes
ostream& operator<<(ostream& out, const LexItem& tok) {
    switch (tok.GetToken()) {
    case IDENT:
        out << "IDENT: <" << tok.GetLexeme() << ">";
        break;
    case CCONST:
        out << "CCONST: " << tok.GetLexeme();
        break;
    case SCONST:
        out << "SCONST: " << tok.GetLexeme() << "\"";
        break;
    case BCONST: case RCONST: case ICONST:
        out << TokToS(tok.GetToken()) << ": (" << tok.GetLexeme() << ")";
        break;
  
    case PROGRAM: case IF: case ELSE: case PRINT:
    case INT: case FLOAT: case BOOL: case CHAR: case STRING:
    case PLUS: case MINUS: case MULT: case DIV: case ASSOP:case EQ:
    case NEQ: case GTHAN: case LTHAN: case AND: case OR: case NOT:
    case ADDASSOP: case SUBASSOP: case MULASSOP: case DIVASSOP:
    case REMASSOP: case REM: case COMMA: case SEMICOL: case LPAREN:
    case RPAREN: case LBRACE: case RBRACE: case DOT:
        out << TokToS(tok.GetToken());
        break;  
    case ERR:
        out << "ERR: In line " << tok.GetLinenum() << ", Error Message {" << tok.GetLexeme() << "}" << endl;
        break;
    default:
        out << "TOKEN NOT RECOGNIZED: " << tok.GetLexeme();
    }
    return out;
}
// The function handling the analysis of each toke
LexItem getNextToken(std::istream& in, int& linenum) {
    char ch;
    std::string lexeme;

    enum TokState { START, INID, ININT, INREAL, INASSIGN, STRL1, STRL2} lexstate = START;

    while (in.get(ch)) {
        switch (lexstate) {
        case START: {
            // Handle whitespace
            if (isspace(ch)) {
                if (ch == '\n') linenum++;
                continue;
            }

            // Handle comments
            if (ch == '/') {
                char next = in.peek();
                if (next == '/') { // Is it a single line comment?
                    while (in.get(ch) && ch != '\n'); //Skips the entire line
                    linenum++;
                    continue;
                }
                else if (next == '*') { // Is it a multiline comment?
                    in.get();
                    while (in.get(ch)) {
                        if (ch == '*' && in.peek() == '/') { // Skips every token until the final "*/" is found
                            in.get();
                            break;
                        }
                        if (ch == '\n') linenum++;
                    }
                }
                else if (next == '=') { // Is it a division assignment operator?
                    in.get();
                    return LexItem(DIVASSOP, "/=", linenum);
                }
                else return LexItem(DIV, "/", linenum);
                continue;
            }

            // Handle string literal
            if (ch == '\'') {
                lexeme += ch;
                lexstate = STRL1;
                continue;
            }
            // Handle string literal
            if (ch == '"') {
                lexeme += ch;
                lexstate = STRL2;
                continue;
            }
            // Handle identifiers or keywords
            if (isalpha(ch) || ch == '_') {
                lexeme = ch; // Start lexeme with current character
                lexstate = INID;
                continue;
            }

            // Handle integers
            if (isdigit(ch)) {
                lexeme = ch; // Start lexeme with current character
                lexstate = ININT;
                continue;
            }
            // Handles plus signs
            if (ch == '+') { 
                char next = in.peek();
                if (isdigit(next) || next == '.') { // Is it a a integer?
                    lexeme = ch;
                    lexstate = ININT;
                    continue;
                }
                else if (next == '=') { // Is it a addition assignment operator?
                    in.get();
                    return LexItem(ADDASSOP, "+=", linenum);
                }
                else return LexItem(PLUS, "+", linenum); // Just a plus sign
            }
            //Handles minus signs
            if (ch == '-') {
                char next = in.peek();
                if (isdigit(next) || next == '.') { // Is it a integer?
                    lexeme = ch;
                    lexstate = ININT;
                    continue;
                }
                else if (next == '=') { //Is it a subtraction assignment operator?
                    in.get();
                    return LexItem(SUBASSOP, "-=", linenum);
                }
                else return LexItem(MINUS, "-", linenum); // Just a minus sign
            }
            // Handles dots - it could be a delimiter or part of a real number
            if (ch == '.') {
                char next = in.peek();
                if (isdigit(next)) {
                    lexeme = ch; // Start lexeme with dot
                    lexstate = INREAL;
                    continue;
                }
                else { // Just a dot
                    return LexItem(DOT, ".", linenum);
                }
            }
            // Handles multiplication
            if (ch == '*') { 
                char next = in.peek();
                if (next == '=') { // Is it a multiplication assignment operator?
                    in.get();
                    return LexItem(MULASSOP, "*=", linenum); 
                }
                else return LexItem(MULT, "*", linenum); // Just a multiplication sign
            }
            // Handles remainders
            if (ch == '%') {
                char next = in.peek();
                if (next == '=') { // Is it a remainder assignment operator?
                    in.get();
                    return LexItem(REMASSOP, "%=", linenum);
                }
                else return LexItem(REM, "%", linenum); //Just a remainder operator
            }
            //Handles NOT operator
            if (ch == '!') {
                char next = in.peek();
                if (next == '=') { // Is it a NOT EQUAL operator?
                    in.get();
                    return LexItem(NEQ, "!=", linenum);
                }
                else return LexItem(NOT, "!", linenum); //Just a NOT
            }
            //Handles AND operator
            if (ch == '&') {
                char next = in.peek();
                if (next == '&') {
                    in.get();
                    return LexItem(AND, "&&", linenum);
                }
                else return LexItem(ERR, "&", linenum); // If the operator is not two &'s, it returns an error

            }
            // Handles OR operator
            if (ch == '|') {
                char next = in.peek();
                if (next == '|') {
                    in.get();
                    return LexItem(OR, "||", linenum);
                }
                else return LexItem(ERR, "|", linenum); // If the operator is not two |'s it returns an error

            }
            // Handle simple single-character tokens
            switch (ch) {
            case '(': return LexItem(LPAREN, "(", linenum);
            case ')': return LexItem(RPAREN, ")", linenum);
            case '{': return LexItem(LBRACE, "{", linenum);
            case '}': return LexItem(RBRACE, "}", linenum);
            case ',': return LexItem(COMMA, ",", linenum);
            case ';': return LexItem(SEMICOL, ";", linenum);
            case '>': return LexItem(GTHAN, ">", linenum);
            case '<': return LexItem(LTHAN, "<", linenum);
            case '=':
                lexstate = INASSIGN;
                lexeme = ch;
                continue;
            default:
                return LexItem(ERR, string(1, ch), linenum);
            }
        }
        // Handles Identifiers and Keywords
        case INID:
            in.putback(ch);
            while (in.get(ch) && (isalnum(ch) || ch == '_')) {
                lexeme += ch; // Accumulate all valid identifier characters
            }
            in.putback(ch); // Put back the character that broke the loop
            return id_or_kw(lexeme, linenum); // Uses the id_or_kw function to determine whether the lexeme is a Identifier or keyword
        // Handles Integers
        case ININT:
            in.putback(ch);
            while (in.get(ch) && (isdigit(ch))) {
                lexeme += ch; // Accumulate all digits
            }
            if (ch == '.') { // Is it a real number?
                char next = in.peek();
                if (isdigit(next)) {
                    lexstate = INREAL;
                    lexeme += ch;
                    continue;
                }
                else { // Returns the "." and stores the integer lexeme
                    in.putback(ch);
                    return LexItem(ICONST, lexeme, linenum);
                }
            }
            in.putback(ch);
            return LexItem(ICONST, lexeme, linenum);
        // Handles real numbers
        case INREAL:
            in.putback(ch);
            while (in.get(ch) && isdigit(ch)) {
                lexeme += ch; // Accumulate all digits after the decimal point
            }
            if (ch == '.') {
                lexeme += ch;
                return LexItem(ERR, lexeme, linenum);
            }
            in.putback(ch);
            return LexItem(RCONST, lexeme, linenum);
        // Handles the Equality Operator
        case INASSIGN:
            in.putback(ch);
            if (in.peek() == '=') {
                in.get();
                return LexItem(EQ, "==", linenum);
            }
            return LexItem(ASSOP, lexeme, linenum);
        // Handles character constants marked by ' '
        case STRL1:
            in.putback(ch);
            if (!in.get(ch)) {
                return LexItem(ERR, lexeme, linenum);
            }
            if (ch == '\n') { // Returns an error if a new line is started before the character constant is closed
               return LexItem(ERR, "New line is an invalid character constant.", linenum);
             }
            lexeme += ch;
            if (!in.get(ch)) { 
                return LexItem(ERR, lexeme, linenum);
            }
            lexeme += ch;
            if (ch == '\n') {
                return LexItem(ERR, "New line is an invalid character constant.", linenum);
            }
            if (ch == '\'') { // Checks for the end of the character constant
               return LexItem(CCONST, lexeme, linenum);
             }
            else { // At this point its not valid character constant
                lexeme += '\'';
                return LexItem(ERR, " Invalid character constant " + lexeme, linenum);
            }
        // Handles string constants marked by " "
        case STRL2:
            in.putback(ch);
            while (in.get(ch)) {
                if (ch == '"') { // Looks for the end of the string constant
                    return LexItem(SCONST, lexeme, linenum);
                }
                if (ch == '\n') { // Returns an error if a new line is started before the string constant is closed
                    return LexItem(ERR, " Invalid string constant " + lexeme, linenum);
                }
                lexeme += ch; // Accumulate all string characters
            }
            return LexItem(ERR, " Invalid string constant " + lexeme, linenum);
        
        }
    }
    //Marks the end of the file
    return LexItem(DONE, "", linenum);
}
