#pragma once

#include <filesystem>

void add(const int &argc, char **const &argv, const char *const &notesPath,
         const char *const &numPath);
void show(const char *const &notesPath);
void remove(const char *const index, const std::string &homeDir, const std::string &notesPath,
            const char *const &numPath);
void help();

void error(const char *const str, const bool &shouldExit = true, const short &exitCode = -1);

bool isNum(const char *const num);
int toInt(const char *const str);
short int strLen(const char *const &str);
char *getLine(FILE *const &fp = stdin);

template <typename T>
void check(const T &val, const char *const message) {
    if (val)
        error(message);
}
