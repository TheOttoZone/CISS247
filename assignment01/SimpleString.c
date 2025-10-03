/*
# Name: Aiden Otto 
# Date: 9/29/25
# Description: Assignment 01
*/

#include <stdio.h>

int StringLength(const char source[])
{
    // declare and initialize a length
    int length = 0;
    
    // iterate through each char in the array and stop once null terminating character
    while(source[length] != '\0'){
        length++;
    }
    return length;
}

int StringCopy(const char source[], int sourceSize, char destination[], int destSize)
{
    // error checking to ensure that we can actually put the source array into our destination, if we can't, return -1
    if(destSize < sourceSize){
        return -1;
    }
    // iterate through each character in the array and set the same index in the destination array equal to the current index of the source
    for (int i = 0; i < sourceSize; i++){
        destination[i] = source[i];
    }

    // set our destination size variable equal to the length of the destination array to ensure it is equal
    destSize = StringLength(destination);

    return destSize;
}

int StringSearch(const char source[], int sourceSize, const char searchString[], int searchStringSize)
{
    // the index we will return if we find the string searched, set to -1 by default in case we dont find it
    int sourceStringFoundIndex = -1;
    // variable for storing the potential index of the found word
    int tempIndex;

    for(int i = 0; i < sourceSize; i++){
        // declare another variable for use in the second for loop, will be used later
        int k;
        // if the char at the current index is the same as the first char in the searchString, continue
        if(source[i] == searchString[0]){
            // since we may have found the word, set our temporary variable equal to the current index
            tempIndex = i;
            // iterate through the source string starting at the current index,
            for(k = i; k < searchStringSize + i; k++){
                // if the char at the current index  is not equal to the equivelant index of the searchString, break
                if(source[k] != searchString[k - i]){
                    break;
                }             
            }
            // if we have gotten far enough to where the amount we have iterated through the source lines up with the searchStringSize added onto the current i iterator,
            // then we have to have found the word and we can safely set our return value to the current i value (tempIndex).
            if(k == searchStringSize + i ){
            sourceStringFoundIndex = tempIndex;
            }
        }

        
    }

    return sourceStringFoundIndex;
}

int StringReverse(const char source[], int sourceSize, char destination[], int destSize)
{
    // same error checking as before, make sure it can fit blah blah blah
    if(destSize < sourceSize){
        return -1;
    }
    // iterate through each character in the source array, copying it to the opposite end of the destination array accounting for the null termininating character
    for(int i = 0; i < sourceSize; i++){
        destination[sourceSize - 1 - i] = source[i];
    }

    return destSize;
}
