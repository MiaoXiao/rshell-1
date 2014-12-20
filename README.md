Program Overview

    Project rshell is intended to be a lightweight shell program with strong parsing.

How to use

    You can use rshell to execute basic system commands. to start it just type ./rshell from its directory, or create a shortcut. To exit, type exit.  

Prerequisites
	All libraries used in this project are part of the typical Linux installation.

Installation guide
	`make 

Bug/Limitations/Issues


	ls still has some undefined behavior on recursive searches. It no longer segfaults, but it's not well defined.
	For multiple commands, delimeters are processed using an established precedence. This means that if you had an adjacent || and &&, || wouldn't see anything to run, so it would assume its job is done. Anything after && would still be executed, because the last condition didn't leave anything unfinished. 
	Whitespace can be either in the form of tabs or spaces. Any non-Ascii text such as if arrow keys are read in or unhandled unicode and non-English characters will be treated as whitespace to avoid undefined handling of these characters. Flags can be used without whitespace, although this is not typical behavior. Rshell just treats a dash '-' as the beginning of a flag to avoid other less well defined behavior. Ls will not print permissions if it cannot parse the file owner. Its width is limited to maintain formatting, so very long names will be truncated.
    
