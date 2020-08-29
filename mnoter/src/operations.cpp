#include "operations.hpp"

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

void remove(char **const &notes, short len, const std::string &homeDir,
            const char *const &notesPath) {
    std::string tmpPath = homeDir + "/tmp.txt";

    FILE *notes_f = fopen(notesPath, "r"), *tmp_f = fopen(tmpPath.c_str(), "w");
    short notesToRem[!len ? 1 : len];

    check<bool>(!notes_f, "Couldn't open notes file!!");
    check<bool>(!tmp_f, "Couldn't open a temporary file!!");

    if (!len) {
        printf("Please enter a note that I should remove: ");
        scanf("%hu", &notesToRem[0]);
        len = 1;
    } else
        for (short j = 0; j < len; ++j) {
            check<bool>(!isNum(notes[j]), (std::string(notes[j]) + " is now a number!!").c_str());
            notesToRem[j] = toInt(notes[j]);
        }

    const short lastN = countNumLines(notes_f);

    for (short i = 0; i < len; ++i)
        check<bool>(notesToRem[i] > lastN || notesToRem[i] <= 0,
                    "The note you wish to remove doesn't exist!!\n");

    /********************** Copy all of the notes except the ones to remove ***********************/
    for (short i = 1; i <= lastN; ++i) {
        bool flag = false;

        for (short j = 0; j < len; ++j)
            if (notesToRem[j] == i) {
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
        if (from == i) {   // We've reached the line to move
            skipLines(notes_f, 1);
            continue;
        } else if (to == i) {   // We've reached where the line should be moved
            if (to > from) {
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

void clear(const char *const &notesPath) {
    FILE *notes_f = fopen(notesPath, "w");
    fclose(notes_f);
}

void backup(const char *const path, const char *const notesPath) {
    using namespace std::filesystem;

    try {
        copy_file(notesPath, path, copy_options::overwrite_existing);
    } catch (__cxx11::filesystem_error &) {
        copy_file(notesPath,
                  path + std::string(path[strLen(path) - 1] == '/' ? "notes.txt" : "/notes.txt"),
                  copy_options::overwrite_existing);
    }
}