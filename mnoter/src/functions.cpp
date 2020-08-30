#include "functions.hpp"

#include <iostream>

void error(const char *const str) {
    printf("%s%sERROR: %s%s%s\n%s", RED, BOLD, RESET, RED, str, RESET);
    exit(-1);
}

bool isNum(const char *const str) {
    short i = 0;

    for (; i < strLen(str); ++i)
        if (!(str[i] >= 48 && str[i] <= 57)) {
            i = strLen(str) + 1;
            break;
        }

    return i != (strLen(str) + 1);
}

short int strLen(const char *const &str) {
    unsigned short int index;

    for (index = 0; (str[index] != '\0'); ++index)
        ;
    return index;
}

int toInt(const char *const str) {
    int num = 0;

    for (short int i = 0; i < strLen(str); ++i) {
        num *= 10;
        if ((str[i] >= 48 && str[i] <= 57))
            num += (str[i] - 48);
    }

    return num;
}

#define COMMON_WORD_LEN 5
char *getLine(FILE *const &fp) {
    char *str = nullptr;
    short stringLen = 0, memLen = COMMON_WORD_LEN;

    str = new char[COMMON_WORD_LEN]{0};
    if (!str)
        throw "Couldn't allocate memory!!\n";

    while (true) {
        if (stringLen >= memLen - 1) {
            str = (char *)realloc(str, (COMMON_WORD_LEN + memLen) * sizeof(char));
            memLen += COMMON_WORD_LEN;
        }

        if ((str[stringLen] = fgetc(fp)) == '\n' || str[stringLen] == EOF)
            break;

        ++stringLen;
    }

    str = (char *)realloc(str, (stringLen + 1) * sizeof(char));

    str[stringLen] = '\0';

    return str;
}

void copyLines(FILE *&from, FILE *&to, const short &numLines) {
    for (short i = 0; i < numLines; ++i) {
        char *line = getLine(from);

        fprintf(to, "%s\n", line);

        delete[] line;
    }
}

void printDeleteBuff(FILE *&file, char *&buff) {
    fprintf(file, "%s\n", buff);
    delete[] buff;
}

void skipLines(FILE *file, const short &lines) {
    for (short i = 0; i < lines; ++i)
        while (fgetc(file) != '\n')
            ;
}

short countNumLines(FILE *&file) {
    short lines = 0;

    for (char i = fgetc(file); i != EOF; i = fgetc(file))
        if (i == '\n')
            ++lines;

    rewind(file);

    return lines;
}

void replaceTmpNotes(const char *const notesPath, const char *const tmpPath) {
    remove(notesPath);
    rename(tmpPath, notesPath);
}

char lower(const char &letter) {
    if ((letter >= 'A') && (letter <= 'Z'))
        return (letter + 32);
    else
        return letter;
}