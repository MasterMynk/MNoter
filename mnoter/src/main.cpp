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

    std::string notesPath = homeDir + "/notes.txt";

    for (short i = 0; i < argc; ++i)
        if (argv[i][0] == '-')
            if (argv[i][1] == 'h' || argv[i][2] == 'h') {
                help();
                break;
            } else {
                using namespace std;

                error(string(string("Unrecognized flag ") + argv[i]).c_str());
            }
        else if (argv[i][0] == 'a') {
            add(&argv[i + 1], argc - (i + 1), notesPath.c_str());
            break;
        } else if (argv[i][0] == 's') {
            if (argv[i][1] == 'h')
                show(notesPath.c_str());
            else {
                using namespace std;

                short arg1, arg2;
                if (i == (argc - 1)) {   // That means no other arguments were given
                    printf("Please tell me which two notes to swap (seperated by a space): ");
                    scanf("%hu %hu", &arg1, &arg2);
                } else if (i == (argc - 2)) {   // That means only one argument was given
                    check<bool>(
                        !isNum(argv[i + 1]),
                        string(string(argv[i + 1]) + " is not a number you cowhead!!").c_str());

                    arg1 = toInt(argv[i + 1]);
                    printf("Please tell me which note should I swap with %d: ", arg1);
                    scanf("%hu", &arg2);
                } else {
                    check<bool>(
                        !isNum(argv[i + 1]),
                        string(string(argv[i + 1]) + " is not a number you cowhead!!").c_str());
                    check<bool>(
                        !isNum(argv[i + 2]),
                        string(string(argv[i + 2]) + " is not a number you cowhead!!").c_str());

                    arg1 = toInt(argv[i + 1]);
                    arg2 = toInt(argv[i + 2]);
                }
                swap(arg1, arg2, homeDir, notesPath.c_str());
            }
            break;
        } else if (argv[i][0] == 'r') {
            short len = argc - (i + 1), notesToRem[len]{0};

            for (short j = 0; j < len; ++j) {
                check<bool>(!isNum(argv[i + 1 + j]), "Please enter a valid number!!");
                notesToRem[j] = toInt(argv[i + 1 + j]);
            }

            remove(notesToRem, len, homeDir, notesPath.c_str());

            break;
        }
}