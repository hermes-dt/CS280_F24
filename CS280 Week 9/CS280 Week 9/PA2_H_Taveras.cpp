/* Implementation of Recursive-Descent Parser
 * for Mini C-Like Language
 * Programming Assignment 2
 * Fall 2024
*/
//-----------------------------------------------
//Filename: PA_H_Taveras.cpp
//Author: Hermy Taveras
//Description:
//-----------------------------------------------

#include "parser.h"
#include <vector>

map<string, bool> defVar;
map<string, Token> SymTable;
vector<string> varNames;

namespace Parser {
	bool pushed_back = false;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if (pushed_back) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem& t) {
		if (pushed_back) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;
	}

}

static int error_count = 0;
int nesting_level = 0;
bool declStmtOn = false;

int ErrCount()
{
	return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}



//PrintStmt:= PRINT (ExprList) 
bool PrintStmt(istream& in, int& line) {
	LexItem t = Parser::GetNextToken(in, line);
	//cout << "in PrintStmt" << endl;

	if (t != LPAREN) {

		ParseError(line, "Missing Left Parenthesis");
		return false;
	}

	bool ex = ExprList(in, line);

	if (!ex) {
		ParseError(line, "Missing expression list after Print");
		return false;
	}

	t = Parser::GetNextToken(in, line);
	if (t != RPAREN) {

		ParseError(line, "Missing Right Parenthesis");
		return false;
	}

	return true;
}//End of PrintStmt


//ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line) {
	bool status = Expr(in, line);
	if (!status) {
		ParseError(line, "Missing Expression");
		return false;
	}

	LexItem tok;

	while (true) {
		tok = Parser::GetNextToken(in, line);
		if (tok == COMMA) {
			status = Expr(in, line);
			if (!status) {
				ParseError(line, "Missing Expression after ','");
				return false;
			}
		}
		else {
			Parser::PushBackToken(tok);
			break;
		}
	}
	return true;

}//End of ExprList

 //Expr:= LogANDExpr{||LogANDExpr}
bool Expr(istream& in, int& line) {
	bool status = LogANDExpr(in, line);
	if (!status) {
		return false;
	}

	LexItem tok;

	while (true) {
		tok = Parser::GetNextToken(in, line);
		if (tok == OR) {
			status = LogANDExpr(in, line);
			if (!status) {
				ParseError(line, "Missing Logical AND Expression after '||'");
				return false;
			}
		}
		else {
			Parser::PushBackToken(tok);
			break;
		}
	}
	return true;
}//End of Expr

 //LogANDExpr::= EqualExpr {&&EqualExpr}
bool LogANDExpr(istream& in, int& line) {
	bool status = false;
	//cout << "in LogANDExpr and before calling EqualExpr" << endl;
	status = EqualExpr(in, line);

	if (!status) {
		return false;
	}

	LexItem tok;

	while (true) {
		tok = Parser::GetNextToken(in, line);
		if (tok == AND) {
			status = EqualExpr(in, line);
			if (!status) {
				ParseError(line, "Missing Equal Expression after '&&'");
				return false;
			}
		}
		else {
			Parser::PushBackToken(tok);
			break;
		}
	}
	return true;
}

// EqualExpr::= RelExpr[(==|!=)RelExpr]
bool EqualExpr(istream& in, int& line) {
	bool status = RelExpr(in, line);
	//cout << "in EqualExpr and before calling RelExpr" << endl;

	LexItem tok = Parser::GetNextToken(in, line);

	if ((tok == EQ) || (tok == NEQ)) {
		//cout << "before calling RelExpr" << endl;
		status = RelExpr(in, line);
		if (!status) {
			ParseError(line, "Missing expression after equals ");
			return false;
		}

		tok = Parser::GetNextToken(in, line);
		if (tok == EQ || tok == NEQ) {
			ParseError(line, "Illegal Equality Expression");
			return false;
		}
		Parser::PushBackToken(tok);
	}
	else {
		Parser::PushBackToken(tok);

	}
	return status;
}

// RelExpr::= AddExpr[(<|>)AddExpr]
bool RelExpr(istream& in, int& line) {
	bool status = AddExpr(in, line);
	//cout << "in RelExpr and before calling calling AddExpr" << endl;
	if (!status) {
		return false;
	}

	LexItem tok = Parser::GetNextToken(in, line);

	if ((tok == GTHAN) || (tok == LTHAN)) {
		//cout << "before calling AddExpr" << endl;
		status = AddExpr(in, line);
		if (!status) {
			ParseError(line, "Missing expression after '>' or '<'");
			return false;
		}
		//cout << "after calling AddExpr" << endl;

		tok = Parser::GetNextToken(in, line);
		if (tok == GTHAN || tok == LTHAN) { //Preventing cascading here -.-
			ParseError(line, "Illegal relational operator");
			return false;
		}
		Parser::PushBackToken(tok);
	}
	else {
		Parser::PushBackToken(tok);
	}
	return status;
}
// AddExpr::= MultExpr{(+|-)MultExpr}
bool AddExpr(istream& in, int& line) {
	bool status = false;
	//cout << "in AddExpr and before calling calling MultExpr" << endl;
	status = MultExpr(in, line);

	if (!status) {
		return false;
	}

	LexItem tok;

	while (true) {
		tok = Parser::GetNextToken(in, line);
		if (tok == PLUS || tok == MINUS) {
			//cout << "before calling 2nd MultExpr" << endl;
			status = MultExpr(in, line);
			if (!status) {
				ParseError(line, "Missing expression after '+' or '-'");
				return false;
			}
		}
		else {
			//cout << "after calling MultExpr" << endl;
			Parser::PushBackToken(tok);
			break;
		}
	}
	return status;
}

// MultExpr::= UnaryExpr{(*|/|%)UnaryExpr}
bool MultExpr(istream& in, int& line) {
	bool status = false;

	status = UnaryExpr(in, line);

	if (!status) {
		return false;
	}

	LexItem tok;

	while (true) {
		tok = Parser::GetNextToken(in, line);
		if (tok == MULT || tok == DIV || tok == REM) {
			status = UnaryExpr(in, line);
			if (!status) {
				ParseError(line, "Missing operand after operator");
				return false;
			}
		}
		else {

			Parser::PushBackToken(tok);
			break;
		}
	}
	return true;
}

// UnaryExpr::= [(-|+|!)]PrimaryExpr
bool UnaryExpr(istream& in, int& line) {
	bool status = false;
	//cout << "in UnaryExpr and before calling checking for expression symbols" << endl;
	LexItem tok = Parser::GetNextToken(in, line);
	Token op = ERR;

	if ((tok == MINUS) || (tok == PLUS) || (tok == NOT)) {
		op = tok.GetToken();
	}
	else {
		Parser::PushBackToken(tok);
	}
	//cout << "before calling PrimaryExpr" << endl;
	status = PrimaryExpr(in, line, op);
	if (!status) {
		if (op != ERR) {
			ParseError(line, "Missing primary expression after unary operator");
		}
		return false;
	}
	return true;
}

// PrimaryExpr::= IDENT | ICONST | RCONST | SCONST | BCONST | CCONST | (Expr)
bool PrimaryExpr(istream& in, int& line, int sign) {
	LexItem tok = Parser::GetNextToken(in, line);

	if (tok == IDENT) {
		// Validate if the identifier is defined (from defVar map, for example)
		if (SymTable.find(tok.GetLexeme()) == SymTable.end()) {
			ParseError(line, "Undeclared Variable");
			return false;
		}
	}

	if (tok == IDENT || tok == ICONST || tok == RCONST || tok == SCONST || tok == BCONST || tok == CCONST) {
		return true;
	}

	if (tok == LPAREN) {
		//cout << "after LPAREN before calling Expr" << endl;
		if (!Expr(in, line)) {
			ParseError(line, "Missing expression after Left Parenthesis");
			return false;
		}
		//cout << "before RPAREN after calling Expr" << endl;
		tok = Parser::GetNextToken(in, line);
		if (tok != RPAREN) {
			ParseError(line, "Missing right Parenthesis after expression");
			Parser::PushBackToken(tok);
			return false;
		}
		return true;

	}
	Parser::PushBackToken(tok);
	//ParseError(line, "Unrecognized primary expression");
	return false;
}

//Prog:= PROGRAM IDENT CompStmt
bool Prog(istream& in, int& line) {
	LexItem tok = Parser::GetNextToken(in, line);

	if (tok != PROGRAM) {
		ParseError(line, "Missing Program Keyword");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != IDENT) {
		ParseError(line, "Missing Program name");
		return false;
	}
	// Parse the compound statement
	if (!CompStmt(in, line)) {
		ParseError(line, "Invalid Program");
		return false;
	}
	cout << "(DONE)" << endl;
	return true;

}

//CompStmt::= '{' StmtList '}'
bool CompStmt(istream& in, int& line) {
	LexItem tok = Parser::GetNextToken(in, line);

	if (tok != LBRACE) {  // Assuming LBRACE represents '{'
		//ParseError(line, "Missing '{' at the beginning of compound statement");
		return false;
	}

	if (!StmtList(in, line)) {
		ParseError(line, "Incorrect statement list");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok != RBRACE) {  // Assuming RBRACE represents '}'
		//ParseError(line, "Missing '}' at the end of compound statement");
		return false;
	}

	return true;
}

//StmtList::= Stmt{Stmt}
bool StmtList(istream& in, int& line) {
	while (true) {
 if (!Stmt(in, line)) {
            LexItem tok = Parser::GetNextToken(in, line);
            if (tok == RBRACE || tok == DONE) {
                Parser::PushBackToken(tok);
                return true;
          
            } else {
                ParseError(tok.GetLinenum(), "Syntactic error in statement list.");
                return false;
            }
        }

	}

	return true;
}

//Stmt::= DeclStmt | Control Stmt | CompStmt
bool Stmt(istream& in, int& line) {
	LexItem tok = Parser::GetNextToken(in, line);
	Parser::PushBackToken(tok);


	if (tok == INT || tok == FLOAT || tok == BOOL || tok == CHAR || tok == STRING) {
		if (!DeclStmt(in, line)) {
			return false;  
		}
		return true;
	}

	if (tok == IDENT || tok == IF || tok == PRINT) {
		if (!ControlStmt(in, line)) {
			ParseError(line, "Invalid control statement");
			return false;
		}
		return true;
	}

	if (tok == LBRACE) {
		if (!CompStmt(in, line)) {
			ParseError(line, "Invalid compound statement");
			return false;
		}
		return true;
	}

	
	return false;

}

//DeclStmt::= (INT | FLOAT | BOOL | CHAR | STRING)VarList;
bool DeclStmt(istream& in, int& line) {
	LexItem tok = Parser::GetNextToken(in, line);

	if (tok == INT || tok == FLOAT || tok == BOOL || tok == CHAR || tok == STRING) {
		Token vType = tok.GetToken();
		declStmtOn = true;

		//cout << "before calling VarList" << endl;

		if (!VarList(in, line)) {
			ParseError(line, "Incorrect variable in Declaration Statement.");
			ParseError(line, "Invalid declaration statement.");
			declStmtOn = false;
			return false;
		}
		declStmtOn = false;
		//cout << "after calling VarList" << endl;

		tok = Parser::GetNextToken(in, line);
		if (tok != SEMICOL) {
			ParseError(line, "Missing semicolon at the end of declaration");
			return false;
		}

		//cout << "after semi colon" << endl;

		for (const std::string& varName : varNames) {
			SymTable[varName] = vType;
		}
		return true;

	}
	Parser::PushBackToken(tok);
	return false;
}

//VarList::= Var [=Expr] {,Var[=Expr]};
bool VarList(istream& in, int& line) {
	varNames.clear();
	do {
		if (!Var(in, line)) {
			//ParseError(line, "Invalid variable in variable list");
			return false;
		}

		LexItem tok = Parser::GetNextToken(in, line);

		if (tok == ASSOP) {
			if (!Expr(in, line)) {
				ParseError(line, "Invalid initialization expression");
				return false;
			}
			std::cout << "Initialization of the variable " << varNames.back()
				<< " in the declaration statement at line " << line << std::endl;

			defVar[varNames.back()] = true;

			tok = Parser::GetNextToken(in, line);
		}
		else defVar[varNames.back()] = false;


		if (tok != COMMA) {
			Parser::PushBackToken(tok);
			break;
		}
	} while (true);

	return true;
}

//Var::= IDENT;
bool Var(istream& in, int& line) {
	LexItem tok = Parser::GetNextToken(in, line);
	if (tok == IDENT) {
		string varName = tok.GetLexeme();
		if (declStmtOn) { //For declaration statements
			if (SymTable.find(varName) != SymTable.end()) {
				ParseError(line, "Variable Redefinition");
				return false;
			}
			varNames.push_back(varName);
		}
		else { //For Undefined variables
			if (SymTable.find(varName) == SymTable.end()) {
				ParseError(line, "Undeclared Variable");
				return false;
			}
		}
		return true;
	}
	return false;
}

//ControlStmt::= AssgnStmt; | IfStmt | PrintStmt;
bool ControlStmt(istream& in, int& line) {
	int saveLine = line;
	LexItem tok = Parser::GetNextToken(in, line);
	Parser::PushBackToken(tok);

	if (tok == IDENT) {
		if (!AssignStmt(in, line)) {
			ParseError(line, "Incorrect Assignment Statement");
			return false;
		}
		tok = Parser::GetNextToken(in, line);
		if (tok != SEMICOL) {
			ParseError(saveLine, "Missing semicolon at end of statement.");
			return false;
		}
		return true;
	}
	else if (tok == IF) {
		if(!IfStmt(in, line)){
			ParseError(line, "Missing if statement condition");
			return false;
		}
		return true;
	}
	else if (tok == PRINT) {
		tok = Parser::GetNextToken(in, line);
		if (!PrintStmt(in, line)) {
			ParseError(line, "Incorrect Print Statement");
			return false;
		}
		tok = Parser::GetNextToken(in, line);
		if (tok != SEMICOL) {
			ParseError(saveLine, "Missing semicolon at end of statement.");
			return false;
		}
		return true;
	}

	ParseError(line, "Invalid control statement");
	return false;
}
//AssignStmt::= Var ( = | += | *= | /= | %=) Expr
bool AssignStmt(istream& in, int& line) {
	LexItem tok; //= Parser::GetNextToken(in, line);
	//string varName;

	if (!Var(in, line)) {
		ParseError(line, "Missing Left - Hand Side Variable in Assignment statement");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (!(tok == ASSOP || tok == ADDASSOP || tok == MULASSOP || tok == DIVASSOP || tok == REMASSOP)) {
		ParseError(line, "Missing Assignment Operator");
		Parser::PushBackToken(tok);
		return false;
	}

	if (!Expr(in, line)) {
		ParseError(line, "Missing expression in Assignment Statement");
		return false;
	}

	//defVar[varName] = true;
	return true;
}
//IfStmt::= IF (Expr) Stmt [ELSE Stmt]
bool IfStmt(istream& in, int& line) {
	LexItem tok = Parser::GetNextToken(in, line);

	if (tok != IF) {
		Parser::PushBackToken(tok);
		return false;
	}

	tok = Parser::GetNextToken(in, line);

	if (tok != LPAREN) {
		ParseError(line, "Missing left parenthesis in IF statement");
		return false;
	}

	if (!Expr(in, line)) {
		ParseError(line, "Incorrect IF Statement");
		return false;
	}

	tok = Parser::GetNextToken(in, line);

	if (tok != RPAREN) {
		ParseError(line, "Missing right parenthesis in IF statement");
		return false;
	}

	nesting_level++;

	cout << "in IfStmt then-clause at nesting level: " << nesting_level << endl;

	if (!Stmt(in, line)) {
		nesting_level--;
		ParseError(line, "Incorrect IF Statement");
		return false;
	}

	tok = Parser::GetNextToken(in, line);

	if (tok == ELSE) {
		cout << "in IfStmt else-clause at nesting level: " << nesting_level << endl;
		if (!Stmt(in, line)) {
			nesting_level--;
			return false;
		}
	}
	else Parser::PushBackToken(tok);

	nesting_level--;
	return true;
}