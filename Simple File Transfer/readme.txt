This is the readmefile for CS372 Project 2

To run:

place makefile, ftserver.c and michael.txt in a folder

make sure the makefile is in the same directory as ftserver.c and run Make

run ./ftserver <port>

place ftclient.py in a different directory

#python ftclient.py 0 <SERVER_HOST>1, <SERVER_PORT>2, <COMMAND>3 <FILENAME>4, <DATA_PORT5>

make sure to use flipx.engr.oregonstate.edu
only accepts full name




Bugs:

Files can be copied.  If the file is copied again, it will indicate it is a copy.  If the file is copied a 3rd time, it will overwrite the copy and generate no warning

List- Some bug in ftserver.c  the list works on the 1st, 2nd, and 4th and 8th attempts.  Every time. Unsure why this is.  The buffer is not getting populated and this has nothing to do with the socket.  It is sending a blank 