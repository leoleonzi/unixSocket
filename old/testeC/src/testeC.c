/*
 ============================================================================
 Name        : testeC.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
  char buf[] ="abc/qwe/ccd";

  //printf("Digite seu nome: ");
  //scanf("%s",buf);

  int i = 0;
  char *p = strtok (buf, "/");
  char *array[3];

  while (p != NULL)
      {
          array[i++] = p;
          p = strtok (NULL, "/");
      }
  for (i = 0; i < 3; ++i)
	  printf("%s\n", array[i]);

  printf(array[2]);

  return 0;
}
