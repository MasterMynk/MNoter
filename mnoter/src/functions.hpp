#pragma once

#include <string>

#define RED    "\x1b[31m"
#define GREEN  "\x1b[32m"
#define YELLOW "\x1b[33m"
#define CYAN   "\x1b[34m"
#define PURPLE "\x1b[35m"
#define WHITE  "\x1b[37m"

#define BOLD  "\x1b[1m"
#define RESET "\x1b[0m"

void error(const char *const str);
bool isNum(const char *const str);
int toInt(const char *const str);
short int strLen(const char *const &str);
char *getLine(FILE *const &fp = stdin);
void copyLines(FILE *&from, FILE *&to, const short &numLines);
void skipLines(FILE *file, const short &lines);
short countNumLines(FILE *&file);
char lower(const char &letter);

void printDeleteBuff(FILE *&file, char *&buff);
void replaceTmpNotes(const char *const notesPath, const char *const tmpPath);

template <typename T>
void check(const T &val, const char *const message) {
    if (val)
        error(message);
}
