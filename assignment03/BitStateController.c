/*
# Name: Aiden Otto
# Date: 10/26/25
# Description: Program that sets individual bits in a uint8_t and dispays the bits in the terminal
*/

#include "BitStateController.h"

void BitStateController_SetBitState(BitStateController* bitStateController, Bit bit, BitState bitState)
{
	// use the coresponding mask for the desired bit
	switch (bit) {
		case Bit_1:
		// if bitState is 1, bitwise OR the bitStates with the mask to flip the desired bit to 1
		if (bitState) {
			bitStateController->bitStates |= BIT_ONE_BIT_MASK;

		}
		// if the bitState is not 1 (0), bitwise AND the bitState with the compliment of the mask to flip the desired bit to 0
		else {
			bitStateController->bitStates &= ~BIT_ONE_BIT_MASK;
		}
		break;

		case Bit_2:
		if (bitState) {
			bitStateController->bitStates |= BIT_TWO_BIT_MASK;
		}
		else {
			bitStateController->bitStates &= ~BIT_TWO_BIT_MASK;
		}
		break;

		case Bit_3:
		if (bitState) {
			bitStateController->bitStates |= BIT_THREE_BIT_MASK;
		}
		else {
			bitStateController->bitStates &= ~BIT_THREE_BIT_MASK;
		}
		break;

		case Bit_4:
		if (bitState) {
			bitStateController->bitStates |= BIT_FOUR_BIT_MASK;
		}
		else {
			bitStateController->bitStates &= ~BIT_FOUR_BIT_MASK;
		}
		break;

		case Bit_5:
		if (bitState) {
			bitStateController->bitStates |= BIT_FIVE_BIT_MASK;
		}
		else {
			bitStateController->bitStates &= ~BIT_FIVE_BIT_MASK;
		}
		break;

		case Bit_6:
		if (bitState) {
			bitStateController->bitStates |= BIT_SIX_BIT_MASK;
		}
		else {
			bitStateController->bitStates &= ~BIT_SIX_BIT_MASK;
		}
		break;

		case Bit_7:
		if (bitState) {
			bitStateController->bitStates |= BIT_SEVEN_BIT_MASK;
		}
		else {
			bitStateController->bitStates &= ~BIT_SEVEN_BIT_MASK;
		}
		break;

		case Bit_8:
		if (bitState) {
			bitStateController->bitStates |= BIT_EIGHT_BIT_MASK;
		}
		else {
			bitStateController->bitStates &= ~BIT_EIGHT_BIT_MASK;
		}
		break;
	}
}

BitState BitStateController_GetBitState(BitStateController* bitStateController, Bit bit)
{
	BitState switchState = Off;

	// use the coresponding mask for the desired bit
	switch(bit) {
		case Bit_1:
			// if the bitwise AND of bitStates and the mask is != 0, set the switchState to On
			if (bitStateController->bitStates & BIT_ONE_BIT_MASK) {
				switchState = On;
			// if its not not 0, set it to Off
			} else {
				switchState = Off;
			}
			break;

		case Bit_2:
			if (bitStateController->bitStates & BIT_TWO_BIT_MASK) {
				switchState = On;
			} else {
				switchState = Off;
			}
			break;

		case Bit_3:
			if (bitStateController->bitStates & BIT_THREE_BIT_MASK) {
				switchState = On;
			} else {
				switchState = Off;
			}
			break;

		case Bit_4:
			if (bitStateController->bitStates & BIT_FOUR_BIT_MASK) {
				switchState = On;
			} else {
				switchState = Off;
			}
			break;

		case Bit_5:
			if (bitStateController->bitStates & BIT_FIVE_BIT_MASK) {
				switchState = On;
			} else {
				switchState = Off;
			}
			break;

		case Bit_6:
			if (bitStateController->bitStates & BIT_SIX_BIT_MASK) {
				switchState = On;
			} else {
				switchState = Off;
			}
			break;

		case Bit_7:
			if (bitStateController->bitStates & BIT_SEVEN_BIT_MASK) {
				switchState = On;
			} else {
				switchState = Off;
			}
			break;

		case Bit_8:
			if (bitStateController->bitStates & BIT_EIGHT_BIT_MASK) {
				switchState = On;
			} else {
				switchState = Off;
			}
			break;
	}
	
	return switchState;
}