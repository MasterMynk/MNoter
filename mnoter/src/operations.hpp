#pragma once

#include <filesystem>

#include "functions.hpp"

#define SILENT_BIT   0b00000001
#define NO_ASK_BIT   0b00000010
#define FOUND_OP_BIT 0b00000100

void add(char **const &argv, const short &argc, const char *const &notesPath);
void show(const char *const &notesPath);
void remove(char **const &notes, short len, const std::string &homeDir,
            const char *const &notesPath, uint8_t &flags);
void swap(char **const &argv, short len, const std::string &homeDir, const char *const &notesPath,
          uint8_t &flags);
void move(char **const &argv, const short &len, const std::string &homeDir,
          const char *const &notesPath, uint8_t &flags);
void edit(char **const &argv, const short &len, const char *const &notesPath, char *&editorStr,
          uint8_t &flags);
void change(char **argv, short len, const std::string &homeDir, const char *const &notesPath);
void clear(char **const &argv, const short &len, const char *const &notesPath, uint8_t &flags);
void backup(char **const &argv, const short &len, const char *const notesPath, uint8_t &flags);