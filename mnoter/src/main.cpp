/*
 * Started writing on 20-8-2020
 * Written by: Mayank Shigaonker
 * Email: shigoankerMayank@gmail.com
 */

#include "flags.hpp"
#include "functions.hpp"
#include "operations.hpp"

#define SILENT_BIT   0b00000001
#define NO_ASK_BIT   0b00000010
#define FOUND_OP_BIT 0b00000100

int main(int argc, char *argv[]) {
    std::string homeDir = getenv("HOME");
    uint8_t flags = false;
    char *editor = nullptr;

    std::filesystem::create_directory(homeDir += "/.config/MNoter");

    std::string notesPath = homeDir + "/notes.txt";

    for (short i = 1; i < argc; ++i)
        if (argv[i][0] == '-')
            if (argv[i][1] == 'h' || argv[i][2] == 'h') {   // Help flag
                help();
                flags |= SILENT_BIT;
                break;
            } else if (argv[i][1] == 's' || argv[i][2] == 's')   // Silent flag
                flags |= SILENT_BIT;
            else if (argv[i][1] == 'e' || argv[i][2] == 'e') {   // Editor flag
                short j = 0;

                for (; argv[i][j] != '='; ++j)
                    ;
                ++j;
                editor = &argv[i][j];

            } else if (argv[i][1] == 'v' || argv[i][2] == 'v') {
                version();
                flags |= SILENT_BIT;
                break;
            } else if (argv[i][1] == 'n' || argv[i][2] == 'n')   // No ask flag
                flags |= NO_ASK_BIT;
            else
                error((std::string("Unrecognized flag ") + argv[i]).c_str());
        else if (argv[i][0] == 'a') {   // Add operation
            add(&argv[i + 1], argc - (i + 1), notesPath.c_str());
            break;
        } else if (argv[i][0] == 's') {
            if (argv[i][1] == 'h') {   // Show
                show(notesPath.c_str());
                flags |= SILENT_BIT;
                break;
            } else {   // Swap
                swap(&argv[i + 1], argc - (i + 1), homeDir, notesPath.c_str());
                break;
            }
        } else if (lower(argv[i][0]) == 'r') {   // Remove
            remove(&argv[i + 1], argc - (i + 1), homeDir, notesPath.c_str());
            break;
        } else if (argv[i][0] == 'm') {   // Move
            move(&argv[i + 1], argc - (i + 1), homeDir, notesPath.c_str());
            break;
        } else if (argv[i][0] == 'e') {   // Edit operation
            bool hasAllocated = false;

            if (!editor)
                editor = getenv("EDITOR");

            if (!editor) {
                printf("Please enter you editor of choice: ");
                editor = getLine();
                hasAllocated = true;
            }

            edit(notesPath.c_str(), editor);

            if (hasAllocated)
                delete[] editor;
        } else if (argv[i][0] == 'c') {
            if (argv[i][1] == 'h') {   // Change operation
                short noteToChange, len;
                char *text;

                if (i == (argc - 1)) {   // No other arguments were supplied
                    printf("Please enter the number of the note to change: ");
                    scanf("%hu", &noteToChange);

                    printf("Now please enter the text I should put in place of note %d: ",
                           noteToChange);

                    getchar();   // Skip the newline character which remains after the user
                                 // enters the number of the note to change

                    text = getLine();
                    len = 1;
                } else if (i == (argc - 2)) {   // Only one arg was supplied
                    check<bool>(!isNum(argv[i + 1]), "Please enter a valid number!!");
                    noteToChange = toInt(argv[i + 1]);

                    printf("Please enter the text I should put in place of note %d: ",
                           noteToChange);
                    text = getLine();
                    len = 1;
                } else {   // Everything was given
                    check<bool>(!isNum(argv[i + 1]), "Please enter a valid number!!");
                    noteToChange = toInt(argv[i + 1]);
                    len = argc - (i + 2);
                }

                change(noteToChange, &((i == (argc - 1) || i == (argc - 2)) ? text : argv[i + 2]),
                       len, homeDir, notesPath.c_str());
                break;
            } else {   // Clear operation
                char inp;

                if (!(flags & NO_ASK_BIT)) {
                    printf("Are you sure you want to delete all your notes [y/n]: ");
                    scanf("%c", &inp);

                    if (inp == 'y')
                        clear(notesPath.c_str());
                    else
                        printf("Input was '%c'. Exiting without deleting notes.\n", inp);
                } else
                    clear(notesPath.c_str());

                break;
            }
        } else if (argv[i][0] == 'b') {
            char *path;

            if (i == (argc - 1))   // No other args were supplied
                printf("Please enter the path to save your notes file: ");

            path = (i == (argc - 1)) ? getLine() : argv[i + 1];

            backup(path, notesPath.c_str());
        }

    if (!(flags & SILENT_BIT))     // If the silent bit is not set
        show(notesPath.c_str());   // Show the notes
}