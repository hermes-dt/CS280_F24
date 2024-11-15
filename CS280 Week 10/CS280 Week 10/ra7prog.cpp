#include <iostream>
#include <fstream>
#include <stack>
#include <string>

using namespace std;


int BalancedBrackets(string strInput) {
    stack<char> Brackets;  
    bool hasBrackets = false; 

   
    for (char chrSymbol : strInput) {
        
        if (chrSymbol == '{' || chrSymbol == '(' || chrSymbol == '[') {
            Brackets.push(chrSymbol);
            hasBrackets = true;  
        }

        else if (chrSymbol == '}' || chrSymbol == ')' || chrSymbol == ']') {
            hasBrackets = true;  
            if (Brackets.empty()) {
                cout << "case 4: There are more closing brackets than opening ones for one or more bracket types in the input string." << endl;
                return -1;
            }
            char topBracket = Brackets.top();
            
            if ((chrSymbol == '}' && topBracket == '{') ||
                (chrSymbol == ')' && topBracket == '(') ||
                (chrSymbol == ']' && topBracket == '[')) {
                Brackets.pop();
            }
            else {  
                cout << "case 3: Mismatching bracket type between an opening and a closing bracket." << endl;
                return -1;
            }
        }
    }

    
    if (Brackets.empty() && hasBrackets) {
        cout << "case 1: Scanning the input string is completed successfully." << endl;
        return 1;
    }
    if (!Brackets.empty()) {
        cout << "case 2: There are more opening brackets than closing ones for one or more bracket types in the input string." << endl;
        return -1;
    }
    cout << "case 5: Scanning the input string is completed successfully." << endl;
    return 0;
}

int main(int argc, char* argv[])
{
    string sentence;
    int balanced;

    ifstream file;
    if (argc >= 2) {
        file.open(argv[1]);
        if (!file)
        {
            /* error */
            cerr << "File cannot be opened: " << argv[1] << endl;
            exit(1);
        }

    }
    else
    {
        cerr << "No file is found" << endl;
        exit(1);
    }

    getline(file, sentence);
    if (sentence.length() == 0)
    {
        cout << "File is empty." << endl;
        return 0;
    }

    balanced = BalancedBrackets(sentence);
    cout << endl;

    if (balanced == 1)
        cout << "The following sentence contains balanced brackets: " << sentence << endl;
    else if (balanced == -1)
        cout << "The following sentence does not contain balanced brackets: " << sentence << endl;
    else
        cout << "The following sentence does not contain any brackets: " << sentence << endl;
}
