#!/usr/bin/python

#Michael Patson
#This is the client to go with ftserver.c

#https://realpython.com/python-sockets/#tcp-sockets
#https://docs.python.org/2/library/socket.html
#both sources really guided

import sys
import socket
import os
import time
#os is used to check file

#this is similar to project one, except we are now connecting here instead of binding
def setupConnection(hostname, hostport):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        sock.connect((hostname, hostport))
    except:
        error("Error connecting to port %d" % (hostport), 1)
    print "Connected to port %d" % (hostport)
    return sock
        
def sendMessage(socket, string):
    try:
        socket.send(string)
    except:
         error("Error sending to server", 1)

#receive a  message of 10000 char(max size of buffer)            
def receiveMessage(receiveSocket):
    receivedMess =receiveSocket.recv(10000)
    return receivedMess


#the following function is modifie from this link
#https://stackabuse.com/python-check-if-a-file-or-directory-exists/
def getFile(fileMessage, filename):
    if (fileMessage=="Requested File Does Not exist"):
        print "Filename %s does not exist" %filename
        return
    
    
    if os.path.isfile(filename):
        print "Filename %s already exist, will make copy" %filename
        
        filename_parts = filename.split('.', 1)
        newFileName= filename_parts[0]+ '_1.'+ filename_parts[1]
        
        f = open(newFileName, "w+")
        f.write(fileMessage)
        f.close()
        print "A copy named  %s was made" %newFileName
        
        return
    
    f = open(filename, "w+")
    f.write(fileMessage)
    f.close()
    
    print "A copy named  %s was made" %filename
 
    

def error(string, exitVal):
    print"%s" %(string)
    sys.exit(exitVal)

#python ftclient.py 0 <SERVER_HOST>1, <SERVER_PORT>2, <COMMAND>3 <FILENAME>4, <DATA_PORT5>

#program runs here
def main():
# Check input of arugements to ensure command
#exit with error if no
    if ((sys.argv[3] != '-l' and sys.argv[3] != '-g')):

        error("Format: python ftclient.py <SERVER_HOST>, <SERVER_PORT>, <COMMAND>,<FILENAME>, <DATA_PORT> ", 4)

    #now based on command check to make sure file name is present
    if ((sys.argv[3]=='-l') and len(sys.argv) != 5):
        error("When using '-l' make sure to use python ftclient.py <SERVER_HOST>, <SERVER_PORT>, <COMMAND>,<DATA_PORT>",4)
        
        
   

    #if -l command    
    if ((sys.argv[3]=='-l') and len(sys.argv) == 5):
        #Now set variables
        hostname=(sys.argv[1])
        hostport=int(sys.argv[2])
        userCommand=(sys.argv[3])    
        dataport=int(sys.argv[4])
        #these are for trouble shooting andshould be commented out
        #print hostname
        #print hostport
        #print userCommand
        #print dataport

    #if -g command
    if ((sys.argv[3]=='-g') and len(sys.argv) != 6):
        error("When using '-g' make sure to use python ftclient.py <SERVER_HOST>, <SERVER_PORT>, <COMMAND>,<DATA_PORT>, filenam ",4)

    if ((sys.argv[3]=='-g') and len(sys.argv) == 6):
        #Now set variables
        hostname=(sys.argv[1])
        hostport=int(sys.argv[2])
        userCommand=(sys.argv[3]) 
        filename = (sys.argv[4])
        dataport=int(sys.argv[5])
        #these are for trouble shooting andshould be commented out
        #print hostname
        #print hostport
        #print userCommand
        #print filename
        #print dataport

    #now variables should be set based on user input
    
    
    
    #check the port numbers to make sure withing range
    if(hostport<1024) or (hostport>65535):
        error("Check the server port number",1)
    
    if(dataport<1024) or (dataport>65535):
            error("Check the dataport port number",1)

    #create connection, just like in previous project1
    host_socket=setupConnection(hostname, hostport)
    clientName = socket.gethostname()
    #sock is open
    #print clientName
    #the order i decode it is command, client hostname, client port, filename
    #send this so the server can process request
    
    if userCommand == "-l":
        sendCommand= userCommand + ";"+ clientName + ";"+str(dataport)
        sendMessage(host_socket,sendCommand)
        
        #print sendCommand
        
    if userCommand == "-g":
        sendCommand= userCommand + ";"+ clientName + ";"+str(dataport)+";"+filename
        sendMessage(host_socket,sendCommand)
        #print sendCommand
        
    time.sleep(2)    
    
    receiveSocket=setupConnection(hostname, dataport)
    
    #receiving socket is open
    
    if userCommand == "-l":
        serverResponse = receiveMessage(receiveSocket)
        print "Receiving Directory: "
        print serverResponse
        
        
    if userCommand == "-g":
        serverResponse = receiveMessage(receiveSocket)
        getFile(serverResponse, filename)
        print "File Transfer Complete"
        
        
        
        
    #close sockets
    host_socket.close()
    #print "close"
    receiveSocket.close()
    #print "close"
    sys.exit(0)

if __name__ == "__main__":
    main()