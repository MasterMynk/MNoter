#pragma once

#include <filesystem>

void add(char **const &argv, const short &argc, const char *const &notesPath);
void show(const char *const &notesPath);
void remove(const short notes[], const short &len, const std::string &homeDir,
            const char *const &notesPath);
void swap(const short &from, const short &to, const std::string &homeDir,
          const char *const &notesPath);
void help();

void error(const char *const str);
bool isNum(const char *const str);
int toInt(const char *const str);
short int strLen(const char *const &str);
char *getLine(FILE *const &fp = stdin);
void copyLines(FILE *&from, FILE *&to, const short &numLines);
void skipLines(FILE *file, const short &lines);
short countNumLines(FILE *&file);

void printDeleteBuff(FILE *&file, char *&buff);

template <typename T>
void check(const T &val, const char *const message) {
    if (val)
        error(message);
}
