/*
# Name: Aiden Otto 
# Date: 9/29/25
# Description: Assignment 01
*/

#include <stdio.h>
#include "SimpleString.h"

//Constants
#define STRING_BUFFER_SIZE 25

int main()
{
	char sourceString[] = "The dog ate the cat.";
    char copiedString[STRING_BUFFER_SIZE] = { 0 };
    char reversedString[STRING_BUFFER_SIZE] = { 0 };
    char searchString[] = "dog";

    printf("Original string: %s\n\n", sourceString);

    int srcSize = StringLength(sourceString);
    printf("Size of string: %d\n\n", srcSize);
    
    int result = StringCopy(sourceString, srcSize, copiedString, STRING_BUFFER_SIZE);
    
    if (result != -1){
        printf("Copied string: %s\n\n", copiedString);
    }
    else{
        printf("Can't copy string. Destination string buffer too small...\n\n");
    }
    result = StringReverse(sourceString, srcSize, reversedString, STRING_BUFFER_SIZE);
    
    if (result != -1)
    {
        printf("Reversed string: %s\n\n", reversedString);
    }
    else
    {
        printf("Can't reverse string. Destination string buffer too small...\n\n");
    }

    int searchStringLength = StringLength(searchString);
    result = StringSearch(sourceString, srcSize, searchString, searchStringLength);

    if (result != -1)
    {
        printf("Search string %s found at index: %d\n", searchString, result);
    }
    else printf("Search string not found.\n");
}
