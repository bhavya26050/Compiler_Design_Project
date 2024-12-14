// language_checker.h
#ifndef LANGUAGE_CHECKER_H
#define LANGUAGE_CHECKER_H

#include <string>
#include <unordered_set>

std::string detectLanguage(const std::string &code);
void runSyntaxChecks(const std::string &code);

#endif // LANGUAGE_CHECKER_H
