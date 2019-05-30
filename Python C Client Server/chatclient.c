/********************************************************
Name: Michael Patosn
CS 372 Oregon State
Date Feb 1st 2019
LastModified Feb 3, 2019
CS 372 - Introduction to Networks 
References Cited: https://beej.us/guide/bgnet/html/multi/clientserver.html#simpleclient
BEEJ suggests breaking out into functions, so I willdo that

Desc: This is the client for a chat service
********************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>


#define MAXDATASIZE 501 //from assignment requrements
//#define MAXHANDLESIZE 10 //from assignemnt requirements(one word up to 10 characters)

//copied from OS assignment
void error(const char *msg) { perror(msg); exit(1); } // Error function used for reporting issues


// get sockaddr, IPv4 or IPv6:
//gets socket address and identified if IPv4 or IPv6
//copied directly from guide
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

/***********
connects this client to the serve
create a new socket
Reports error if socket cannot be created

Gets the socket address(calls function)
Contacsts servers

//this is very similar to the encoding project from last semester
Creates a socket
***********/



/**************
create handle
make size for a name is 10 characters... plus a endline \0
make the length 12 bc > ; is added to each string
**********/

void createHandle (char handle[12])
{
    
    
    //clear memory
    memset(handle,'\0', 12);
    //memset((char *)&handle, '\0', sizeof(handle));
    //int i;
    //ask for handle alias
    char inputbuffer[50];
    memset(inputbuffer,'\0', 50);
    
    printf("What is your handle? :::Max 10 characters::: \n");
    
    fgets(inputbuffer, 50, stdin);
    
   //i coped this froms stack along time ago
   //https://stackoverflow.com/questions/7898215/how-to-clear-input-buffer-in-c
    //modified for my own use, use the concept outlined
      
    //this jsut ensures if the message is less than 10 character no need to hit enter twice
   if(strlen(inputbuffer)>11)
    {
    int c;  
    while ((c = getchar()) != '\n' && c != EOF);
    }
    

    //following adds > to string for any size less than 10
    int z;
    z=0;
    z=strcspn(inputbuffer, "\n");
    int y;
    for (y=0; y<=10; y++)
    {
        handle[y]=inputbuffer[y];
        if (z==y)
        {
        handle[(y)] = '>';
        handle[(y+1)] = ' ';
            break;
        }
        else 
        {
        if (y==10)
        {
        handle[(y)] = '>';
        handle[(y+1)] = ' ';
        }
        }
        
    }
    
    
    
    printf("Handle Accepted\n");

 
    if (handle[0]=='\n' && handle[1] == '\0')
        {
           error("Did not enter handle??"); 
        }
    //else
    printf("This is your handle: %s\n", handle);
           
}

/************************
Sends the first message, includes port adn greeting
no modifications
reports back error if message was not sent
*********/

void sendFirstMessage(int socketFD, char handle[12], char* port) 
{
    int n;
    char message[50];
    char greeting[23];
    memset(greeting,'\0', sizeof(greeting));
    strcpy(greeting, "(Handle :: Access Port)");
    memset(message,'\0', sizeof(message));
    strcpy(message, handle);
    strcat(message, port);
    strcat(message, greeting);
    n = write(socketFD, &message, strlen(message));
    
    //no data was sent
    if (n < 0) {
        error("ERROR sending message\n");
    }

}


/*this is the main send message function
it sends a messge of 512 chracters to the server
512 because the max length of the handle name is 10 plus two characters max length of message 500
also checks message for quit 
each of the functions is commented on below
*/

int sendMessage(int socketFD, char handle[12])
{
    int n;
    char message[512];
    memset(message,'\0', sizeof(message));
    strcpy(message, handle);
    
    char messageBuffer[1000];
    memset(messageBuffer,'\0', 1000);


    // prompt for message
    printf(handle);
    
    int x;

   
    fgets(messageBuffer, 999, stdin);
    //this jsut ensures if the message is longer than 500 character need to hit enter twice
    if(strlen(messageBuffer)>501)
    {
    int c;  
    while ((c = getchar()) != '\n' && c != EOF);
    }
    
    int i; 
    //trims handle
    i=strcspn(messageBuffer, "\n");
    
    
    char shortMessage[500];
    
    int a;
    
    for (a=0;a<500;a++)
    {
        shortMessage[a]=messageBuffer[a];
        if (a>=i)
        {
           shortMessage[a] ='\0';
        }
    }
    
    
    
    
    // checks for \quit message
    //if error is quit, then leaves
    if(strncmp(shortMessage, "\\quit", 4) == 0) {
        n = write(socketFD, &messageBuffer, strlen(messageBuffer));
        
        if (n < 0) 
        {
            error("Did not send message.\n");
        }
        printf("Chat session concluded\n");
        close(socketFD);
        return 1;

    }
        //if the message is not quiet
    //then send the message
    else {
        strcat(message, shortMessage);
        n = write(socketFD, &message, strlen(message));
        if (n < 0) {
            error("ERROR sending message\n");
            return -1;
        }
    }
    return 0;
}


//this needs to be an int so we can send a signal to close
//same error checking copied from OS
//servername is always the same
//508 because server size (name and file are same)
int decodeMessage(int socketFD)
{
    int n;
    char message[508];
    memset(message,'\0', sizeof(message));
        
    n = read(socketFD, message, 508);
        if (n < 0) 
            {
            error("Cannot read from socket \n");
            }
    
    
    
    printf("%s \n", message);

    if(strstr(message, "\\quit") != NULL) {
        printf("Server has quit \n");
        return -1;
    }
    return 0;
}
    
        
//takes the host name and the port number as arguments
int main(int argc, char *argv[])
{
    int port;
    struct hostent *server;

    // check that we have the right amount of arguments
    //copied directly from previous assignment in OPERATING SYSTERS
    if (argc < 3) {
        fprintf(stderr,"Use %s hostname port\n", argv[0]);
        exit(0);
    }

    // get server name
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "Error, cannot find host\n");
        exit(0);
    }

    // get port number
    port = atoi(argv[2]);

    
    int socketFD = connectServer(server, port);

    char handle[12];
    createHandle(handle);

    // send the initial communication message with handle and port
    sendFirstMessage(socketFD, handle, argv[2]);

    int boolAccept;
    boolAccept = 0;

    // send and receive messages until 'quit' is entered
    //the two functions are set up, so that they return 0 when messages are sent
    //the functions return 1 or -1 if error with send or the quit message is received
    //same thing used in python server
    //break statements allow for exit after quit
    while(boolAccept == 0)
    {
        boolAccept = decodeMessage(socketFD);
        if (boolAccept !=0)
            break;
        boolAccept = sendMessage(socketFD, handle);
        if (boolAccept !=0)
            break;
    }

    close(socketFD);
    return 0;
}

