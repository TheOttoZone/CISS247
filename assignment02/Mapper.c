#include "Mapper.h"
#include <stdio.h>

void OutputAsHexCharacters(unsigned int value)
{
	unsigned int mask = 0x000000F;
    unsigned int nibble = 0;
    unsigned int returnValue = 0;
    char array[8];

    for(int i = 1; i < 9; i++){
        nibble = value & mask;
        mask = mask << 4;
        nibble = nibble >> ((i * 4) - 4);
        printf("%c\n", MapNibbleToHexCharacter(nibble));
        //returnValue = returnValue | nibble; 
        //printf("0x%08X\n", returnValue); 
    }

}

unsigned char MapNibbleToHexCharacter(unsigned int value)
{
    char hexArray[] = "0123456789ABCDEF";
    return hexArray[value];
}
