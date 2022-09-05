#define BUFFER_SIZE 30



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// char *main()
char *fetchCommandAndSocket(int commandId)
{

    // int commandId = 15;

    char *socketPath[BUFFER_SIZE];
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen("../ModuloDeComandosShell/listaDeComandos.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    int i = 0;
    char *p;
    char *array[30];
    bool foundId;
    static char socketPath3[30];
    
    while ((read = getline(&line, &len, fp)) != -1)
    {
        int i = 0;
        p = strtok(line, " ");
        while (p != NULL)
        {
            array[i++] = p;
            p = strtok(NULL, " ");
        }
        // printf("Received command %s from %s, path %s\n", array[0], array[1], array[2]);
        foundId = (atoi(array[0])==commandId);

        if (foundId == 1){
            strcpy(socketPath3, array[2]);
            break;
        }
    }

    if (foundId == 0){
        printf("Command %d not found\n", commandId);
    }

    else printf("Command %s found. Responsible for execution: %s. \nUnix Domain Socket Path utilized %s\n", array[0], array[1], socketPath3);

    
    fclose(fp);
    if (line)
        free(line);

    return socketPath3;
}