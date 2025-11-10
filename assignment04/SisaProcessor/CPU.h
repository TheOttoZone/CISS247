#ifndef CPU_H
#define CPU_H

/*******************************************************************************
* Module:       CPU
* Developer:    Michael Koepp
* Company:      WWU Computer Science Department
* Create Date:  
* Description:  
*******************************************************************************/

#include "MainMemory.h"
#include "MMU.h"
#include "ALU.h"
#include "Registers.h"
#include "Controller.h"
#include "ALUController.h"
#include "Adder.h"
#include "Multiplexer.h"

#define PC_SOURCE_SELECT_NORMAL								0
#define PC_SOURCE_SELECT_BRANCH								1

typedef struct
{
	//Program Counter: Stores the address of the machine instruction
	uint16_t PC_Register;

	//Instruction Register: Stores the machine instruction fetched from memory
	uint16_t IR_Register;

	//Program Counter source selector
	uint8_t pcSourceSelector; 

	//General purpose registers
	Registers registers;

	//Arithmetic Logic Unit
	ALU alu;

	//ALU controller
	ALUController aluController;

	//Memory Management Unit
	MMU mmu;

	//CPU main controller for the fetch, decode and execute cycle
	Controller controller;
	
	Adder pcNextAdder;
	Adder pcBranchOffsetAdder;

	Multiplexer aluDataInputBSourceMultiplexer;
	Multiplexer registerWriteDataSourceMultiplexer;
	Multiplexer pcSourceMultiplexer;

}CPU;

void CPUInitialize(CPU* cpuPtr, MainMemory* mainMemoryPtr);
int16_t CPUExecuteProgram(CPU* cpuPtr, uint16_t startingMainMemoryAddress, int programSize);
void CPUSetOptions(int argc, char* argv[]);

#endif // !CPU_H