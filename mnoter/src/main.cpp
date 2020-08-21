/*
 * Started writing on 20-8-2020
 * Written by: Mayank Shigaonker
 * Email: shigoankerMayank@gmail.com
 */

/*
TODO:
Complete remove function
Add a feature that takes tells the user to input something if they haven't
*/
#include "functions.hpp"

int main(int argc, char *argv[]) {
    std::string homeDir = getenv("HOME");

    std::filesystem::create_directory(homeDir += "/.config/MNoter");

    std::string notesPath = homeDir + "/notes.txt", numPath = homeDir + "/num.txt";

    for (short i = 0; i < argc; ++i)
        if (argv[i][0] == '-')
            if (argv[i][1] == 'h' || argv[i][2] == 'h')
                help();
            else
                error(std::string(std::string("Unrecognized flag ") + argv[i]).c_str());
        else if (argv[i][0] == 'a') {
            add(argc - (i + 1), &argv[i + 1], notesPath.c_str(), numPath.c_str());
            break;
        } else if (argv[i][0] == 's') {
            if (argv[i][1] == 'h')
                show(notesPath.c_str());
            else
                swap(argv[i + 1], argv[i + 2], homeDir.c_str(), notesPath.c_str(), numPath.c_str());
            break;
        } else if (argv[i][0] == 'r') {
            remove(argv[i + 1], homeDir, notesPath, numPath.c_str());
            break;
        }
}