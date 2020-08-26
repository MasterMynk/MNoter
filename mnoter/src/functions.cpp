#include "functions.hpp"

#include <iostream>

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define CYAN "\x1b[34m"
#define PURPLE "\x1b[35m"
#define WHITE "\x1b[37m"

#define BOLD "\x1b[1m"
#define RESET "\x1b[0m"

void add(char **const &argv, const short &argc, const char *const &notesPath) {
    FILE *notes_f = fopen(notesPath, "a");

    check<bool>(!(notes_f = fopen(notesPath, "a+")), "Couldn't open notes file");

    for (short i = 0; i < argc; ++i)
        fprintf(notes_f, "%s ", argv[i]);
    fputc('\n', notes_f);

    fclose(notes_f);
}

void show(const char *const &notesPath) {
    FILE *notes_f = fopen(notesPath, "r");
    short lines;

    check(!notes_f, "Couldn't open notes file. You probably don't have any notes yet!");
    lines = countNumLines(notes_f);

    for (short i = 1; i <= lines; ++i) {
        char *line = getLine(notes_f);

        printf("%s%d. %s%s\n", BOLD CYAN, i, WHITE, line);
        delete[] line;
    }
}

void help() {
    std::cout << BOLD GREEN "MNoter " WHITE "is a small program that helps you manage your notes.\n"
              << PURPLE "usage: " CYAN "mnoter <options> <operation>\n\n"
              << PURPLE "options: " WHITE "The options available are " RED "-h" WHITE "/" RED
                        "--help" WHITE "," RED " -s" WHITE "/" RED "--silent" WHITE ", " RED
                        "-e" WHITE "/" RED "--editor" WHITE ".\n"
              << PURPLE "         -h" WHITE "/" PURPLE "--help:" WHITE
                        " Prints out this message.\n\n"
              << PURPLE
        "         -s" WHITE "/" PURPLE "--silent:" WHITE " By default " GREEN "MNoter " WHITE
        "prints out your notes after operations\n"
        "                      such as" RED " add" WHITE "," RED " remove" WHITE "," RED
        " swap" WHITE " and" RED " move" WHITE
        ". This flag turns that\n"
        "                      feature off.\n\n"
              << PURPLE "         -e" WHITE "/" PURPLE "--editor: " WHITE
                        "This flag only has an effect when using the edit operation.\n"
                        "                      It lets you provide the editor you would like to "
                        "use with having being asked about it.\n"
              << PURPLE "                      usage:" CYAN
                        " -e/--editor=\"<editor of choice>\"\n\n"
              << PURPLE
        "operations: " WHITE "There are three operation this program can perform " RED "add" WHITE
        ", " RED "show" WHITE ", " RED "remove" WHITE ",\n" RED "            swap" WHITE ", " RED
        "move " WHITE "and " RED "edit" WHITE ".\n"
              << PURPLE "            add:" WHITE
                        " Creates a new note with all the parameters specified afterwards.\n\n"
              << PURPLE "            show:" WHITE " Prints out all the notes.\n\n"
              << PURPLE "            remove:" WHITE
                        " Removes a notes at the numbers specified afterwards.\n\n"
              << PURPLE "            swap:" WHITE
                        " Swaps a note with another based on the numbers specified.\n\n"
              << PURPLE "            move:" WHITE
                        " Moves the note specified to another number specified.\n\n"
              << PURPLE "            edit: " WHITE
                        "This opens the file that stores the notes in a text editor so\n"
                        "                  that you can edit it. Every line is a new note. If no "
                        "editor\n"
                        "                  is supplied with the " RED "-e/--editor " WHITE
                        "flags " GREEN "MNoter " WHITE
                        "will first check\n"
                        "                  the " YELLOW "EDITOR " WHITE
                        "variable and if an editor is still not found then it\n"
                        "                  will ask for the editor.\n"
              << RESET;
}

void remove(const short notes[], const short &len, const std::string &homeDir,
            const char *const &notesPath) {
    std::string tmpPath = homeDir + "/tmp.txt";

    FILE *notes_f = fopen(notesPath, "r"), *tmp_f = fopen(tmpPath.c_str(), "w");

    check<bool>(!notes_f, "Couldn't open notes file!!");
    check<bool>(!tmp_f, "Couldn't open a temporary file!!");

    const short lastN = countNumLines(notes_f);

    for (short i = 0; i < len; ++i)
        check<bool>(notes[i] > lastN || notes[i] <= 0,
                    "The note you wish to remove doesn't exist!!\n");

    /********************** Copy all of the notes except the ones to remove ***********************/
    for (short i = 1; i <= lastN; ++i) {
        bool flag = false;

        for (short j = 0; j < len; ++j)
            if (notes[j] == i) {
                flag = true;
                skipLines(notes_f, 1);

                break;
            }

        if (!flag)
            copyLines(notes_f, tmp_f, 1);
    }

    /*********** Closing these early to avoid complecations while deleteting it later ************/
    fclose(tmp_f);
    fclose(notes_f);

    replaceTmpNotes(notesPath, tmpPath.c_str());
}

void swap(const short &from, const short &to, const std::string &homeDir,
          const char *const &notesPath) {
    const std::string tmpPath = homeDir + "/tmp.txt";
    FILE *tmp_f = fopen(tmpPath.c_str(), "w"), *notes_f = fopen(notesPath, "r");
    char *buff;

    check<bool>(!tmp_f, "Couldn't open temporary file!!");
    check<bool>(!notes_f,
                "Couldn't open notes file!!\n"
                "       You probably don't have any notes yet.\n"
                "       Add a note with the command mnoter add");

    const short lastN = countNumLines(notes_f);
    check<bool>(to > lastN || to <= 0 || from > lastN || from <= 0,
                "The notes you wish to swap don't exist");

    /************************* Copy the note which is further down first *************************/
    skipLines(notes_f, (to > from ? to : from) - 1);
    buff = getLine(notes_f);

    rewind(notes_f);   // Go back to the top

    /*********************** Now go to the note which has the lower number ************************/
    /****************** And while doing so copy stuff around ******************/
    copyLines(notes_f, tmp_f, (to > from ? from : to) - 1);
    printDeleteBuff(tmp_f, buff);   // Put the note copied earlier in its place
    buff = getLine(notes_f);        // And copy it into buffer

    /*************************** Copy the notes in between to and from ****************************/
    copyLines(notes_f, tmp_f, (to > from ? to - from : from - to) - 1);

    /*********** Put the note copied earlier in its place and skip the note afterwards ************/
    printDeleteBuff(tmp_f, buff);
    skipLines(notes_f, 1);

    /******************************** Copy the rest of the notes *********************************/
    copyLines(notes_f, tmp_f, to > from ? lastN - to : lastN - from);

    /**************** Close these because we are going to be modifying them later *****************/
    fclose(tmp_f);
    fclose(notes_f);

    replaceTmpNotes(notesPath, tmpPath.c_str());
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

void move(const short &from, const short &to, const std::string &homeDir,
          const char *const &notesPath) {
    std::string tmpPath = homeDir + "/tmp.txt";
    FILE *notes_f = fopen(notesPath, "r"), *tmp_f = fopen(tmpPath.c_str(), "w");
    char *buff;

    if (from == to)   // Don't waste time because note from is already at note from
        return;

    check<bool>(!notes_f, "Couldn't open notes file!!");
    check<bool>(!tmp_f, "Couldn't open a temporary file!!");

    const short lastN = countNumLines(notes_f);
    check<bool>(from > lastN || from <= 0 || to > lastN || to < 0, "Please enter a valid number!!");

    /************************* First copy the note that has to be moved **************************/
    skipLines(notes_f, from - 1);
    buff = getLine(notes_f);

    rewind(notes_f);

    /******** Now cycle through the notes putting the copied note where is deserves to be *********/
    for (short i = 1; i <= lastN; ++i) {
        if (from == i) {
            skipLines(notes_f, 1);
            continue;
        } else if (to == i) {
            if (to == lastN) {
                copyLines(notes_f, tmp_f, 1);
                fprintf(tmp_f, "%s\n", buff);
                continue;
            } else
                fprintf(tmp_f, "%s\n", buff);
        }

        copyLines(notes_f, tmp_f, 1);
    }

    delete[] buff;

    fclose(notes_f);
    fclose(tmp_f);

    replaceTmpNotes(notesPath, tmpPath.c_str());
}

void replaceTmpNotes(const char *const notesPath, const char *const tmpPath) {
    remove(notesPath);
    rename(tmpPath, notesPath);
}

void edit(const char *const &notesPath, const char *const &editor) {
    system((std::string(editor) + ' ' + notesPath).c_str());
}