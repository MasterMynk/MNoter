#pragma once

#include "functions.hpp"

void add(char **const &argv, const short &argc, const char *const &notesPath);
void show(const char *const &notesPath);
void remove(const short notes[], const short &len, const std::string &homeDir,
            const char *const &notesPath);
void swap(const short &from, const short &to, const std::string &homeDir,
          const char *const &notesPath);
void move(const short &from, const short &to, const std::string &homeDir,
          const char *const &notesPath);
void edit(const char *const &notesPath, const char *const &editor);
void change(const short &note, char **const &toChangeWith, const short &len,
            const std::string &homeDir, const char *const &notesPath);
