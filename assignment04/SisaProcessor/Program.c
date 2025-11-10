/*******************************************************************************
* Module:       Program
* Developer:    Michael Koepp
* Company:      WWU Computer Science Department
* Create Date:  
* Description:  This program tests an instruction processor implementation. 
*******************************************************************************/

#include <stdio.h>
#include "Computer.h"
#define MINIMUM_COMMAND_LINE_PARAMETERS 1

Computer computer;

void main(int argc, char* argv[])
{
	//Local stack allocated pointer so that we can easily access the 
	//Computer's structure members from the debugger's locals window
	Computer* computerPtr = &computer;

    printf("Starting computer system...\n\n");

	printf("Usage: sisaprocessor MachineCodeFileName.bin [options]\n\n");
	printf("Option: -step 	  Step through execution. Pauses execution after each instruction is processed\n");
	printf("Option: -cusigs   View Main Controller Unit Signals\n");
	printf("Option: -alusigs  View ALU Controller Signals\n");
	printf("Option: -mem 	  View memory of both the data and code segments\n");
	printf("Option: -dmem 	  View the data segment memory\n");
	printf("Option: -cmem 	  View the code segment memory\n");
	printf("\n");

	if (argc >= MINIMUM_COMMAND_LINE_PARAMETERS) 
	{
		ComputerInitialize(&computer);

		CPUSetOptions(argc, argv);
		ControllerSetOptions(argc, argv);
		ALUControllerSetOptions(argc, argv);
		MainMemorySetOptions(argc, argv);

		//int16_t programReturnValue = ComputerExecuteProgram(computerPtr, argv[1]);
		//int16_t programReturnValue = ComputerExecuteProgram(&computer, "MachineCodeInstructions_01.mci");
		//int16_t programReturnValue = ComputerExecuteProgram(&computer, "MachineCodeInstructions_02.mci");
		//int16_t programReturnValue = ComputerExecuteProgram(&computer, "MachineCodeInstructions_03.mci");
		//int16_t programReturnValue = ComputerExecuteProgram(&computer, "ArrayTotal.mci");
		//int16_t programReturnValue = ComputerExecuteProgram(&computer, "ArrayMax.mci");
		//int16_t programReturnValue = ComputerExecuteProgram(&computer, "TestProgram.mci");
		int16_t programReturnValue = ComputerExecuteProgram(&computer, "ArraySearch.mci");
		
		printf("Program return value: %d\n\n", programReturnValue);
	}

	printf("Press the enter key to continue...");

	int c = getchar();
}
