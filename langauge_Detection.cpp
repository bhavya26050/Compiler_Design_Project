#include <iostream>
#include <string>
#include <unordered_set>
using namespace std;
bool contains(const string &code, const string &keyword) {
    return code.find(keyword) != string::npos;
}

bool containsExact(const string &code, const string &keyword) {
    return code.find(keyword) != string::npos &&
           (code.find(keyword + " ") != string::npos ||
            code.find(keyword + "(") != string::npos ||
            code.find(keyword + ":") != string::npos);
}
bool isPythonConditional(const string &code) {
    return contains(code, "for ") && contains(code, "in range") || contains(code, "while ") ||
           contains(code, "if ") || contains(code, "elif ") || contains(code, "else:");
}

bool isCppConditional(const string &code) {
    return (contains(code, "for (") || contains(code, "while (") ||
            contains(code, "do {") || contains(code, "if (") ||
            contains(code, "else if (") || contains(code, "else {")) &&
           contains(code, "cout");
}

bool isJavaConditional(const string &code) {
    return (contains(code, "for (") || contains(code, "while (") ||
            contains(code, "do {") || contains(code, "if (") ||
            contains(code, "else if (") || contains(code, "else {")) &&
           (contains(code, "System.out.println") || contains(code, "public class") || contains(code, "static void main"));
}

bool isJavaScriptConditional(const string &code) {
    return (contains(code, "for (") || contains(code, "while (") ||
            contains(code, "if (") || contains(code, "else if (") || contains(code, "else {")) &&
            contains(code, "console.log");
}

void detectGeneralLanguage(const string &code, unordered_set<string> &detectedLanguages) {
    if (contains(code, "def") || containsExact(code, "print")) {
        detectedLanguages.insert("Python");
    }

    if (contains(code, "#include <iostream>") || contains(code, "cout") ||
        contains(code, "int main") || contains(code, "struct")) {
        detectedLanguages.insert("C++");
    }
    if (contains(code, "#include <stdio.h>") &&
        (contains(code, "printf") || contains(code, "scanf"))) {
        detectedLanguages.insert("C");
    }

    if (contains(code, "public class") || contains(code, "System.out.println") ||
        contains(code, "static void main")) {
        detectedLanguages.insert("Java");
    }

    if (contains(code, "<html>") || contains(code, "<body>") ||
        contains(code, "<div>") || contains(code, "<table>")) {
        detectedLanguages.insert("HTML");
    }

    if (contains(code, "function") || contains(code, "console.log") ||
        contains(code, "let") || contains(code, "const") || contains(code, "var")) {
        detectedLanguages.insert("JavaScript");
    }
}

string detectLanguage(const string &code) {
    unordered_set<string> detectedLanguages;

    detectGeneralLanguage(code, detectedLanguages);

    if (isPythonConditional(code)) {
        detectedLanguages.insert("Python");
    }
    if (isCppConditional(code)) {
        detectedLanguages.insert("C++");
    }
    if (isJavaConditional(code)) {
        detectedLanguages.insert("Java");
    }
    if (isJavaScriptConditional(code)) {
        detectedLanguages.insert("JavaScript");
    }

    if (detectedLanguages.size() > 1) {
        return "Error: Mixed Language Code";
    } else if (detectedLanguages.empty()) {
        return "Unknown Language";
    }

    return *detectedLanguages.begin();
}
 
int main() {
    string code;
    string line;
    cout << "Enter the block of code (type 'END' on a new line to finish):\n";
    while (true) {
        getline(cin, line);
        if (line == "END")
            break; 
        code += line + "\n";
    }
    string language = detectLanguage(code);
    cout << "The detected language is: " << language << endl;
    return 0;
}
 