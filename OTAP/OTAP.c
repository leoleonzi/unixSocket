#define SOCKET_NAME "/tmp/moduloOTAP.sock"
#define BUFFER_SIZE 30

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>

int main(){

    
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
    if (listen_socket == -1) {
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

    ret = bind(listen_socket, (const struct sockaddr *) &addr,
               sizeof(struct sockaddr_un));
    if (ret == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    /*
     * Prepare for accepting connections. The backlog size is set
     * to 20. So while one request is being processed other requests
     * can be waiting.
     */

    ret = listen(listen_socket, 20);
    if (ret == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    /* This is the main loop for handling connections. */

    for (;;) {

        /* Wait for incoming connection. */

        data_socket = accept(listen_socket, NULL, NULL);
        if (data_socket == -1) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        //result = 0;
        for(;;) {

            /* Wait for next data packet. */

            ret = read(data_socket, buffer, BUFFER_SIZE);
            if (ret == -1) {
                perror("read");
                exit(EXIT_FAILURE);
            }

            /* Ensure buffer is 0-terminated. */

            buffer[BUFFER_SIZE - 1] = 0;

            /* Handle commands. */

            if (!strncmp(buffer, "DOWN", BUFFER_SIZE)) {
                down_flag = 1;
                break;
            }

            if (!strncmp(buffer, "END", BUFFER_SIZE)) {
                break;
            }

            int i = 0;
            char *p = strtok (buffer, " ");
            char *array[30];

            while (p != NULL)
                {
                    array[i++] = p;
                    p = strtok (NULL, " ");
                }
            printf("Received command %s from Command Module", array[0]);

        }

        /* Send result. */

        ret = write(data_socket, "Command module received request: OK", BUFFER_SIZE);

        if (ret == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        /* Close socket. */

        close(data_socket);

        /* Quit on DOWN command. */

        if (down_flag) {
            break;
        }
    }

    close(listen_socket);

    /* Unlink the socket. */

    unlink(SOCKET_NAME);

    exit(EXIT_SUCCESS);

    return 0;
}
