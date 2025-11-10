#include "Computer.h"
#include <stdio.h>

void ComputerInitialize(Computer* computerPtr)
{
    MainMemoryInitialize(&computerPtr->mainMemory);
    CPUInitialize(&computerPtr->cpu, &computerPtr->mainMemory);
    OperatingSystemInitialize(&computerPtr->operatingSystem, &computerPtr->cpu, &computerPtr->mainMemory);
}

int16_t ComputerExecuteProgram(Computer* computerPtr, char* executableFilename)
{
    printf("Executing %s...\n", executableFilename);
    printf("\n");

    int16_t returnValue = OperatingSystemExecuteProgram(&computerPtr->operatingSystem, executableFilename);

    return returnValue;
}
