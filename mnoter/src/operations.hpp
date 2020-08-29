#pragma once

#include <filesystem>

#include "functions.hpp"

void add(char **const &argv, const short &argc, const char *const &notesPath);
void show(const char *const &notesPath);
void remove(char **const &notes, short len, const std::string &homeDir,
            const char *const &notesPath);
void swap(char **const &argv, const short &len, const std::string &homeDir,
          const char *const &notesPath);
void move(char **const &argv, const short &len, const std::string &homeDir,
          const char *const &notesPath);
void edit(const char *const &notesPath, const char *const &editor);
void change(const short &note, char **const &toChangeWith, const short &len,
            const std::string &homeDir, const char *const &notesPath);
void clear(const char *const &notesPath);
void backup(const char *const path, const char *const notesPath);