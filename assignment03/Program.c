/*
# Name: Aiden Otto
# Date: 10/26/25
# Description: Program that sets individual bits in a uint8_t and dispays the bits in the terminal
*/

#include <stdio.h>
#include <stdint.h>
#include "BitStateController.h"

int main() 
{
	BitStateController bitStateController = {.bitStates = 0};
	
	//Input
	int response = 0;
	Bit bit = None;
	BitState bitState = Off;

	printf("Enter a bit number OR -1 to exit [1..8]: ");
	int returnValue = scanf("%d", &response);
	bit = (Bit)response;
	while (bit != -1)
	{
		printf("Enter a bit state (0-Off,1-On): ");
		returnValue = scanf("%d", &response);
		bitState = (BitState)response;
		
		BitStateController_SetBitState(&bitStateController, bit, bitState);

		printf("\n");

		for (int bitCounter = 1; bitCounter <= BIT_COUNT; bitCounter++)
		{
			BitState state = BitStateController_GetBitState(&bitStateController, bitCounter);

			printf("Bit State %d: %d\n", bitCounter, state);
		}

		printf("\n");

		printf("Enter a bit number OR -1 to exit [1..8]: ");
		returnValue = scanf("%d", &response);
		bit = (Bit)response;
	}

	printf("\n");
	printf("Press the enter key to continue...");

	int c = getchar();
	c = getchar();

	return 0;
}