/*
Name: Michael Patson
Class CS 372 Oregon State
Desc.  This is the "server" file for program 2
This program builds on previous work from Oregon States CS 344 course.
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <dirent.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <signal.h>

#include <arpa/inet.h>



#define MAX_SIZE 10000



//copied from OS assignment
void error(const char *msg) {perror(msg); exit(1); } // Error function used for reporting issues

//valids inputs
void validateInput(int argc, int i)  
{
    // Check inputs
    if (argc != 2)
    {
        //exit with error if not exactly 2
        printf("Format: ./ftserver.c <SERVER_HOST>\n");
        exit(1);
    }
    else if (i < 1024 || i > 65535)
    {
        //check rang of port input, exit with error
        printf("Select Port in Range: 1024 - 65535\n");
        exit(1);
    }   
}

//this function is basically a copy from project 1, swtiched the bind and connect functions.

int createHost(int port)
{
    int socketFD;
    
    //create socket and clear memory
    struct sockaddr_in serverAddress;
    memset((char *)&serverAddress, '\0', sizeof(serverAddress));
    
    
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    
    
    //char hostname[1024];
    //gethostname(hostname, 1024);
    //printf("Hostname %s.\n", hostname);
    
    
    //create socket and open it
    socketFD = socket(AF_INET, SOCK_STREAM, 0); 
    //error if cannot create
    if (socketFD < 0) error("Cannot create socket\n");
    
      int yes=1; setsockopt(socketFD,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));
    
    
    setsockopt(socketFD, SOL_SOCKET, SO_REUSEPORT, &yes,sizeof(int));
    if (bind(socketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to port
		error("Cannot bind socket\n");
	
    //not sure why i used 5, it is what we used in OS
    
    
    listen(socketFD, 5); // Flip the socket on - it can now receive up to 5 connections
    //when i copied this over, did not add listen (was original used in client)
    //MICHAEL LOOK FOR ERROR HERE
    
    //printf("Server open on %d\n", port);  
    
    return socketFD;
    
}


int createHost2(int port)
{
    int socketFD;
    
    //create socket and clear memory
    struct sockaddr_in serverAddress;
    memset((char *)&serverAddress, '\0', sizeof(serverAddress));
    
    
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    
    
    //char hostname[1024];
    //gethostname(hostname, 1024);
    //printf("Hostname %s.\n", hostname);
    
   
    //create socket and open it
    socketFD = socket(AF_INET, SOCK_STREAM, 0); 
    //error if cannot create
    if (socketFD < 0) error("Cannot create socket\n");
    
    //i was really stucked here
    //https://stackoverflow.com/questions/10619952/how-to-completely-destroy-a-socket-connection-in-c
    //The first comment sovled it for me, and provided me somethng to look up and understand
    
    
   int yes=1; setsockopt(socketFD,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));
    
    
    setsockopt(socketFD, SOL_SOCKET, SO_REUSEPORT, &yes,sizeof(int));
    
    if (bind(socketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to port
		error("Cannot bind 2socket\n");
	
    //not sure why i used 5, it is what we used in OS
    
  
    
    return socketFD;
    
}



//this function takes the socket id and a pointer to buffer
//reads into buffer
void readINTObuffer(int socketFD, char*buffer)
{
    //clear buffer
    memset(buffer, '\0', sizeof(buffer));
    int charR;
    //leave trailing \0
    charR= recv(socketFD, buffer, MAX_SIZE-1, 0);
    
    if (charR < 0)
        error("Could not read from socket\n");
    
    
    //this is for testing remove MICHAEL
    //printf("This is the buffer:\n %s", buffer);
      
}

//this function breaks apart the message buffer
//which is seperated with semicolons
//takes in pointer for variables and assignemtsn them
//the order i decode it is command, client hostname, client port, filename

void decode(int * clientPort, char *inputCommand, char* buffer, char * fileNameString, char *clientNameString )
{
    
    char *readCommand;
    //get command
    readCommand= strtok(buffer, ";");
    strcpy(inputCommand, readCommand );
    //printf("This is the input command %s\n", inputCommand);
    
    char *readName;
    readName=strtok(NULL, ";");
    strcpy(clientNameString, readName );
    
    //get port
    *clientPort=atoi(strtok(NULL, ";"));
    //printf("This is the port %d\n", clientPort);
    
    if (!strcmp(readCommand, "-g"))
    {
    char* tempFileName;
    tempFileName= (strtok(NULL, ";"));
    strcpy(fileNameString, tempFileName );  
    }
    
    
}

void printOUT(int clientPort, char *inputCommand, char * fileNameString, char *clientNameString)
{
    printf("Connection from : %s\n", clientNameString);
    
    if (!strcmp(inputCommand, "-l"))
    {
        printf("List directory requested on %d\n", clientPort);
        printf("Sending directory contents to %s : %d\n",clientNameString,clientPort );
    }
    
    if (!strcmp(inputCommand, "-g"))
    {
        printf("File :%s requested on port: %d\n",fileNameString,clientPort );
        printf("Sending :%s to %s port: %d\n",fileNameString, clientNameString, clientPort );
    }
}

//I COPIED this function from here
//https://stackoverflow.com/questions/3554120/open-directory-using-c
//Takes the buffer and rewrites it to include the files listin
//
void directory(char* buffer)
{
    memset(buffer, '\0', sizeof(buffer));

	DIR* p;
	struct dirent *d;
	p = opendir(".");
    
    if (d!=NULL){
	while (d = readdir(p)) {
		//this is aimed at hidding hidden files an is takig fro CS3444
		if (d->d_name[0] != '.') {
			strcat(buffer, d->d_name);
            strcat(buffer, "\n");
		}
	}
    }
    d=NULL;
}


//this jsut takes a file name and reasds it to buffer
void fileRead(char * buffer, char *fileNameString )
{
    
    
    memset(buffer, '\0', sizeof(buffer));
    
    FILE * fileSEND = fopen(fileNameString, "r");
    
    
    char tempbuffer[1000];
    memset(tempbuffer, '\0', sizeof(tempbuffer));
    while(1){
        if (fgets(tempbuffer, 1000, fileSEND )== NULL)
        {
            strcat(buffer,  "\n");
            break;
        }
        fgets(tempbuffer, 1000, fileSEND );
        strcat(buffer, tempbuffer);
    }
}


//this is the function where we send what is in the buffer
// it takes the buffer and it takes the 
//lifted this from OS class
void replyToClient(char * buffer, int socketClient_FD)
{
    int charR;
    charR= send(socketClient_FD, buffer, strlen(buffer), 0);
    
    if (charR<0){error("Could not send reply to client");}
    
    
}

//execute command decides is it a -l or -g and selections the appropriate function
void executeCommand(char *inputCommand, char * buffer, char * fileNameString, int socketClient_FD)
{
    if (!strcmp(inputCommand, "-l"))
        {
            directory(buffer);
            replyToClient(buffer,socketClient_FD);
            
        }
        
    //checks if file exists and if it does sends it
    //if nto sends a message, file does not exits
    else if ((!strcmp(inputCommand, "-g"))&& ( access( fileNameString, F_OK ) != -1 ) ) 
        {
        
            fileRead(buffer, fileNameString);
            replyToClient(buffer,socketClient_FD);
        }
    else if ((!strcmp(inputCommand, "-g"))&& ( access( fileNameString, F_OK ) == -1 ) ) 
    {
        //file doesnt exits
            memset(buffer, '\0', sizeof(buffer));
            printf("Requested File Does Not exist\n");
            strcat(buffer, "Requested File Does Not exist");
        replyToClient(buffer,socketClient_FD);
    }
     
    
}


//from beesges
void sigint_handler(int sig)
{
    write(0, NULL, 14);
}

    

int main(int argc, char *argv[])
{
    
//validates input
validateInput(argc, atoi(argv[1]));

    
//set variables
    
int hostPort, socketFD, socketFD_Q, socketClient_FD, pid, clientPort;
char buffer[MAX_SIZE];
char inputCommand[5];
char fileNameString[100];
char hostNameString[200];
char clientNameString[200];
hostPort=atoi(argv[1]);
   
    
    //copied from BEEJS guide
    //https://beej.us/guide/bgipc/html/multi/signals.html
    struct sigaction sa;
    sa.sa_handler = sigint_handler;
    sa.sa_flags = 0; // or SA_RESTART
    sigemptyset(&sa.sa_mask)   ; 
    
    socketFD=createHost(hostPort);   
    printf("Server open on %d\n", hostPort);
    
    
    while(1){
        
        
        //struct sockaddr_storage their_addr;
        //socklen_t addr_size;
        //addr_size = sizeof their_addr;

        
        memset(buffer, '\0', sizeof(buffer));
        
        
       if (sigaction(SIGINT, &sa, NULL) == -1) 
       {
        break;   
        }
        
        
        socketFD_Q= accept(socketFD, NULL, NULL);
        
        
        if (socketFD_Q < 0) error("Cannot accept\n");
        
        
        //followed CS344 style
        pid =fork();
        //printf("PID is , %d\n", pid);
        switch (pid){
                //cannot fork
            case -1:
                error("cannot fork, case -1 exit\n");
                
            case 0:
                
                //read into buffer readINTObuffer(int socketFD, char*buffer)
                memset(buffer, '\0', sizeof(buffer));
                readINTObuffer(socketFD_Q, buffer);
                //printf("This is the buffer: %s\n", buffer);
                
                
                //MICHAEL ADD THE OTHER HOST NAME HERE
               decode(&clientPort,inputCommand, buffer, fileNameString, clientNameString);
                 
                
                //below are used for testing
                //printf("Connection from %s.\n", clientNameString);
                //printf("This is the port %d\n", clientPort);
                //printf("This is the filename if here  %s\n", fileNameString);
                //message is decoded, nowh handle response
                memset(buffer, '\0', sizeof(buffer));
                
                
                printOUT(clientPort,inputCommand, fileNameString, clientNameString);
                
                
                //open a new connection
               
               
                int socketFD_Q = createHost(clientPort);
				socketClient_FD = accept(socketFD_Q, NULL, NULL);
                if (socketClient_FD < 0) error("Error on second accept\n"); 

                
                executeCommand(inputCommand, buffer, fileNameString, socketClient_FD);
                
                //used to double check sendingmessage
                //printf("%s", buffer);
                
                //clear for good practive
                
                memset(buffer, '\0', sizeof(buffer));
                
                memset(inputCommand, '\0', sizeof(inputCommand));
                
                
                memset(fileNameString, '\0', sizeof(fileNameString));
                
                close(socketClient_FD);
                 
                close(socketFD_Q);
    
                
        }
            
    
    }
    
    //close client socket
    
    close(socketFD);
    
    printf("Closing server");
    return 0;
    
}
