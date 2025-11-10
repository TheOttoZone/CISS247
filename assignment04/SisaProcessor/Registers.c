#include <stdio.h>
#include "Registers.h"

void RegistersPrintRegisterValues(Registers* registerPtr)
{
	printf("Registers:\n");

	for (int registerIndex = 0; registerIndex < REGISTER_SET_SIZE; registerIndex++)
	{
		printf("%02d: 0x%04hX ", registerIndex, registerPtr->values[registerIndex]);

		if ((registerIndex + 1) % 4 == 0) printf("\n");
	}

	printf("\n");
}