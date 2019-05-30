Client Server
Author: Michael Patson
CS 372 - Introduction to Networks 

Prog Description
This program is a client/server pair for a simple chat application

Files:
readme.txt
chatclient.c
makefile
chatserve.py

How to use:
1: Start server by typing: 'python chatserve.py port'. replace port with the port number
2: Copy chatclient.c and makefile into same directory and type 'make' into terminal
3: run: chatclient hostname port.  Hostname and port are provided when chatserve.py is run
4: Initial message is sent from client, server must respond
5: If messages exceed character limit (500) enter must be hit twice, same for client handle (10) characters;
    otherwise enter only needs to be hit onces
6: Type \quit into either chat or client to quit
    \quit into client ends chat but leaves serve running
    \quit into server leaves server running, quits client
7: Exit server with control+c (recommend quiting first)
8: To remove C executable, type 'make clean' in directory of chatclient