/* This example shows how you can connect to a server, */
/* and send the output of the program through a socket.*/

#include "../include/pasm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "192.168.1.16"
#define SERVER_PORT 1337
#define BUFFER_SIZE 1024

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        fprintf(stderr, "Socket creation failed");
	return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
        fprintf(stderr, "Invalid address/ Address not supported");
	return 1;
    }

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        fprintf(stderr, "Connection failed");
	return 1;
    }

    while (1) {
	// Receive data from the server
	int bytesRead = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
	if (bytesRead == -1) {
	    fprintf(stderr, "Receive failed");
	    close(clientSocket);
	    return 1;
	}

	buffer[bytesRead] = '\0'; // Null-terminate the received data
	if (bytesRead > 0 && buffer[bytesRead - 1] == '\n')
	    buffer[bytesRead - 1] = '\0'; //deletes \n
	if (strcmp(buffer, "end") == 0)
	    break;
	printf("script path: %s\n", buffer);
	pasm_run_script(buffer, 0, 0, clientSocket); //considering buffer is the path of a valid .pasm script
    }

    close(clientSocket);
    return 0;
}
