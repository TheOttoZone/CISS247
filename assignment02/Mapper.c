/*
# Name: Aiden Otto
# Date: 10/17/25
# Description: Assignment 02
*/

#include "Mapper.h"
#include <stdio.h>

void OutputAsHexCharacters(unsigned int value)
{
	unsigned int mask = 0x000000F;
    unsigned int nibble = 0;
    unsigned int returnValue = 0;
    unsigned char finalHex[9] = {0};

    for(int i = 1; i < 9; i++){
        nibble = value & mask;
        mask = mask << 4;
        nibble = nibble >> ((i * 4) - 4);
        finalHex[9-i] = MapNibbleToHexCharacter(nibble);
        //printf("%c", MapNibbleToHexCharacter(nibble));
    }

    printf("\nHexidecimal Characters: 0x");
    for(int i = 0; i < 9; i++){
        printf("%c", finalHex[i]);
    }
    printf("\n\n");
}

unsigned char MapNibbleToHexCharacter(unsigned int value)
{
    char hexArray[] = "0123456789ABCDEF";
    return hexArray[value];
}
