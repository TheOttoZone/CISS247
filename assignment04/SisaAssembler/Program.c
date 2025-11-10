#include <stdio.h>
#include "Assembler.h"
#include "Testcases.h"

#define USE_COMMAND_LINE_ARGS 1
#define MINIMUM_COMMAND_LINE_PARAMETERS 3

void main(int argc, char* argv[])
{
	printf("Starting assembler...\n\n");

	if (USE_COMMAND_LINE_ARGS == 1)
	{
		if (argc < MINIMUM_COMMAND_LINE_PARAMETERS)
		{
			printf("Usage: sisaassembler AssemblyCodeFileName.aci MachineCodeFileName.bin\n\n");

			printf("\n");
		}
		else
		{
			executeAssembler(argv[1], argv[2]);
		}
	}
	else
	{
		//Testcases_Execute();

		//executeAssembler("AssemblyCodeInstructions_01_HEX.aci", "MachineCodeInstructions_01_HEX.mci");
		//executeAssembler("AssemblyCodeInstructions_01_BIN.aci", "MachineCodeInstructions_01_BIN.mci");
		
		//executeAssembler("AssemblyCodeInstructions_02_HEX.aci", "MachineCodeInstructions_02_HEX.mci");
		//executeAssembler("AssemblyCodeInstructions_02_BIN.aci", "MachineCodeInstructions_02_BIN.mci");
		
		//executeAssembler("AssemblyCodeInstructions_03_HEX.aci", "MachineCodeInstructions_03_HEX.mci");
		//executeAssembler("AssemblyCodeInstructions_03_BIN.aci", "MachineCodeInstructions_03_BIN.mci");
		
		//executeAssembler("ArrayTotal_HEX.aci", "ArrayTotal_HEX.mci");
		//executeAssembler("ArrayTotal_BIN.aci", "ArrayTotal_BIN.mci");
		
		//executeAssembler("ArrayTotal_HEX.aci", "ArrayTotal_HEX.mci");
		//executeAssembler("ArrayMax_BIN.aci", "ArrayMax_BIN.mci");

		executeAssembler("ArraySearch_HEX.aci", "ArraySearch_HEX.mci");
		
	}

	

    printf("Press the enter key to continue...");

	int c = getchar();
}