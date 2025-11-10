#pragma warning(disable : 4996)
//https://www.tutorialspoint.com/c_standard_library/c_function_fgets.htm

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Assembler.h"
#include "RegisterSet.h"
#include "InstructionOpcodes.h"

//Private Functions
void assemble(char* assemblyFilename, char* machineCodeFilename);

void generateImmediateTypeMachineCodeInstruction(char* token, unsigned short* machineCodeInstruction);
void generateRegisterTypeMachineCodeInstruction(char* token, unsigned short* machineCodeInstruction);
void generateDataTypeMachineCodeInstruction(char* token, unsigned short* machineCodeInstruction);
void generateUnconditionalBranchTypeMachineCodeInstruction(char* token, unsigned short* machineCodeInstruction);
void generateConditionalBranchTypeMachineCodeInstruction(char* token, unsigned short* machineCodeInstruction);
void writeMachineCodeInstructionToFile(FILE* fptr, unsigned short machineInstruction);
void writeDssSize(FILE* fptr, char* dssSize);
void writeDssWord(FILE* fptr, char* dssWord);
void appendOpcode(char* mnemonic, unsigned short* machineInstruction, unsigned short leftShift);
void appendRegisterNumber(char* registerNumber, unsigned short* machineInstruction, unsigned short leftShift);
void appendImmediateValue(char* immediateValue, unsigned short* machineInstruction, unsigned short leftShift);

void executeAssembler(char* assemblyFilename, char* machineCodeFilename)
{
    int test_case_result = 0;

    printf("************************BEGIN ASSEMBLY PROCESS************************\n\n");

    printf("Assembly code instruction file: %s\n", assemblyFilename);
    printf("Machine code instruction file:  %s\n\n", machineCodeFilename);

    assemble(assemblyFilename, machineCodeFilename);

    printf("\n");

    printf("Assembly complete...\n\n");

    printf("Generated machine code instructions...\n\n");

    unsigned short machineCodeInstruction = 0;

    FILE* fptrGeneratedObjectCode = fopen(machineCodeFilename, "rb");
    
    size_t result = fread(&machineCodeInstruction, sizeof(unsigned short), 1, fptrGeneratedObjectCode);

    while (result != 0)
    {
        
        printf("0x%04x\n", machineCodeInstruction);

        result = fread(&machineCodeInstruction, sizeof(unsigned short), 1, fptrGeneratedObjectCode);
    }

    printf("\n");

    printf("*************************END ASSEMBLY PROCESS*************************\n\n");
}

void assemble(char* assemblyFilename, char* objectCodeFilename)
{
    char fileline[ASSEMBLY_STATEMENT_BUFFER_SIZE] = { 0 };
    
    FILE* fptrRead;
    FILE* fptrWrite;

    fptrRead = fopen(assemblyFilename, "r");
    fptrWrite = fopen(objectCodeFilename, "wb");

    char* result = fgets(fileline, ASSEMBLY_STATEMENT_BUFFER_SIZE, fptrRead);

    while (result != 0)
    {
        unsigned short machineCodeInstruction = 0;
        int cmpResult = 0;

        fileline[strcspn(fileline, "\r")] = 0; //Linux
        fileline[strcspn(fileline, "\n")] = 0; //Windows

        printf("Translating assembly statement: %s\n", fileline);

        char* token = strtok(fileline, " ");

        cmpResult = strcmp(token, DSS);

        if (cmpResult == 0)
        {
            //DSS b0000000000000011
            //DSS h0003

            char* dssSize = strtok(NULL, "\0");
            
            writeDssSize(fptrWrite, dssSize);
        }

        cmpResult = strcmp(token, DWORD);

        if (cmpResult == 0)
        {
            //DWORD b0000000000000001
            //DWORD h0001

            char* dssDword = strtok(NULL, "\0");

            writeDssWord(fptrWrite, dssDword);
        }

        cmpResult = strcmp(token, MOVI);

        if (cmpResult == 0)
        {
            //MOVI R0, b00000001
            //MOVI R0, h01

            unsigned short machineCodeInstruction = 0;

            generateImmediateTypeMachineCodeInstruction(token, &machineCodeInstruction);

            writeMachineCodeInstructionToFile(fptrWrite, machineCodeInstruction);
        }

        cmpResult = strcmp(token, ADD);

        if (cmpResult == 0)
        {
            //ADD R2, R0, R1

            unsigned short machineCodeInstruction = 0;

            generateRegisterTypeMachineCodeInstruction(token, &machineCodeInstruction);

            writeMachineCodeInstructionToFile(fptrWrite, machineCodeInstruction);
        }

        cmpResult = strcmp(token, SUB);

        if (cmpResult == 0)
        {
            //SUB R2, R0, R1

            unsigned short machineCodeInstruction = 0;

            generateRegisterTypeMachineCodeInstruction(token, &machineCodeInstruction);

            writeMachineCodeInstructionToFile(fptrWrite, machineCodeInstruction);
        }

        cmpResult = strcmp(token, CMP);

        if (cmpResult == 0)
        {
            //CMP R3, R1, R2

            unsigned short machineCodeInstruction = 0;

            generateRegisterTypeMachineCodeInstruction(token, &machineCodeInstruction);

            writeMachineCodeInstructionToFile(fptrWrite, machineCodeInstruction);
        }

        cmpResult = strcmp(token, MUL);

        if (cmpResult == 0)
        {
            //MUL R2, R0, R1

            unsigned short machineCodeInstruction = 0;

            generateRegisterTypeMachineCodeInstruction(token, &machineCodeInstruction);

            writeMachineCodeInstructionToFile(fptrWrite, machineCodeInstruction);
        }

        cmpResult = strcmp(token, DIV);

        if (cmpResult == 0)
        {
            //DIV R2, R0, R1

            unsigned short machineCodeInstruction = 0;

            generateRegisterTypeMachineCodeInstruction(token, &machineCodeInstruction);

            writeMachineCodeInstructionToFile(fptrWrite, machineCodeInstruction);
        }

        cmpResult = strcmp(token, LDR);

        if (cmpResult == 0)
        {
            //LDR R1, [R0 + b0000] (Optional offset)
            //LDR R1, [R0 + h0]    (Optional offset)
            //LDR R1, [R0]         (No offset)

            unsigned short machineCodeInstruction = 0;

            generateDataTypeMachineCodeInstruction(token, &machineCodeInstruction);
            
            writeMachineCodeInstructionToFile(fptrWrite, machineCodeInstruction);
        }

        cmpResult = strcmp(token, STR);

        if (cmpResult == 0)
        {
            //STR R3, [R0 + b0000] (Optional offset in binary)
            //STR R3, [R0 + h0]    (Optional offset in hex)
            //STR R3, [R0]         (No offset)

            unsigned short machineCodeInstruction = 0;

            generateDataTypeMachineCodeInstruction(token, &machineCodeInstruction);

            writeMachineCodeInstructionToFile(fptrWrite, machineCodeInstruction);
        }

        cmpResult = strcmp(token, B);

        if (cmpResult == 0)
        {
            //B b000000000010
            //B h002

            unsigned short machineCodeInstruction = 0;

            generateUnconditionalBranchTypeMachineCodeInstruction(token, &machineCodeInstruction);
            
            writeMachineCodeInstructionToFile(fptrWrite, machineCodeInstruction);
        }

        cmpResult = strcmp(token, BE);

        if (cmpResult == 0)
        {
            //BE R3, b00000010
            //BE R3, h02

            unsigned short machineCodeInstruction = 0;

            generateConditionalBranchTypeMachineCodeInstruction(token, &machineCodeInstruction);

            writeMachineCodeInstructionToFile(fptrWrite, machineCodeInstruction);
        }

        cmpResult = strcmp(token, BNE);

        if (cmpResult == 0)
        {
           //BNE R3, b00000010
           //BNE R3, h02

            unsigned short machineCodeInstruction = 0;

            generateConditionalBranchTypeMachineCodeInstruction(token, &machineCodeInstruction);

            writeMachineCodeInstructionToFile(fptrWrite, machineCodeInstruction);
        }

        cmpResult = strcmp(token, BL);

        if (cmpResult == 0)
        {
            //BL R3, b00000010
            //BL R3, h02

            unsigned short machineCodeInstruction = 0;

            generateConditionalBranchTypeMachineCodeInstruction(token, &machineCodeInstruction);

            writeMachineCodeInstructionToFile(fptrWrite, machineCodeInstruction);
        }

        cmpResult = strcmp(token, BG);

        if (cmpResult == 0)
        {
            //BG R3, b00000010
            //BG R3, h02

            unsigned short machineCodeInstruction = 0;

            generateConditionalBranchTypeMachineCodeInstruction(token, &machineCodeInstruction);

            writeMachineCodeInstructionToFile(fptrWrite, machineCodeInstruction);
        }

        cmpResult = strcmp(token, BLE);

        if (cmpResult == 0)
        {
            //BLE R3, b00000010
            //BLE R3, h02

            unsigned short machineCodeInstruction = 0;

            generateConditionalBranchTypeMachineCodeInstruction(token, &machineCodeInstruction);

            writeMachineCodeInstructionToFile(fptrWrite, machineCodeInstruction);
        }

        cmpResult = strcmp(token, BGE);

        if (cmpResult == 0)
        {
            //BGE R3, b00000010
            //BGE R3, h02

            unsigned short machineCodeInstruction = 0;

            generateConditionalBranchTypeMachineCodeInstruction(token, &machineCodeInstruction);

            writeMachineCodeInstructionToFile(fptrWrite, machineCodeInstruction);
        }

        cmpResult = strcmp(token, HALT);

        if (cmpResult == 0)
        {
            appendOpcode(token, &machineCodeInstruction, OPCODE_SHIFT);
            
            writeMachineCodeInstructionToFile(fptrWrite, machineCodeInstruction);
        }

        result = fgets(fileline, ASSEMBLY_STATEMENT_BUFFER_SIZE, fptrRead);
    }

    fclose(fptrRead);
    fclose(fptrWrite);
}

void writeMachineCodeInstructionToFile(FILE* fptr, unsigned short machineInstruction)
{
    size_t result = fwrite(&machineInstruction, sizeof(unsigned short), 1, fptr);
}

void generateImmediateTypeMachineCodeInstruction(char* token, unsigned short* machineCodeInstruction)
{
    char* destReg = strtok(NULL, ",");
    char* immediate = strtok(NULL, " \0");

    appendOpcode(token, machineCodeInstruction, OPCODE_SHIFT);
    appendRegisterNumber(destReg, machineCodeInstruction, RD_REG_SHIFT);
    appendImmediateValue(immediate, machineCodeInstruction, IMM_8_BIT_SHIFT);
}

void generateRegisterTypeMachineCodeInstruction(char* token, unsigned short* machineCodeInstruction)
{
    char* destReg = strtok(NULL, ", ");
    char* src1Reg = strtok(NULL, ", ");
    char* src2Reg = strtok(NULL, " \0");

    appendOpcode(token, machineCodeInstruction, OPCODE_SHIFT);
    appendRegisterNumber(destReg, machineCodeInstruction, RD_REG_SHIFT);
    appendRegisterNumber(src1Reg, machineCodeInstruction, RM_REG_SHIFT);
    appendRegisterNumber(src2Reg, machineCodeInstruction, RN_REG_SHIFT);
}

void generateDataTypeMachineCodeInstruction(char* token, unsigned short* machineCodeInstruction)
{
    char* destReg = strtok(NULL, ",");
    char* bracketRemoval = strtok(NULL, "[");
    char* memoryAddressBaseReg = strtok(NULL, " ");
    char* operatorRemoval = strtok(NULL, "+ ");
    char* memoryAddressOffset = NULL;

    if (operatorRemoval == NULL)
        memoryAddressOffset = "h0";
    else
        memoryAddressOffset = strtok(operatorRemoval, "]");

    appendOpcode(token, machineCodeInstruction, OPCODE_SHIFT);
    appendRegisterNumber(destReg, machineCodeInstruction, RT_REG_SHIFT);
    appendRegisterNumber(memoryAddressBaseReg, machineCodeInstruction, RM_REG_SHIFT);
    appendImmediateValue(memoryAddressOffset, machineCodeInstruction, DATA_MEMORY_OFFSET_BIT_SHIFT);

}

void generateUnconditionalBranchTypeMachineCodeInstruction(char* token, unsigned short* machineCodeInstruction)
{
    char* pcOffset = strtok(NULL, "\0");

    appendOpcode(token, machineCodeInstruction, OPCODE_SHIFT);
    appendImmediateValue(pcOffset, machineCodeInstruction, UCOND_BR_PC_OFFSET_BIT_SHIFT);
}

void generateConditionalBranchTypeMachineCodeInstruction(char* token, unsigned short* machineCodeInstruction)
{
    char* cmpReg = strtok(NULL, ",");
    char* memoryAddressOffset = strtok(NULL, " \0");

    appendOpcode(token, machineCodeInstruction, OPCODE_SHIFT);
    appendRegisterNumber(cmpReg, machineCodeInstruction, RT_REG_SHIFT);
    appendImmediateValue(memoryAddressOffset, machineCodeInstruction, COND_BR_PC_OFFSET_BIT_SHIFT);
}

void writeDssSize(FILE* fptr, char* dssSize)
{
    unsigned short machineCode = 0;
    char* value = dssSize + 1;

    if (dssSize[0] == 'b')
    {
        machineCode = (unsigned short)strtol(value, NULL, 2);
    }

    if (dssSize[0] == 'h')
    {
        machineCode = (unsigned short)strtol(value, NULL, 16);
    }

    size_t result = fwrite(&machineCode, sizeof(unsigned short), 1, fptr);
}

void writeDssWord(FILE* fptr, char* dssWord)
{
    unsigned short machineCode = 0;
    char* value = dssWord + 1;

    if (dssWord[0] == 'b')
    {
        machineCode = (unsigned short)strtol(value, NULL, 2);
    }

    if (dssWord[0] == 'h')
    {
        machineCode = (unsigned short)strtol(value, NULL, 16);
    }

    size_t result = fwrite(&machineCode, sizeof(unsigned short), 1, fptr);
}

void appendOpcode(char* mnemonic, unsigned short* machineInstruction, unsigned short leftShift)
{
    unsigned short machineOpcode = 0;
    int cmpResult = 0;

    cmpResult = strcmp(mnemonic, MOVI);
    if (cmpResult == 0) machineOpcode = OPCODE_MOVI;

    cmpResult = strcmp(mnemonic, ADD);
    if (cmpResult == 0) machineOpcode = OPCODE_ADD;

    cmpResult = strcmp(mnemonic, SUB);
    if (cmpResult == 0) machineOpcode = OPCODE_SUB;

    cmpResult = strcmp(mnemonic, CMP);
    if (cmpResult == 0) machineOpcode = OPCODE_CMP;

    cmpResult = strcmp(mnemonic, MUL);
    if (cmpResult == 0) machineOpcode = OPCODE_MUL;

    cmpResult = strcmp(mnemonic, DIV);
    if (cmpResult == 0) machineOpcode = OPCODE_DIV;

    cmpResult = strcmp(mnemonic, LDR);
    if (cmpResult == 0) machineOpcode = OPCODE_LDR;

    cmpResult = strcmp(mnemonic, STR);
    if (cmpResult == 0) machineOpcode = OPCODE_STR;

    cmpResult = strcmp(mnemonic, BE);
    if (cmpResult == 0) machineOpcode = OPCODE_BE;

    cmpResult = strcmp(mnemonic, BNE);
    if (cmpResult == 0) machineOpcode = OPCODE_BNE;

    cmpResult = strcmp(mnemonic, BL);
    if (cmpResult == 0) machineOpcode = OPCODE_BL;

    cmpResult = strcmp(mnemonic, BG);
    if (cmpResult == 0) machineOpcode = OPCODE_BG;

    cmpResult = strcmp(mnemonic, BLE);
    if (cmpResult == 0) machineOpcode = OPCODE_BLE;

    cmpResult = strcmp(mnemonic, BGE);
    if (cmpResult == 0) machineOpcode = OPCODE_BGE;

    cmpResult = strcmp(mnemonic, B);
    if (cmpResult == 0) machineOpcode = OPCODE_B;

    cmpResult = strcmp(mnemonic, HALT);
    if (cmpResult == 0) machineOpcode = OPCODE_HALT;
       
    machineOpcode = machineOpcode << leftShift;
    *machineInstruction = *machineInstruction | machineOpcode;
}

void appendRegisterNumber(char* registerNumber, unsigned short* machineInstruction, unsigned short leftShift)
{
    unsigned short machineRegister = 0;
    int cmpResult = 0;

    cmpResult = strcmp(registerNumber, ACRO_R0);
    if (cmpResult == 0) machineRegister = MCRO_R0;
  
    cmpResult = strcmp(registerNumber, ACRO_R1);
    if (cmpResult == 0) machineRegister = MCRO_R1;

    cmpResult = strcmp(registerNumber, ACRO_R2);
    if (cmpResult == 0) machineRegister = MCRO_R2;

    cmpResult = strcmp(registerNumber, ACRO_R3);
    if (cmpResult == 0) machineRegister = MCRO_R3;

    cmpResult = strcmp(registerNumber, ACRO_R4);
    if (cmpResult == 0) machineRegister = MCRO_R4;

    cmpResult = strcmp(registerNumber, ACRO_R5);
    if (cmpResult == 0) machineRegister = MCRO_R5;

    cmpResult = strcmp(registerNumber, ACRO_R6);
    if (cmpResult == 0) machineRegister = MCRO_R6;

    cmpResult = strcmp(registerNumber, ACRO_R7);
    if (cmpResult == 0) machineRegister = MCRO_R7;

    cmpResult = strcmp(registerNumber, ACRO_R8);
    if (cmpResult == 0) machineRegister = MCRO_R8;

    cmpResult = strcmp(registerNumber, ACRO_R9);
    if (cmpResult == 0) machineRegister = MCRO_R9;

    cmpResult = strcmp(registerNumber, ACRO_R10);
    if (cmpResult == 0) machineRegister = MCRO_R10;

    cmpResult = strcmp(registerNumber, ACRO_R11);
    if (cmpResult == 0) machineRegister = MCRO_R11;

    cmpResult = strcmp(registerNumber, ACRO_R12);
    if (cmpResult == 0) machineRegister = MCRO_R12;

    cmpResult = strcmp(registerNumber, ACRO_R13);
    if (cmpResult == 0) machineRegister = MCRO_R13;

    cmpResult = strcmp(registerNumber, ACRO_R14);
    if (cmpResult == 0) machineRegister = MCRO_R14;

    cmpResult = strcmp(registerNumber, ACRO_R15);
    if (cmpResult == 0) machineRegister = MCRO_R15;

    machineRegister = machineRegister << leftShift;
    *machineInstruction = *machineInstruction | machineRegister;
}

void appendImmediateValue(char* immediateValue, unsigned short* machineInstruction, unsigned short leftShift)
{
   unsigned short machineImmediate = 0;
   char* value = immediateValue + 1;

   if (immediateValue[0] == 'b')
   {
       machineImmediate = (unsigned short)strtol(value, NULL, 2);
   }

   if (immediateValue[0] == 'h')
   {
       machineImmediate = (unsigned short)strtol(value, NULL, 16);
   }
   
   machineImmediate = machineImmediate << leftShift;
   *machineInstruction = *machineInstruction | machineImmediate;
}
