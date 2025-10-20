/*
# Name: Aiden Otto
# Date: 10/17/25
# Description: Assignment 02
*/

#include "Mapper.h"
#include <stdio.h>

void OutputAsHexCharacters(unsigned int value)
{
    // delcare and initialize variables
	unsigned int mask = 0x000000F;
    unsigned int nibble = 0;
    unsigned int returnValue = 0;
    unsigned char finalHex[9] = {0};

    for(int i = 1; i < 9; i++){
        nibble = value & mask; // bitwise AND the input value with the mask to get the nibble that will be converted to hex
        mask = mask << 4; // bitshift the mask left 4 bits to go to the next nibble to mask
        nibble = nibble >> ((i * 4) - 4); // bitshift the nibble by 4 times the amount of loops we have done to be able to properly convert it
        finalHex[9-i] = MapNibbleToHexCharacter(nibble); // insert the converted nibble at the correct place in the array 
    }

    // prepare the output of the hex array
    printf("\nHexidecimal Characters: 0x");
    // array output loop
    for(int i = 0; i < 9; i++){
        printf("%c", finalHex[i]);
    }
    printf("\n\n");
}

unsigned char MapNibbleToHexCharacter(unsigned int value)
{
    // create a char array of all hex characters because this assingment wants me to use chars for whatever reason
    char hexArray[] = "0123456789ABCDEF";
    // use the current nibble value as the index for this character array
    return hexArray[value];
}
