/*
# Name: Aiden Otto
# Date: 10/17/25
# Description: Assignment 02
*/

#include <stdio.h>
#include "Mapper.h"

int main() 
{
   
	//Input
	unsigned int value = 0;
	
	printf("Enter an unsigned integer value: ");
	int returnValue = scanf("%u", &value);

	//Process & Output
	OutputAsHexCharacters(value);

	// yeah theres definitely code here to implement a "press enter key to continue"
	printf("Press the enter key to continue...\n");

	return 0;
}
