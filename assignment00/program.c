/*
#  Name: Aiden Otto
#  Date: 10/3/25
#  Description: Assignment 00
*/

#include <stdio.h>

//Function Prototypes
double ComputeTulipEarnings(int tulips_picked);

int main()
{
    //Constant Declarations
    const int SENTINEL_VALUE = -1;

    //Variable Declarations
    int tulipsPicked = 0;
    double totalEarnings = 0;
    int transactionCount = 0;
   
    //Input and Process
    printf("Enter quantity of tulips picked or %d to quit: ", SENTINEL_VALUE);
    scanf("%d", &tulipsPicked);

    while (tulipsPicked != SENTINEL_VALUE)
    {
        double earnings = ComputeTulipEarnings(tulipsPicked);

        printf("Earnings: %lf \n\n", earnings);

        totalEarnings += earnings;

        transactionCount++;

        printf("Enter quantity of tulips picked or %d to quit: ", SENTINEL_VALUE);
        scanf("%d", &tulipsPicked);
    }

    double averageEarnings = totalEarnings / transactionCount;

    //Output
    printf("\n");
    printf("Total earnings: %lf \n", totalEarnings);
    printf("Average earnings: %lf \n", averageEarnings);
}

//Function Definitions
double ComputeTulipEarnings(int tulipsPicked)
{
    double earnings = 0;
    
    // If the user picked less than 100 tulips, give them no money
    if(tulipsPicked < 100){
        earnings = 0;
    }
    // If the user picked less than 200 tulips, give them only 5 dollar
    else if(tulipsPicked < 200){
        earnings = 5.00;
    }
    // If the user picked less than 400 tulips, give them 5.25 per 100
    else if(tulipsPicked < 400){
        earnings = ((tulipsPicked/100) * 5.00) + ((tulipsPicked/100) * 0.25);
    }
    // If the user picked more than 400 tulips, give them 5.25 per 100 for the first 300 and then 6 per 100 after that
    else if(tulipsPicked >= 400){
        earnings = 16.25 + (((tulipsPicked-300)/100) * 6.00);
    }
   
    return earnings;
}
