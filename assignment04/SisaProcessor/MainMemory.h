#ifndef MEMORY_H
#define MEMORY_H

/*******************************************************************************
* Module:       MainMemory
* Developer:    Michael Koepp
* Company:      WWU Computer Science Department
* Create Date:
* Description:  
*******************************************************************************/

#include <stdint.h>

#define MAIN_MEMORY_ADDRESS_BITS  7
#define MAIN_MEMORY_SIZE 1 << MAIN_MEMORY_ADDRESS_BITS //2 ^ MEMORY_ADDRESS_BIT_SIZE

typedef struct
{
    uint16_t values[MAIN_MEMORY_SIZE];

}MainMemory;

void MainMemoryInitialize(MainMemory* mainMemoryPtr);
void MainMemorySetOptions(int argc, char* argv[]);
void MainMemoryPrintValues(MainMemory* mainMemoryPtr, int16_t startAddress, int16_t endAddress);
void MainMemoryPrintDataSegmentValues(MainMemory* mainMemoryPtr, int16_t startAddress, int16_t endAddress);
void MainMemoryPrintCodeSegmentValues(MainMemory* mainMemoryPtr, int16_t startAddress, int16_t endAddress);

#endif

