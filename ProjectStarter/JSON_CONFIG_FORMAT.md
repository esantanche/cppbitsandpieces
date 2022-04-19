# Json configurationm format

The json configuration file has name projectstarterconfiguration.json.

It's an array of projects.

Each project has a name, project_name.

It also has a list of tasks.

Each task has two properties: task_type and task_uri.

Let's see each task type individually.

To know the actual executables the tasks invoke, see method initialize_lookup_table_for_executables.

## BROWSER

This task opens a browser.

The property task_uri contains one or more URIs.

They can be URLs or they may be like file:///home/esantanche/MasterWorkspace/GTD_Reference/C++/C++%20Hub.html#.

They have to be separated by spaces.

If there is one only URI, the command line option --new-window is added so prevent the browser from
opening the URI in an existing browser window.

## VIDEOPLAYER

It opens a video player. The property task_uri is ignored.

## FILEMANAGER

It opens the folder whose path is in task_uri in a file manager.

No more than one folder can be opened.

The URI can have spaces.

## PDFVIEWER

It opens a pdf file. The property task_uri is the path of the file.

There can be one only path and spaces are fine.

## TEXTEDITOR

It opens a text editor. The property task_uri is the path of the file.

There can be one only path and spaces are fine.

## SHELL

It opens a shell and runs the command specified in the property task_uri.

## TERMINAL

It opens a terminal app. The property task_uri is the path the terminal opens in.

The executables gnome-terminal and xfce4-terminal are properly supported because they contain the substring
'terminal'. Other terminal apps may not be correctly treated.

See method needs_to_go_to_the_background.

## DOC

It opens a document in Libreoffice. The property task_uri is the path of the file.

There can be one only path and spaces are fine.


