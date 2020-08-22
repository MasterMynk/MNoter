#include "functions.hpp"

#define RED "\x1b[31m"
#define CYAN "\x1b[34m"
#define PURPLE "\x1b[35m"
#define WHITE "\x1b[37m"

#define BOLD "\x1b[1m"
#define RESET "\x1b[0m"

void add(const int &argc, char **const &argv, const char *const &notesPath,
         const char *const &numPath) {
    bool todoFlag = true;
    FILE *notes_f = fopen(notesPath, "r"), *num_f = nullptr;

    // Just check if it exists
    if (notes_f) {
        fclose(notes_f);
        todoFlag = false;
    }

    check<bool>(!(notes_f = fopen(notesPath, "a+")), "Couldn't open notes file");

    if (todoFlag) {
        check<bool>(!(num_f = fopen(numPath, "w")), "Couldn't open num file!!");

        fputc('1', num_f);
        fprintf(notes_f, "%s%sTODO:%s\n%s%s1. %s", BOLD, RED, RESET, BOLD, CYAN, WHITE);

        fclose(num_f);
    } else {
        short numNotes = 0;

        check<bool>(!(num_f = fopen(numPath, "r")), "Couldn't open num file for reading!!");

        fscanf(num_f, "%hu", &numNotes);
        fprintf(notes_f, "%s%s%d. %s", BOLD, CYAN, ++numNotes, WHITE);

        check<bool>(!(num_f = freopen(numPath, "w", num_f)), "Couldn't open num file!!");
        fprintf(num_f, "%d", numNotes);

        fclose(num_f);
    }

    for (short i = 0; i < argc; ++i)
        fprintf(notes_f, "%s ", argv[i]);
    fprintf(notes_f, "%s\n", RESET);

    fclose(notes_f);
}

void show(const char *const &notesPath) {
    char toPrint;
    FILE *notes_f = fopen(notesPath, "r");

    check(!notes_f, "Couldn't open notes file!!");

    while ((toPrint = fgetc(notes_f)) != EOF)
        printf("%c", toPrint);
}

void help() {
    printf(
        "MNoter is a small program that can help you keep your notes.%s%s\n"
        "mnoter <options> <operation>%s\n"
        "options:%s The options available are %s-h --help%s Prints out this message.%s\n"
        "operation:%s There are three operation this program can perform%s add%s,%s\n\
           show%s and%s remove%s\n"
        "           add:%s Creates a new note with all the parameters specified\n\
                afterwards.%s\n"
        "           show:%s Prints out all the notes.%s\n"
        "           remove:%s Removes a note at the number specified afterwards.\n",
        BOLD, CYAN, PURPLE, WHITE, RED, WHITE, PURPLE, WHITE, RED, WHITE, RED, WHITE, RED, PURPLE,
        WHITE, PURPLE, WHITE, PURPLE, WHITE);
    exit(0);
}

void remove(const char *const index, const std::string &homeDir, const std::string &notesPath,
            const char *const &numPath) {
    std::string tmpPath = homeDir + "/tmp.txt";

    FILE *notes_f = fopen(notesPath.c_str(), "a+"), *num_f = fopen(numPath, "r"),
         *tmp_f = fopen(tmpPath.c_str(), "w");

    short int lastN, intIndex = toInt(index);

    check<bool>(!notes_f, "Couldn't open notes file!!");
    check<bool>(!num_f, "Couldn't open num file!!");
    check<bool>(!tmp_f, "Couldn't open a temporary file");
    check<bool>(!isNum(index), "Please enter a valid number!!");

    fscanf(num_f, "%hu", &lastN);
    check<bool>(intIndex > lastN, "Please enter a index smaller than the number of notes!!");

    for (short i = 0; i < intIndex; ++i) {
        char *line = getLine(notes_f);
        fprintf(tmp_f, "%s\n", line);

        delete[] line;
    }

    delete[] getLine(notes_f);

    for (short i = intIndex; i < lastN; ++i) {
        char *line = getLine(notes_f);
        short j = 0;

        // Transfer the colors from notes.txt --> tmp.txt
        for (; j < 9; ++j)
            fputc(line[j], tmp_f);

        // Now line[j] is probably at the number
        // So ignore it and put a new number in its place
        for (; line[j] != '.'; ++j)
            ;
        fprintf(tmp_f, "%d", i);

        // Now transfer the entire notes here onwards
        fprintf(tmp_f, "%s\n", &line[j]);

        delete[] line;
    }

    fclose(tmp_f);   // Closing it early to avoid complecations while deleteting it later
    fclose(notes_f);

    std::filesystem::remove(notesPath);
    std::filesystem::rename(tmpPath, notesPath);

    num_f = freopen(numPath, "w", num_f);
    fprintf(num_f, "%d", --lastN);
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
    if (to > lastN) {
        printf("%s%sERROR:%s%s There is no note with the number %d%s", BOLD, RED, RESET, RED, to,
               RESET);
        exit(-1);
    }
    if (from > lastN) {
        printf("%s%sERROR:%s%s There is no note with the number %d%s", BOLD, RED, RESET, RED, from,
               RESET);
        exit(-1);
    }

    /******** Get the note which has the highest number first ********/
    // This for loop takes me at the start of the line to be copied
    for (short i = 0; i < (to > from ? to : from); ++i)
        delete[] getLine(notes_f);
    fseek(notes_f, 10, SEEK_CUR);   // This moves to the first number
    for (char i = fgetc(notes_f); i != ' '; i = fgetc(notes_f))
        ;
    buff = getLine(notes_f);

    rewind(notes_f);

    /* Now go to the note which has the lower number */
    /* And while doing so copy stuff around */
    for (short i = 0; i < (to > from ? from : to); ++i) {
        char *line = getLine(notes_f);

        fprintf(tmp_f, "%s\n", line);

        delete[] line;
    }
    for (char i = fgetc(notes_f); i != ' '; i = fgetc(notes_f))
        fputc(i, tmp_f);
    fprintf(tmp_f, " %s\n", buff);
    delete[] buff;
    buff = getLine(notes_f);

    for (short i = 1; i < (to > from ? to - from : from - to); ++i) {
        char *line = getLine(notes_f);

        fprintf(tmp_f, "%s\n", line);

        delete[] line;
    }
    for (char i = fgetc(notes_f); i != ' '; i = fgetc(notes_f))
        fputc(i, tmp_f);
    fprintf(tmp_f, " %s\n", buff);
    delete[] buff;
    delete[] getLine(notes_f);

    for (short i = 0; i < (to > from ? lastN - to : lastN - from); ++i) {
        char *line = getLine(notes_f);

        fprintf(tmp_f, "%s\n", line);

        delete[] line;
    }

    fclose(tmp_f);
    fclose(notes_f);

    std::filesystem::remove(notesPath);
    std::filesystem::rename(tmpPath, notesPath);

    fclose(num_f);
}

void error(const char *const str, const bool &shouldExit, const short &exitCode) {
    printf("%s%sERROR: %s%s%s\n%s", RED, BOLD, RESET, RED, str, RESET);
    if (shouldExit)
        exit(exitCode);
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
        printf("%s\n", line);

        delete[] line;
    }
}