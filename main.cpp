// main.cpp
#include <iostream>
#include <string>
#include "language_checker.h" // Include the header for language detection and syntax checking

using namespace std;

int main() {
    string code;
    string line;

    // Language Detection
    cout << "Enter the block of code for language detection (type 'END' on a new line to finish):\n";
    while (true) {
        getline(cin, line);
        if (line == "END")
            break; 
        code += line + "\n";
    }
    string language = detectLanguage(code);
    cout << "The detected language is: " << language << endl;

    // Syntax Checking
    code.clear(); // Clear the code variable for syntax checking
    cout << "\nEnter C++ code to check for syntax errors (type 'END' to finish):\n";
    while (true) {
        getline(cin, line);
        if (line == "END") break;
        code += line + "\n";
    }

    runSyntaxChecks(code);
    return 0;
}
