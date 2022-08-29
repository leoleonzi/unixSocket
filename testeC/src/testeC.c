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



int main(void)
{
  char nome[3];

  printf("Digite seu nome: ");
  scanf("%s",nome);

  printf("O nome armazenado foi: %s", nome);

  return 0;
}
