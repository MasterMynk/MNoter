# MNoter
A small program that helps you manage your notes.
## usage:
*mnoter \<flags> \<operation>*
### flags:
* **-h**/**--help**: Prints out this message.

* **-s**/**--silent**: By default MNoter prints out your notes after operations such as `add`, `remove`, `swap` and `move`. This flag turns that feature off.

* **-e**/**--editor**: This flag only has an effect when using the edit operation. It lets you provide the editor you would like to use with having being asked about it.
    * **usage** *-e/--editor="\<editor of choice>"*

* **-v**/**--version**: Prints out the version and exits.

* **-n**/**--no-ask**: This flag only has an effect when used with the `clear` operation. It only disables the confirmation prompt.

### operations:
* **add**: Creates a new note with all the parameters given afterwards.

* **show**: Prints out all the notes.

* **remove**: Removes a notes at the numbers given afterwards.

* **swap**: Swaps a note with another based on the numbers given.

* **move**: Moves the note at the number given first to another number given afterwards.

* **edit**: This opens the file that stores the notes in a text editor so that you can edit it. Every line is a new note. If no editor is supplied with the `-e`/`--editor` flags, MNoter will first check the EDITOR variable and if an editor is still not found then it will ask for the editor.

* **change**: Accepts a number which will be the note to change and replaces it will all the arguments specified later.

* **clear**: Deletes all your notes. It always asks before deleting your notes but this can be disabled using the `-n`/`--no-ask` flag.

* **backup**: Copies the notes file to a another location that is supposed to be given afterwards.