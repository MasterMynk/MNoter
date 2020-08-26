# MNoter
A small program that helps you manage your notes.
## usage:
*mnoter \<options> \<operation>*
### options:
The options available are `-h/--help`, `-s/--silent`, `-e/--editor`.

* **-h/--help:** Prints out this message.

* **-s/--silent:** By default MNoter prints out your notes after operations such as `add`, `remove`, `swap` and `move`. This flag turns that feature off.

* **-e/--editor:** This flag only has an effect when using the edit operation. It lets you provide the editor you would like to use with having being asked about it.
    * **usage:** *-e/--editor="\<editor of choice>"*

### operations:
There are three operation this program can perform `add`, `show`, `remove`, `swap`, `move` and `edit`.

* **add:** Creates a new note with all the parameters specified afterwards.

* **show:** Prints out all the notes.

* **remove:** Removes a notes at the numbers specified afterwards.

* **swap:** Swaps a note with another based on the numbers specified.

* **move:** Moves the note specified to another number specified.

* **edit:** This opens the file that stores the notes in a text editor so that you can edit it. Every line is a new note. If no editor is supplied with the -e/--editor flags MNoter will first check the EDITOR variable and if an editor is still not found then it will ask for the editor.