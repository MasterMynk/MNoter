#include "functions.hpp"

#define RED "\x1b[31m"
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
    printf(
        "%sMNoter is a small program that helps you keep your notes.%s\n"
        "mnoter <options> <operation>%s\n"
        "options:%s The options available are %s-h%s,%s --help%s.%s\n"
        "         -h/--help:%s Prints out this message.%s\n"
        "operation:%s There are three operation this program can perform%s add%s,%s\n\
           show%s,%s remove%s and%s swap%s\n"
        "           add:%s Creates a new note with all the parameters specified\n\
                afterwards.%s\n"
        "           show:%s Prints out all the notes.%s\n"
        "           remove:%s Removes a note at the number specified afterwards.%s\n"
        "           swap:%s Swaps a note with another based on the numbers specified.%s\n",
        BOLD WHITE, CYAN, PURPLE, WHITE, RED, WHITE, RED, WHITE, PURPLE, WHITE, PURPLE, WHITE, RED,
        WHITE, RED, WHITE, RED, WHITE, RED, PURPLE, WHITE, PURPLE, WHITE, PURPLE, WHITE, PURPLE,
        WHITE, RESET);
}

void remove(const short &note, const std::string &homeDir, const char *const &notesPath) {
    std::string tmpPath = homeDir + "/tmp.txt";

    FILE *notes_f = fopen(notesPath, "r"), *tmp_f = fopen(tmpPath.c_str(), "w");

    check<bool>(!notes_f, "Couldn't open notes file!!");
    check<bool>(!tmp_f, "Couldn't open a temporary file");

    const short lastN = countNumLines(notes_f);

    check<bool>(note > lastN || note <= 0, "The note you wish to remove doesn't exist!!\n");

    copyFileLines(notes_f, tmp_f, note - 1);       // Copy notes up to the note to remove
    skipLines(notes_f, 1);                         // Skip the note to be removed
    copyFileLines(notes_f, tmp_f, lastN - note);   // Copy the rest of the notes

    // Closing these early to avoid complecations while deleteting it later
    fclose(tmp_f);
    fclose(notes_f);

    remove(notesPath);
    rename(tmpPath.c_str(), notesPath);
}

void swap(const short &from, const short &to, const char *const &homeDir,
          const char *const &notesPath, const char *const &numPath) {
    const std::string tmpPath = std::string(homeDir) + "/tmp.txt";
    FILE *tmp_f = fopen(tmpPath.c_str(), "w"), *notes_f = fopen(notesPath, "r"),
         *num_f = fopen(numPath, "r");

    short int lastN;
    char *buff;

    check<bool>(!tmp_f, "Couldn't open temporary file!!");
    check<bool>(!notes_f,
                "Couldn't open notes file!!\n"
                "       You probably don't have any notes yet.\n"
                "       Add a note  with the command mnoter add");
    check<bool>(!num_f, "Couldn't open num file!!");

    fscanf(num_f, "%hu", &lastN);
    if (to > lastN)
        error("There is no note with the number ", to);
    if (from > lastN)
        error("There is no note with the number ", from);

    /******** Get the note which has the highest number first ********/
    // This for loop takes me at the start of the line to be copied
    skipLines(notes_f, to > from ? to : from);
    fseek(notes_f, 10, SEEK_CUR);   // This moves to the first number
    for (char i = fgetc(notes_f); i != ' '; i = fgetc(notes_f))
        ;
    buff = getLine(notes_f);

    rewind(notes_f);

    /* Now go to the note which has the lower number */
    /* And while doing so copy stuff around */
    copyFileLines(notes_f, tmp_f, to > from ? from : to);
    copyColor(notes_f, tmp_f);
    printDeleteBuff(tmp_f, buff);
    buff = getLine(notes_f);

    copyFileLines(notes_f, tmp_f, (to > from ? to - from : from - to) - 1);
    copyColor(notes_f, tmp_f);
    printDeleteBuff(tmp_f, buff);
    delete[] getLine(notes_f);

    copyFileLines(notes_f, tmp_f, to > from ? lastN - to : lastN - from);

    fclose(tmp_f);
    fclose(notes_f);

    std::filesystem::remove(notesPath);
    std::filesystem::rename(tmpPath, notesPath);

    fclose(num_f);
}

void error(const char *const str) {
    printf("%s%sERROR: %s%s%s\n%s", RED, BOLD, RESET, RED, str, RESET);
    exit(-1);
}

void error(const char *const str, const short &num) {
    printf("%s%sERROR: %s%s%s%d\n%s", RED, BOLD, RESET, RED, str, num, RESET);
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

        if ((str[stringLen] = fgetc(fp)) == '\n')
            break;

        ++stringLen;
    }

    str = (char *)realloc(str, stringLen * sizeof(char));

    str[stringLen] = '\0';

    return str;
}

void copyFileLines(FILE *&from, FILE *&to, const short &numLines) {
    for (short i = 0; i < numLines; ++i) {
        char *line = getLine(from);

        fprintf(to, "%s\n", line);

        delete[] line;
    }
}

void copyColor(FILE *&from, FILE *&to) {
    for (char i = fgetc(from); i != ' '; i = fgetc(from))
        fputc(i, to);
}

void printDeleteBuff(FILE *&file, char *&buff) {
    fprintf(file, " %s\n", buff);
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