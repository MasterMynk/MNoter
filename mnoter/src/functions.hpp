#pragma once

#include <filesystem>

void add(const int &argc, char **const &argv, const char *const &notesPath,
         const char *const &numPath);
void show(const char *const &notesPath);
void remove(const char *const index, const std::string &homeDir, const std::string &notesPath,
            const char *const &numPath);
void swap(const short &from, const short &to, const char *const &homeDir,
          const char *const &notesPath, const char *const &numPath);
void help();

void error(const char *const str);
void error(const char *const str, const short &num);

bool isNum(const char *const str);
int toInt(const char *const str);
short int strLen(const char *const &str);
char *getLine(FILE *const &fp = stdin);
void copyFileLines(FILE *&from, FILE *&to, const short &numLines);

void copyColor(FILE *&from, FILE *&to);
void printDeleteBuff(FILE *&file, char *&buff);
void skipLines(FILE *file, const short &lines);

template <typename T>
void check(const T &val, const char *const message) {
    if (val)
        error(message);
}
