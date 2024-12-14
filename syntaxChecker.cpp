#include <iostream>
#include <stack>
#include <regex>
#include <string>
#include <unordered_set>
#include <sstream>
#include<unordered_map>
using namespace std;

unordered_set<string> declaredVariables;
unordered_set<string> validHeaders = {
    "iostream", "vector", "string", "map", "unordered_map", "algorithm", 
    "stack", "queue", "set", "cmath", "cstdlib", "cstdio", "climits", "cfloat"
};

bool checkHeaderSyntax(const string &code) {
    stringstream ss(code);
    string line;
    regex headerPattern(R"(#\s*include\s*[<\"](\w+)\.h?[>\"])");  // Match both `<...>` and `"..."`

    bool hasErrors = false;
    smatch match;

    while (getline(ss, line)) {
        if (regex_search(line, match, headerPattern)) {
            string headerName = match[1];
            if (validHeaders.find(headerName) == validHeaders.end()) {
                cout << "Error: Invalid or unsupported header: " << headerName << endl;
                hasErrors = true;
            }
        } else if (line.find("#include") != string::npos) {
            cout << "Error: Incorrect header syntax: " << line << endl;
            hasErrors = true;
        }
    }
    return hasErrors;
}

unordered_set<string> validKeywords = {
    "while", "for", "if", "else", "int", "float", "double", "char", 
    "string", "bool", "class", "struct", "return", "void", "public", 
    "private", "protected", "const", "static", "virtual"
};

unordered_map<string, string> keywordMisspellings = {
    {"iff", "if"}, {"elseif", "else if"}, {"elif", "else if"}, 
    {"wile", "while"}, {"fore", "for"}, {"mian", "main"}, 
    {"naim", "main"}, {"pubic", "public"}, {"privat", "private"}, 
    {"proteced", "protected"},{"cuot","cout"}
};

bool checkBalancedBrackets(const string &code) {
    stack<char> brackets;
    for (char ch : code) {
        if (ch == '{' || ch == '(' || ch == '[') {
            brackets.push(ch);
        } else if (ch == '}' || ch == ')' || ch == ']') {
            if (brackets.empty()) return false;
            char top = brackets.top();
            if ((ch == '}' && top != '{') || (ch == ')' && top != '(') || (ch == ']' && top != '[')) {
                return false;
            }
            brackets.pop();
        }
    }
    return brackets.empty();
}

bool checkMainFunction(const string &code) {
    return regex_search(code, regex(R"(\bint\s+main\s*\(\s*\))"));
}

bool checkKeywordMisspellings(const string &code) {
    stringstream ss(code);
    string word;
    bool hasError = false;

    while (ss >> word) {
        if (keywordMisspellings.find(word) != keywordMisspellings.end()) {
            cout << "Error: Possible misspelling of keyword '" << word
                 << "'. Did you mean '" << keywordMisspellings[word] << "'?" << endl;
            hasError = true;
        }
    }
    return !hasError;
}

bool checkVariableDeclarations(const string &line) {
    regex varPattern(R"(\b(?:int|float|double|char|string|bool)\s+(\w+)\s*(?:=|;|,|\s))");
    smatch match;
    bool valid = false;

    string tempLine = line;
    while (regex_search(tempLine, match, varPattern)) {
        declaredVariables.insert(match[1]);
        tempLine = match.suffix().str();
        valid = true;
    }
    return valid;
}

bool checkUndeclaredVariables(const string &line) {
    regex varUsePattern(R"(\b(\w+)\s*=)");
    smatch match;
    bool hasErrors = false;

    string tempLine = line;
    while (regex_search(tempLine, match, varUsePattern)) {
        string varName = match[1];
        if (declaredVariables.find(varName) == declaredVariables.end()) {
            cout << "Error: Undeclared variable: " << varName << endl;
            hasErrors = true;
        }
        tempLine = match.suffix().str();
    }
    return !hasErrors;
}

bool checkCinCoutOperators(const string &code) {
    regex coutMisuse(R"(cout\s*>>)");
    regex cinMisuse(R"(cin\s*<<)");

    bool hasErrors = false;

    if (regex_search(code, coutMisuse)) {
        cout << "Error: Incorrect operator '>>' used with 'cout'. Use '<<' instead." << endl;
        hasErrors = true;
    }
    if (regex_search(code, cinMisuse)) {
        cout << "Error: Incorrect operator '<<' used with 'cin'. Use '>>' instead." << endl;
        hasErrors = true;
    }
    return hasErrors;
}

bool checkOperators(const string &code) {
    regex assignmentInCondition(R"(\bif\s*\(\s*\w+\s*=\s*\w+\s*\))");
    bool hasErrors = false;

    if (regex_search(code, assignmentInCondition)) {
        cout << "Warning: Possible use of assignment (=) instead of comparison (==) in condition." << endl;
        hasErrors = true;
    }
    return hasErrors;
}

bool checkSemicolons(const string &code) {
    stringstream ss(code);
    string line;
    int lineNumber = 1;
    bool hasErrors = false;

    regex conditionalPattern(R"((if|while|for)\s*\(.+\)\s*;)");

    while (getline(ss, line)) {
        line = regex_replace(line, regex(R"(^\s+|\s+$)"), "");

        if (regex_search(line, conditionalPattern)) {
            cout << "Error: Conditional or loop statement should not end with a semicolon on line "
                 << lineNumber << ": " << line << endl;
            hasErrors = true;
        } 
        else if (!line.empty() && line.back() != ';' && line.back() != '{' && line.back() != '}') {
            cout << "Error: Missing semicolon on line " << lineNumber << ": " << line << endl;
            hasErrors = true;
        }
        lineNumber++;
    }
    return !hasErrors;
}

void runSyntaxChecks(const string &code) {
    if (!checkKeywordMisspellings(code)) return;
    if (!checkBalancedBrackets(code)) {
        cout << "Error: Mismatched or unbalanced brackets." << endl;
    }
    if (!checkMainFunction(code)) {
        cout << "Error: 'int main()' function is missing or incorrectly declared." << endl;
    }
    checkHeaderSyntax(code);

    stringstream ss(code);
    string line;
    while (getline(ss, line)) {
        checkVariableDeclarations(line);
        checkUndeclaredVariables(line);
    }
    checkSemicolons(code);
    checkOperators(code);
    checkCinCoutOperators(code);
    cout << "Syntax check completed." << endl;
}

int main() {
    string code;
    string line;
    cout << "Enter C++ code to check for syntax errors (type 'END' to finish):\n";
    while (true) {
        getline(cin, line);
        if (line == "END") break;
        code += line + "\n";
    }

    runSyntaxChecks(code);
    return 0;
}
