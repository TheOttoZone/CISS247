int array[] = { 4,5,6 };
int search_value = 5;

int main()
{   
    int size = 3;
    int index = 0;
    int search_value_array_index = -1;

    while (index < size)
    {
        if (search_value == array[index])
        {
            search_value_array_index = index;

            break;
        }

        index++;
    }
    
    return search_value_array_index;
}

/*
Registers:
R0: array base memory address
R1: 1 (used for incrementing)
R2: 0 (zero)
R3: index value
R4: array size
R5: search value 5
R6: value to be returned
*/