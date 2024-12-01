//
//
//
//

#include "val.h"
#include "parserInterp_Fall24.h"

bool Expr(istream& in, int& line, Value& retVal) {
    Value val1;
    if (!LogANDExpr(in, line, val1)) {
        return false;
    }

    LexItem tok;
    while (true) {
        tok = Parser::GetNextToken(in, line);
        if (tok == OR) {
            Value val2;
            if (!LogANDExpr(in, line, val2)) {
                ParseError(line, "Missing Logical AND Expression after '||'");
                return false;
            }
            // Perform the OR operation
            try {
                val1 = val1 || val2;
            }
            catch (const char* err) {
                ParseError(line, err);
                return false;
            }
        }
        else {
            Parser::PushBackToken(tok);
            break;
        }
    }

    retVal = val1;
    return true;
}

bool VarList(istream& in, int& line, LexItem& idtok) {
    do {
        LexItem tok;
        Value val;
        if (!Var(in, line, idtok)) {
            return false;
        }

        tok = Parser::GetNextToken(in, line);
        if (tok == ASSOP) {
            if (!Expr(in, line, val)) {
                ParseError(line, "Invalid initialization expression");
                return false;
            }
            SymTable[idtok.GetLexeme()] = val;
        }

        tok = Parser::GetNextToken(in, line);
        if (tok != COMMA) {
            Parser::PushBackToken(tok);
            break;
        }
    } while (true);

    return true;
}
