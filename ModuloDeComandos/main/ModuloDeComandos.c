/*
 ============================================================================
 Name        : ModuloDeComandos.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================


 */

#define SOCKET_NAME "/tmp/moduloDeComandosServer.sock"
#define BUFFER_SIZE 30

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include "./periphericModulesCall.h"
#include "./fetchCommandAndSocket.h"


int main(int argc, char *argv[])
{
    struct sockaddr_un addr;
    int down_flag = 0;
    int ret;
    int listen_socket;
    int data_socket;
    char result[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];

    /*
     * In case the program exited inadvertently on the last run,
     * remove the socket.
     */

    unlink(SOCKET_NAME);

    /* Create local socket. */

    listen_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (listen_socket == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /*
     * For portability clear the whole structure, since some
     * implementations have additional (nonstandard) fields in
     * the structure.
     */

    memset(&addr, 0, sizeof(struct sockaddr_un));

    /* Bind socket to socket name. */

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

    ret = bind(listen_socket, (const struct sockaddr *)&addr,
               sizeof(struct sockaddr_un));
    if (ret == -1)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    /*
     * Prepare for accepting connections. The backlog size is set
     * to 20. So while one request is being processed other requests
     * can be waiting.
     */

    ret = listen(listen_socket, 20);
    if (ret == -1)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    /* This is the main loop for handling connections. */

    for (;;)
    {

        /* Wait for incoming connection. */

        data_socket = accept(listen_socket, NULL, NULL);
        if (data_socket == -1)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // result = 0;
        char *array[30];


// come??ar a thread aqui 
        static char * socketPath;
        for (;;)
        {

            /* Wait for next data packet. */

            ret = read(data_socket, buffer, BUFFER_SIZE);
            if (ret == -1)
            {
                perror("read");
                exit(EXIT_FAILURE);
            }

            /* Ensure buffer is 0-terminated. */

            buffer[BUFFER_SIZE - 1] = 0;

            /* Handle commands. */

            if (!strncmp(buffer, "DOWN", BUFFER_SIZE))
            {
                down_flag = 1;
                break;
            }

            if (!strncmp(buffer, "END", BUFFER_SIZE))
            {
                break;
            }

            int i = 0;
            char *p = strtok(buffer, " ");

            while (p != NULL)
            {
                array[i++] = p;
                p = strtok(NULL, " ");
            }
            printf("Received command %s from %s\n", array[0], array[1]);
            printf("Searching for this command in the list of commands\n");

            // printf("%s\n", array[0]);
            // printf("%s\n", array[1]);
            // printf("%s\n", array[2]);

            socketPath = fetchCommandAndSocket(atoi(array[0]));

            
        }
        
        printf("Socket name to be utilized: %s\n", socketPath);
        
        periphericModulesCallMain(socketPath, array[0]);

        /* Send result. */

        ret = write(data_socket, "Command module received request: OK\n", BUFFER_SIZE);

        if (ret == -1)
        {
            perror("write");
            exit(EXIT_FAILURE);
        }

        strcpy (buffer, "END");
        ret = write(data_socket, buffer, strlen(buffer) + 1);
        if (ret == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        /* Close socket. */

        close(data_socket);

        /* Quit on DOWN command. */

        if (down_flag)
        {
            break;
        }
    }

    close(listen_socket);

    /* Unlink the socket. */

    unlink(SOCKET_NAME);

    exit(EXIT_SUCCESS);
}
