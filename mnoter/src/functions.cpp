#include "functions.hpp"

#include <iostream>

void help() {
    std::cout << BOLD GREEN "MNoter " WHITE "is a small program that helps you manage your notes.\n"
              << PURPLE "usage" WHITE ": " CYAN "mnoter <options> <operation>\n\n"
              << PURPLE "options" WHITE ": The options available are " RED "-h" WHITE "/" RED
                        "--help" WHITE "," RED " -s" WHITE "/" RED "--silent" WHITE ", " RED
                        "-e" WHITE "/" RED "--editor" WHITE ", " RED "-v" WHITE "/" RED
                        "--version " WHITE "and " RED "-n" WHITE "/" RED "--no-ask" WHITE ".\n"
              << PURPLE "         -h" WHITE "/" PURPLE "--help" WHITE
                        ": Prints out this message.\n\n"
              << PURPLE
        "         -s" WHITE "/" PURPLE "--silent" WHITE ": By default " GREEN "MNoter " WHITE
        "prints out your notes after operations\n"
        "                      such as" RED " add" WHITE "," RED " remove" WHITE "," RED
        " swap" WHITE " and" RED " move" WHITE
        ". This flag turns that\n"
        "                      feature off.\n\n"
              << PURPLE "         -e" WHITE "/" PURPLE "--editor" WHITE
                        ": This flag only has an effect when using the edit operation.\n"
                        "                      It lets you provide the editor you would like to "
                        "use with\n"
                        "                      having being asked about it.\n"
              << PURPLE "                      usage" WHITE ":" CYAN
                        " -e/--editor=\"<editor of choice>\"\n\n"
              << PURPLE "         -v" WHITE "/" PURPLE "--version" WHITE
                        ": Prints out the version of " GREEN "MNoter " WHITE "and exits.\n\n"
              << PURPLE
        "         -n" WHITE "/" PURPLE "--no-ask" WHITE
        ": This flag only has an effect when used with the " RED "clear\n" WHITE
        "                      operation. It only disables the confirmation prompt.\n\n"
              << PURPLE
        "operations" WHITE ": There are three operation this program can perform " RED "add" WHITE
        ", " RED "show" WHITE ", " RED "remove" WHITE ",\n" RED "            swap" WHITE ", " RED
        "move " WHITE ", " RED "edit" WHITE ", " RED "change " WHITE "and " RED "clear" WHITE
        ".\n\n"
              << PURPLE "            add" WHITE
                        ": Creates a new note with all the parameters given afterwards.\n\n"
              << PURPLE "            show" WHITE ": Prints out all the notes.\n\n"
              << PURPLE "            remove" WHITE
                        ": Removes a notes at the numbers given afterwards.\n\n"
              << PURPLE "            swap" WHITE
                        ": Swaps a note with another based on the numbers given.\n\n"
              << PURPLE "            move" WHITE
                        ": Moves the note at the number given first to another number\n"
                        "                  given afterwards.\n\n"
              << PURPLE "            edit" WHITE
                        ": This opens the file that stores the notes in a text editor so\n"
                        "                  that you can edit it. Every line is a new note. If no "
                        "editor\n"
                        "                  is supplied with the " RED "-e" WHITE "/" RED
                        "--editor " WHITE "flags, " GREEN "MNoter " WHITE
                        "will first check\n"
                        "                  the " YELLOW "EDITOR " WHITE
                        "variable and if an editor is still not found then\n"
                        "                  it will ask for the editor.\n\n"
              << PURPLE "            change" WHITE
                        ": Accepts a number which will be the note to change and replaces\n"
                        "                    it will all the arguments specified later.\n\n"
              << PURPLE "            clear" WHITE
                        ": Deletes all your notes. It always asks before deleting your\n"
                        "                   notes but this can be disabled using the " RED
                        "-n" WHITE "/" RED "--no-ask" WHITE " flag."
              << RESET;
}

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

void edit(const char *const &notesPath, const char *const &editor) {
    system((std::string(editor) + ' ' + notesPath).c_str());
}

void change(const short &note, char **const &toChangeWith, const short &len,
            const std::string &homeDir, const char *const &notesPath) {
    std::string tmpPath = homeDir + "/tmp.txt";
    FILE *notes_f = fopen(notesPath, "r"), *tmp_f = fopen(tmpPath.c_str(), "w");

    check<bool>(!notes_f, "Couldn't open notes file. You probably don't have any notes yet.");
    check<bool>(!tmp_f, "Couldn't open a temporary file!!");

    const short lastN = countNumLines(notes_f);
    check<bool>(note > lastN || note <= 0, "The note you want to change doesn't exist!!");

    copyLines(notes_f, tmp_f, note - 1);   // Copy all the notes before the note to change
    skipLines(notes_f, 1);                 // Skip the like to be changed
    for (short i = 0; i < len; ++i)        // Put the new line in its place
        fprintf(tmp_f, "%s ", toChangeWith[i]);
    fputc('\n', tmp_f);
    copyLines(notes_f, tmp_f, lastN - note);

    fclose(notes_f);
    fclose(tmp_f);

    replaceTmpNotes(notesPath, tmpPath.c_str());
}

void printVersion() { std::cout << BOLD GREEN "MNoter " WHITE "version: " RED << VERSION << RESET; }