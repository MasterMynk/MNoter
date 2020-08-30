/*
 * Started writing on 20-8-2020
 * Written by: Mayank Shigaonker
 * Email: shigoankerMayank@gmail.com
 */

#include "flags.hpp"
#include "functions.hpp"
#include "operations.hpp"

int main(int argc, char *argv[]) {
    std::string homeDir = getenv("HOME");
    uint8_t flags = false;
    char *editorStr = nullptr;

    std::filesystem::create_directory(homeDir += "/.config/MNoter");

    std::string notesPath = homeDir + "/notes.txt";

    for (short i = 1; i < argc; ++i)
        if (argv[i][0] == '-')
            if (argv[i][1] == 'h' || argv[i][2] == 'h') {   // Help flag
                help();
                flags |= SILENT_BIT;
                break;
            } else if (argv[i][1] == 'v' || argv[i][2] == 'v') {
                version();
                flags |= SILENT_BIT;
                break;
            } else if (argv[i][1] == 's' || argv[i][2] == 's')   // Silent flag
                flags |= SILENT_BIT;
            else if (argv[i][1] == 'e' || argv[i][2] == 'e')   // Editor flag
                editor(argv[i], editorStr);
            else if (argv[i][1] == 'n' || argv[i][2] == 'n')   // No ask flag
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
                swap(&argv[i + 1], argc - (i + 1), homeDir, notesPath.c_str(), flags);
                break;
            }
        } else if (lower(argv[i][0]) == 'r') {   // Remove
            remove(&argv[i + 1], argc - (i + 1), homeDir, notesPath.c_str(), flags);
            break;
        } else if (argv[i][0] == 'm') {   // Move
            move(&argv[i + 1], argc - (i + 1), homeDir, notesPath.c_str(), flags);
            break;
        } else if (argv[i][0] == 'e') {   // Edit operation
            edit(&argv[i + 1], argc - (i + 1), notesPath.c_str(), editorStr, flags);
            break;
        } else if (argv[i][0] == 'c') {
            if (argv[i][1] == 'h') {   // Change operation
                change(&argv[i + 1], argc - (i + 1), homeDir, notesPath.c_str());
                break;
            } else {   // Clear operation
                clear(&argv[i + 1], argc - (i + 1), notesPath.c_str(), flags);
                break;
            }
        } else if (argv[i][0] == 'b') {
            backup(&argv[i + 1], argc - (i + 1), notesPath.c_str(), flags);
            break;
        }

    if (!(flags & SILENT_BIT))     // If the silent bit is not set
        show(notesPath.c_str());   // Show the notes
}