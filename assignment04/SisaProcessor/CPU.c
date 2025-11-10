#include <stdio.h>
#include <string.h>
#include "CPU.h"
#include "MachineOpcodes.h"
#include "InstructionFields.h"
#include "PrintingFormats.h"

#define PAUSE_AFTER_INSTRUCTION_PROCESSED 1

//PRIVATE FUNCTION PROTOTYPES
void CPUFetchInstruction(CPU* cpuPtr);
void CPUDecodeInstruction(CPU* cpuPtr, InstructionFields* instructionFieldsPtr);
void CPUExecuteInstruction(CPU* cpuPtr, InstructionFields* instructionFieldsPtr);
void CPUParseInstruction(CPU* cpuPtr, InstructionFields* instructionFieldsPtr);
void CPUSetMultiplexers(CPU* cpuPtr, Controller* controllerPtr);
void CPUReadALUSourceRegisters(CPU* cpuPtr, InstructionFields* instructionFieldsPtr);
void CPUExecuteALU(CPU* cpuPtr, InstructionFields* instructionFieldsPtr);
void CPUProcessUnconditionalBranchInstruction(CPU* cpuPtr, InstructionFields* instructionFieldsPtr);
void CPUProcessConditionalBranchInstruction(InstructionFields* instructionFieldsPtr, CPU* cpuPtr);
void CPUWriteToDestinationRegister(CPU* cpuPtr, InstructionFields* instructionFieldsPtr);
void CPUWriteRegisterToMemory(CPU* cpuPtr, InstructionFields* instructionFieldsPtr);
void CPUUpdatePC(CPU* cpuPtr, InstructionFields* instructionFieldsPtr);
void CPUPrintDecodedInformation(CPU* cpuPtr, uint16_t instructionCounter, InstructionFields* instructionFieldsPtr);
void CPUPrintExecutionStateInformation(CPU* cpuPtr, InstructionFields* instructionFieldsPtr, uint16_t startingMainMemoryAddress, int programSize);
void CPUPrintInstruction(CPU* cpuPtr, uint16_t instructionCounter, InstructionFields* instructionFieldsPtr);

//MODULE LEVEL VARIABLES
int pauseAfterInstructionProcessed = 0;

void CPUInitialize(CPU* cpuPtr, MainMemory* mainMemoryPtr)
{
    MMUInitialize(&cpuPtr->mmu, mainMemoryPtr);
}

int16_t CPUExecuteProgram(CPU* cpuPtr, uint16_t startingMainMemoryAddress, int programSize)
{
    uint16_t instructionCounter = 0;

    InstructionFields instructionFields;
    InstructionFields* instructionFieldsPtr;
    instructionFieldsPtr = &instructionFields;

    cpuPtr->PC_Register = startingMainMemoryAddress;
    
    do
    {
        instructionCounter++;

        //Fetch Stage
        CPUFetchInstruction(cpuPtr);
        CPUPrintInstruction(cpuPtr, instructionCounter, instructionFieldsPtr);

        //Decode Stage
        CPUDecodeInstruction(cpuPtr, instructionFieldsPtr);
        CPUPrintDecodedInformation(cpuPtr, instructionCounter, instructionFieldsPtr);
        
        //Execute Stage
        CPUExecuteInstruction(cpuPtr, instructionFieldsPtr);
        CPUPrintExecutionStateInformation(cpuPtr, instructionFieldsPtr, startingMainMemoryAddress, programSize);
        
        //PC Update Stage
        CPUUpdatePC(cpuPtr, instructionFieldsPtr);

        printf("*************************END PROCESSING INSTRUCTION*************************\n\n");

        if (pauseAfterInstructionProcessed == PAUSE_AFTER_INSTRUCTION_PROCESSED)
        {
            printf("Press enter key to execute next instruction...\n");

            int c = getchar();
        }

    } while (instructionFields.opcode != OPCODE_HALT_HEX);

    return cpuPtr->registers.values[R0];
}

void CPUFetchInstruction(CPU* cpuPtr)
{
    uint16_t instruction = cpuPtr->mmu.mainMemoryPtr->values[cpuPtr->PC_Register];

    cpuPtr->IR_Register = instruction;
}

//Determine the opcode, register addresses, and constant value.
//The constant values could be an 8-bit immediate for a MOVI instruction:  
//  4-bit offset value for a LDR/STR instruction
//  8-bit immediate for a conditional branch instruction
//  12-bit offset for an unconditional branch instructio
void CPUDecodeInstruction(CPU* cpuPtr, InstructionFields* instructionFieldsPtr)
{
    CPUParseInstruction(cpuPtr, instructionFieldsPtr);

    ControllerSetControlRegister(&cpuPtr->controller, instructionFieldsPtr->opcode);

    ALUControllerSetControlRegisters(&cpuPtr->aluController, &cpuPtr->controller);

    CPUSetMultiplexers(cpuPtr, &cpuPtr->controller);
}

void CPUExecuteInstruction(CPU* cpuPtr, InstructionFields* instructionFieldsPtr)
{
    if (cpuPtr->controller.uncondBranch == UNCOND_BRANCH_FLAG_ON)
    {
        //This is the only instruction that doesn't require the ALU
        CPUProcessUnconditionalBranchInstruction(cpuPtr, instructionFieldsPtr);
    }
    
    if (cpuPtr->controller.uncondBranch == UNCOND_BRANCH_FLAG_OFF) 
    {
        //All other instructions require the ALU

        CPUReadALUSourceRegisters(cpuPtr, instructionFieldsPtr);

        CPUExecuteALU(cpuPtr, instructionFieldsPtr);

        if (cpuPtr->controller.condBranch == COND_BRANCH_FLAG_ON)
        {
            CPUProcessConditionalBranchInstruction(instructionFieldsPtr, cpuPtr);
        }
        
        if (cpuPtr->controller.condBranch == COND_BRANCH_FLAG_OFF)
        {
            if (cpuPtr->controller.regWrite == REG_WRITE_FLAG_ON)
                CPUWriteToDestinationRegister(cpuPtr, instructionFieldsPtr);

            if (cpuPtr->controller.memWrite == MEM_WRITE_FLAG_ON)
                CPUWriteRegisterToMemory(cpuPtr, instructionFieldsPtr);
        }
    }
}

void CPUUpdatePC(CPU* cpuPtr, InstructionFields* instructionFieldsPtr)
{
    if (cpuPtr->pcSourceSelector == PC_SOURCE_SELECT_BRANCH)
    {
        //TODO: Move to PC Branch-Offset Adder
        
        //Branch offsets are in bytes. Instructions are two-byte words.
        //Convert to word offsets by dividing by two.
        int8_t byteToWordAdjustment = instructionFieldsPtr->immediate >> 1;

        cpuPtr->PC_Register = cpuPtr->PC_Register + byteToWordAdjustment;
    }
    else
    {
       (cpuPtr->PC_Register)++; //Move to PC Adder
    }
}

void CPUProcessUnconditionalBranchInstruction(CPU* cpuPtr, InstructionFields* instructionFieldsPtr)
{
    cpuPtr->pcSourceSelector = PC_SOURCE_SELECT_BRANCH;
}

void CPUProcessConditionalBranchInstruction(InstructionFields* instructionFieldsPtr, CPU* cpuPtr)
{
    if (instructionFieldsPtr->opcode == OPCODE_BE_HEX)
    {
        if (cpuPtr->alu.zeroFlag == ALU_FLAG_ON)
        {
            cpuPtr->pcSourceSelector = PC_SOURCE_SELECT_BRANCH;
        }
    }

    if (instructionFieldsPtr->opcode == OPCODE_BNE_HEX)
    {
        if (cpuPtr->alu.zeroFlag == ALU_FLAG_OFF)
        {
            cpuPtr->pcSourceSelector = PC_SOURCE_SELECT_BRANCH;
        }
    }

    if (instructionFieldsPtr->opcode == OPCODE_BL_HEX)
    {
        if (cpuPtr->alu.lessZeroFlag == ALU_FLAG_ON)
        {
            cpuPtr->pcSourceSelector = PC_SOURCE_SELECT_BRANCH;
        }
    }

    if (instructionFieldsPtr->opcode == OPCODE_BG_HEX)
    {
        if (cpuPtr->alu.greaterZeroFlag == ALU_FLAG_ON)
        {
            cpuPtr->pcSourceSelector = PC_SOURCE_SELECT_BRANCH;
        }
    }

    if (instructionFieldsPtr->opcode == OPCODE_BLE_HEX)
    {
        if ((cpuPtr->alu.zeroFlag == ALU_FLAG_ON) || (cpuPtr->alu.lessZeroFlag == ALU_FLAG_ON))
        {
            cpuPtr->pcSourceSelector = PC_SOURCE_SELECT_BRANCH;
        }
    }

    if (instructionFieldsPtr->opcode == OPCODE_BGE_HEX)
    {
        if ((cpuPtr->alu.zeroFlag == ALU_FLAG_ON) || (cpuPtr->alu.greaterZeroFlag == ALU_FLAG_ON))
        {
            cpuPtr->pcSourceSelector = PC_SOURCE_SELECT_BRANCH;
        }
    }
}

void CPUParseInstruction(CPU* cpuPtr, InstructionFields* instructionFieldsPtr)
{
    uint16_t result = cpuPtr->IR_Register & OPCODE_BITMASK;
    instructionFieldsPtr->opcode = (uint8_t)(result >> OPCODE_BIT_SHIFT);

    switch (instructionFieldsPtr->opcode)
    {
        case OPCODE_MOVI_HEX:
        {
            result = cpuPtr->IR_Register & RD_Field_BITMASK;
            instructionFieldsPtr->RD_RT = (uint8_t)(result >> RD_FIELD_BIT_SHIFT);

            instructionFieldsPtr->RM = 0;

            instructionFieldsPtr->RN = 0;

            result = cpuPtr->IR_Register & IMM_FIELD_BITMASK;
            instructionFieldsPtr->immediate = (int8_t)(result >> IMM_FIELD_BIT_SHIFT);

            break;
        }
        case OPCODE_ADD_HEX:
        case OPCODE_SUB_HEX:
        case OPCODE_CMP_HEX:
        case OPCODE_MUL_HEX:
        case OPCODE_DIV_HEX:
        {
            result = cpuPtr->IR_Register & RD_Field_BITMASK;
            instructionFieldsPtr->RD_RT = (uint8_t)(result >> RD_FIELD_BIT_SHIFT);

            result = cpuPtr->IR_Register & RM_Field_BITMASK;
            instructionFieldsPtr->RM = (uint8_t)(result >> RM_FIELD_BIT_SHIFT);

            result = cpuPtr->IR_Register & RN_Field_BITMASK;
            instructionFieldsPtr->RN = (uint8_t)(result >> RN_FIELD_BIT_SHIFT);

            instructionFieldsPtr->immediate = 0;

            break;
        }
        case OPCODE_LDR_HEX:
        case OPCODE_STR_HEX:
        {
            result = cpuPtr->IR_Register & RT_Field_BITMASK;
            instructionFieldsPtr->RD_RT = (uint8_t)(result >> RT_FIELD_BIT_SHIFT);

            result = cpuPtr->IR_Register & RM_Field_BITMASK;
            instructionFieldsPtr->RM = (uint8_t)(result >> RM_FIELD_BIT_SHIFT);

            instructionFieldsPtr->RN = 0;

            result = cpuPtr->IR_Register & BASE_OFFSET_FIELD_BITMASK;
            instructionFieldsPtr->immediate = (int8_t)(result >> BASE_OFFSET_FIELD_BIT_SHIFT);

            break;
        }
        case OPCODE_BE_HEX:
        case OPCODE_BNE_HEX:
        case OPCODE_BL_HEX:
        case OPCODE_BG_HEX:
        case OPCODE_BLE_HEX:
        case OPCODE_BGE_HEX:
        {
            result = cpuPtr->IR_Register & RT_Field_BITMASK;
            instructionFieldsPtr->RD_RT = (uint8_t)(result >> RT_FIELD_BIT_SHIFT);

            instructionFieldsPtr->RM = 0;

            instructionFieldsPtr->RN = 0;

            result = cpuPtr->IR_Register & CBRANCH_PC_OFFSET_FIELD_BITMASK;
            instructionFieldsPtr->immediate = (int8_t)(result >> CBRANCH_PC_OFFSET_FIELD_BIT_SHIFT);

            break;
        }
        case OPCODE_B_HEX:
        {
            instructionFieldsPtr->RD_RT = 0;

            instructionFieldsPtr->RM = 0;

            instructionFieldsPtr->RN = 0;

            result = cpuPtr->IR_Register & UBRANCH_PC_OFFSET_FIELD_BITMASK;

            //Sign-extend from 12 bits to 16 bits.
            instructionFieldsPtr->immediate = (int16_t)(result << UBRANCH_PC_OFFSET_FIELD_BIT_SHIFT);
            instructionFieldsPtr->immediate = (int16_t)(instructionFieldsPtr->immediate >> UBRANCH_PC_OFFSET_FIELD_BIT_SHIFT);

            break;
        }
        case OPCODE_HALT_HEX:
        {
            instructionFieldsPtr->RD_RT = 0;
            instructionFieldsPtr->RM = 0;
            instructionFieldsPtr->RN = 0;
            instructionFieldsPtr->immediate = 0;

            break;
        }
        default:
        {
            instructionFieldsPtr->RD_RT = 0;
            instructionFieldsPtr->RM = 0;
            instructionFieldsPtr->RN = 0;
            instructionFieldsPtr->immediate = 0;

            break;
        }
    }
}

void CPUSetMultiplexers(CPU* cpuPtr, Controller* controllerPtr)
{
    cpuPtr->aluDataInputBSourceMultiplexer.dataSelector = controllerPtr->aluInputBSourceSelector;
    cpuPtr->registerWriteDataSourceMultiplexer.dataSelector = controllerPtr->regWriteSourceSelector;
    cpuPtr->pcSourceSelector = PC_SOURCE_SELECT_NORMAL;
}

void CPUReadALUSourceRegisters(CPU* cpuPtr, InstructionFields* instructionFieldsPtr)
{
    if (cpuPtr->aluDataInputBSourceMultiplexer.dataSelector == ALU_INPUT_B_SOURCE_SELECT_REG)
    {
        cpuPtr->alu.dataInputA = cpuPtr->registers.values[instructionFieldsPtr->RM];
        cpuPtr->alu.dataInputB = cpuPtr->registers.values[instructionFieldsPtr->RN];
    }
    else if (cpuPtr->aluDataInputBSourceMultiplexer.dataSelector == ALU_INPUT_B_SOURCE_SELECT_IMM)
    {
        //Determine if the immediate is a 8-bit MOVI immediate or a 4-bit LDR/STR offset.
        //If the MEM_READ_FLAG_ON flag or the MEM_WRITE_FLAG_ON flag is on, it's a 4-bit memory data offset
        //otherwise, it's an 8-bit MOVI immediate.

        if ((cpuPtr->controller.memRead == MEM_READ_FLAG_ON) || (cpuPtr->controller.memWrite == MEM_WRITE_FLAG_ON))
        {
            cpuPtr->alu.dataInputA = cpuPtr->registers.values[instructionFieldsPtr->RM];
            cpuPtr->alu.dataInputB = instructionFieldsPtr->immediate;
        }
        else
        {
            //MOVI is a pass-through operation. Input A isn't used.
            cpuPtr->alu.dataInputA = 0;
            cpuPtr->alu.dataInputB = instructionFieldsPtr->immediate;
        }
    }
}

void CPUExecuteALU(CPU* cpuPtr, InstructionFields* instructionFieldsPtr)
{
    if (cpuPtr->aluController.aluOpMovi == ALU_OP_SIG_ON)
        //MOVI is a pass-through operation. Input A isn't used.
        cpuPtr->alu.dataOutput = cpuPtr->alu.dataInputB; 

    if (cpuPtr->aluController.aluOpAdd == ALU_OP_SIG_ON)
        cpuPtr->alu.dataOutput = cpuPtr->alu.dataInputA + cpuPtr->alu.dataInputB;

    if (cpuPtr->aluController.aluOpSub == ALU_OP_SIG_ON)
        cpuPtr->alu.dataOutput = cpuPtr->alu.dataInputA - cpuPtr->alu.dataInputB;

    if (cpuPtr->aluController.aluOpCmp == ALU_OP_SIG_ON)
    {
        cpuPtr->alu.zeroFlag = ALU_FLAG_OFF;
        cpuPtr->alu.lessZeroFlag = ALU_FLAG_OFF;
        cpuPtr->alu.greaterZeroFlag = ALU_FLAG_OFF;

        int16_t aluResult = cpuPtr->alu.dataInputA - cpuPtr->alu.dataInputB;

        cpuPtr->alu.dataOutput = aluResult;

        if (aluResult == 0) cpuPtr->alu.zeroFlag = ALU_FLAG_ON;
        if (aluResult < 0) cpuPtr->alu.lessZeroFlag = ALU_FLAG_ON;
        if (aluResult > 0) cpuPtr->alu.greaterZeroFlag = ALU_FLAG_ON;
    }

    if (cpuPtr->aluController.aluOpMul == ALU_OP_SIG_ON)
        cpuPtr->alu.dataOutput = cpuPtr->alu.dataInputA * cpuPtr->alu.dataInputB;

    if (cpuPtr->aluController.aluOpDiv == ALU_OP_SIG_ON)
        cpuPtr->alu.dataOutput = cpuPtr->alu.dataInputA / cpuPtr->alu.dataInputB;
}

void CPUWriteToDestinationRegister(CPU* cpuPtr, InstructionFields* instructionFieldsPtr)
{
    if (cpuPtr->controller.regWrite == REG_WRITE_FLAG_ON)
    {
        if (cpuPtr->registerWriteDataSourceMultiplexer.dataSelector == REG_WRITE_SOURCE_SELECT_ALU)
        {
            cpuPtr->registers.values[instructionFieldsPtr->RD_RT] = cpuPtr->alu.dataOutput;
        }

        if (cpuPtr->registerWriteDataSourceMultiplexer.dataSelector == REG_WRITE_SOURCE_SELECT_MEM)
        {
            uint16_t mainMemoryValue = cpuPtr->mmu.mainMemoryPtr->values[cpuPtr->alu.dataOutput];
            cpuPtr->registers.values[instructionFieldsPtr->RD_RT] = mainMemoryValue;
        }
    }
}

void CPUWriteRegisterToMemory(CPU* cpuPtr, InstructionFields* instructionFieldsPtr)
{
    cpuPtr->mmu.mainMemoryPtr->values[cpuPtr->alu.dataOutput] = cpuPtr->registers.values[instructionFieldsPtr->RD_RT];
}

void CPUPrintInstruction(CPU* cpuPtr, uint16_t instructionCounter, InstructionFields* instructionFieldsPtr)
{
    printf("********************** BEGIN PROCESSING INSTRUCTION: %2d ********************\n\n", instructionCounter);

    printf("Address (PC):      Hex: 0x%04X Decimal: %d \n", cpuPtr->PC_Register, cpuPtr->PC_Register);
    printf("Instruction (IR):  Hex: 0x%04X ", cpuPtr->IR_Register);
    printf("Binary: ");
    printf(SIXTEEN_BIT_BYTE_TO_BINARY_PATTERN, SIXTEEN_BIT_BYTE_TO_BINARY(cpuPtr->IR_Register));
    printf("\n\n");
}

void CPUPrintDecodedInformation(CPU* cpuPtr, uint16_t instructionCounter, InstructionFields* instructionFieldsPtr)
{
    InstructionFieldsPrintState(instructionFieldsPtr);
}

void CPUPrintExecutionStateInformation(CPU* cpuPtr, InstructionFields* instructionFieldsPtr, 
    uint16_t startingMainMemoryAddress, int programSize)
{
    ControllerPrintControlRegister(&cpuPtr->controller);
    ALUControllerPrintControlRegister(&cpuPtr->aluController);
    RegistersPrintRegisterValues(&cpuPtr->registers);
    MainMemoryPrintValues(cpuPtr->mmu.mainMemoryPtr, 0, programSize);
    MainMemoryPrintDataSegmentValues(cpuPtr->mmu.mainMemoryPtr, 0, startingMainMemoryAddress);
    MainMemoryPrintCodeSegmentValues(cpuPtr->mmu.mainMemoryPtr, startingMainMemoryAddress, programSize);
}

void CPUSetOptions(int argc, char* argv[])
{
    for (int commandLineParameter = 1; commandLineParameter < argc; commandLineParameter++)
    {
        int result = strcmp(argv[commandLineParameter], "-step");

        if (result == 0) pauseAfterInstructionProcessed = PAUSE_AFTER_INSTRUCTION_PROCESSED;
    }
}

