#include "OperatingSystem.h"
#include "MainMemory.h"
#include <stdio.h>
#include <stdlib.h>

//PRIVATE FUNCTION PROTOTYPES
int OperatingSystemLoadExecutableFile(OperatingSystem* operatingSystemPtr, char* executableFilename, int* programSize);

#define DATA_SEGMENT_START_ADDRESS 0x0000
#define DATA_WORD_SIZE 16
#define DATA_WORD_BUFFER_SIZE (DATA_WORD_SIZE + 4)

#define INSTRUCTION_SIZE 16
#define INSTRUCTION_BUFFER_SIZE (INSTRUCTION_SIZE + 4)

void OperatingSystemInitialize(OperatingSystem* operatingSystemPtr, CPU* cpuPtr, MainMemory*  mainMemoryPtr)
{
    operatingSystemPtr->cpuPtr = cpuPtr;
    operatingSystemPtr->mainMemoryPtr = mainMemoryPtr;
}

int16_t OperatingSystemExecuteProgram(OperatingSystem* operatingSystemPtr, char* executableFilename)
{
    int16_t returnValue = -1;

    int programSize = 0;

    int startingMainMemoryAddress = OperatingSystemLoadExecutableFile(operatingSystemPtr, executableFilename, &programSize);
    
    if (startingMainMemoryAddress != -1)
    {
        returnValue = CPUExecuteProgram(operatingSystemPtr->cpuPtr, startingMainMemoryAddress, programSize);
    }

    return returnValue;
}

int OperatingSystemLoadExecutableFile(OperatingSystem* operatingSystemPtr, char* executableFilename, int* programSize)
{
    int codeSegmentStartAddress = 0;
    
    //Open executable file
    FILE* executableFilenameFilePtr = fopen(executableFilename, "rb");

    if (executableFilenameFilePtr == NULL)
    {
        printf("Executable file does not exist: %s\n\n", executableFilename);

        return -1;
    }

    //LOAD DATA SEMENT

    //Read size of data segment
    uint16_t dataSegmentSize = 0;
    size_t result = fread(&dataSegmentSize, sizeof(uint16_t), 1, executableFilenameFilePtr);

    //Read data segment values
    int mainMemoryAddress = DATA_SEGMENT_START_ADDRESS;

    for (int index = 0; index < dataSegmentSize; index++)
    {
        uint16_t dataSegmentWord = 0;
        size_t result = fread(&dataSegmentWord, sizeof(uint16_t), 1, executableFilenameFilePtr);

        operatingSystemPtr->mainMemoryPtr->values[mainMemoryAddress] = dataSegmentWord;

        mainMemoryAddress++;
    }

    //LOAD CODE SEGMENT

    codeSegmentStartAddress = mainMemoryAddress;

    uint16_t instruction = 0;

    result = fread(&instruction, sizeof(uint16_t), 1, executableFilenameFilePtr);

    while (result != 0)
    {
        operatingSystemPtr->mainMemoryPtr->values[mainMemoryAddress] = instruction;
        mainMemoryAddress++;

        result = fread(&instruction, sizeof(uint16_t), 1, executableFilenameFilePtr);
    }

    fclose(executableFilenameFilePtr);

    *programSize = mainMemoryAddress;

    return codeSegmentStartAddress;
}