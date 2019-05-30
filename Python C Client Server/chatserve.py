####Name:Michael Patson
####Date Feb 5, 2019
####Oregon State Assignment 1
#### chatserve.py
#### This code opens a "server" to receive messages
#### CS 372 - Introduction to Networks 


#######works cited
#https://realpython.com/python-sockets/#tcp-sockets
#https://docs.python.org/2/library/socket.html
#both sources really guided

import sys
import socket

#create a socket
#report socket is open
#print name of host and port
def setupConnection(port):

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    print('Socket created')
    print('Waiting for connection')
    # Bind the socket to port
    
    HOST = socket.gethostname()
    PORT = int(port)
    print("Host:")
    print(HOST)
    print("Port Number:")
    print(PORT)
    
    #will try to bind host
    #bind opens up to connections, learned about it here
    #https://stackoverflow.com/questions/27014955/socket-connect-vs-bind
    try:
        sock.bind((HOST, PORT))
    except socket.error as msg:
        print('Cannot Open Port : ' + str(msg[0]) + msg[1])
        sys.exit()
    print('Socket Open')

    #can try to accept 10 more connections
    sock.listen(0)
    return sock

#This takes the client message and looks for quit
def quitClient(clientMessage):
    quitMessage = "\quit"
    if clientMessage.find(quitMessage) != -1:
        print('Client has sent "quit" command')
        return -1
    
#chekcs the server to see if sent quit
def quitServer(serverMessage):
    quitMessage = "\quit"
    if serverMessage.find(quitMessage) != -1:
        print('Sever has sent "quit" command')
        return -1
    
#accepts the conenction
#displays the message
#checks for quit, if yes return -1
#otherwise, return 0
def receiveMessage(connection):
    clientMessage = connection.recv(512)
    print(clientMessage)
    if quitClient(clientMessage)== -1:
        connection.close()
        return -1
    return 0


#sets terminal screen to say server>
#combines into messages
#checks for quit, if yes return -1
#otherwise, return 0
def sendMessage(connection):
    # server handle is 8 chars long
    #compose message here
    handle = "SERVER> "
    inputText = raw_input(handle)
    message = handle + inputText

    # only get the first 508 chars
    #and sends message
    serverMessage = message[:508]
    connection.send(serverMessage)
    #looks for quit
    if quitServer(serverMessage)== -1:
        connection.close()
        return -1
    return 0


#if message cannot send or receive or detects quit
def sendreceive(connection):
    
    while True:
        if receiveMessage(connection)== -1:
            break
        if sendMessage(connection)==-1:
            break

def main(argv):
    sock = setupConnection(argv[1])
    while True:
        #Socket is open, and connected, client goes first(sends auto message of port)
        print('Waiting for Client')
        connection, client_address = sock.accept()

        try:
            print("Accepted Connection")
            print("IP, Port")
            print(client_address)
            #pass in connection info
            sendreceive(connection)
        finally:
            #close
            connection.close()

main(sys.argv)