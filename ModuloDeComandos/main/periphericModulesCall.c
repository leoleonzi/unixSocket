// Commands module working as Client when accessing peripheric modules

// To do: collect the unix socket path autoamtically, according to the command id

// #define SOCKET_NAME "/tmp/moduloOTAP.sock"
#define BUFFER_SIZE 30

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

int periphericModulesCallMain(char * SOCKET_NAME)
{
    struct sockaddr_un addr;
    int i;
    int ret;
    int data_socket;
    char buffer[BUFFER_SIZE];
    char dado[BUFFER_SIZE] = "Cmd 1";

    /* Create local socket. */

    data_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (data_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /*
     * For portability clear the whole structure, since some
     * implementations have additional (nonstandard) fields in
     * the structure.
     */

    memset(&addr, 0, sizeof(struct sockaddr_un));

    /* Connect socket to socket address */

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

    ret = connect (data_socket, (const struct sockaddr *) &addr,
                   sizeof(struct sockaddr_un));
    if (ret == -1) {
        fprintf(stderr, "The server is down.\n");
        exit(EXIT_FAILURE);
    }

	ret = write(data_socket, dado, 30);
        if (ret == -1) {
            perror("write");
            // break;
        }

    /* Request close of socket. */

    

    /* Receive result. */

    ret = read(data_socket, buffer, BUFFER_SIZE);
    if (ret == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    /* Ensure buffer is 0-terminated. */

    printf("Result of operation with peripherical module = %s\n", buffer);

    /* Close socket. */

    strcpy (buffer, "END");
    ret = write(data_socket, buffer, strlen(buffer) + 1);
    if (ret == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    close(data_socket);

    exit(EXIT_SUCCESS);

    return 0;
}
