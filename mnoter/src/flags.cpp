#include "flags.hpp"

void help() {
    std::cout << BOLD GREEN "MNoter " WHITE "is a small program that helps you manage your notes.\n"
              << PURPLE "usage" WHITE ": " CYAN "mnoter <flags> <operation>\n\n"
              << PURPLE "flags" WHITE ": The flags available are " RED "-h" WHITE "/" RED
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

void version() { std::cout << BOLD GREEN "MNoter " WHITE "version: " RED << VERSION << RESET; }