/***************************************
Name: Melody Flavel
Student ID: 33878648
Date: 05/05/2021
File: Assignment2.c
Purpose: File I/O and data structures
****************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

const int NAMESIZE = 15;
const int ARRAYSIZE = 15;
const int COINS = 4;

/* Define customer data stucture */
typedef struct customer
{
    char name[NAMESIZE];
    int change;
    int coins[COINS];
} customerType;


void Store_data(customerType customers[], FILE *srcFile)
{
        int i = 0;

        /* Array loop */
        for(i = 0; i < ARRAYSIZE; i++)
        {
            /* Write data from file to to array */
            fscanf(srcFile, "%s %d", customers[i].name, &customers[i].change);
        }
        printf("\n");
        fclose(srcFile);

    return;
}


void Determine_coins(customerType customers[])
{
    int i;
    int change;

    /* Array loop */
    for(i = 0; i < ARRAYSIZE; i++)
    {
        /* Initialise customer coin elements to 0 */
        customers[i].coins[0] = 0;
        customers[i].coins[1] = 0;
        customers[i].coins[2] = 0;
        customers[i].coins[3] = 0;
        
        /* Assign customers' change values to variable for easier looping */
        change = customers[i].change;

        /* Determine coins */
        while(change >= 50)
        {
            customers[i].coins[0] = customers[i].coins[0] + 1;
            change = change - 50;
        }
        while(change >= 20)
        {
            customers[i].coins[1] = customers[i].coins[1] + 1;
            change = change - 20;
        }
        while(change >= 10)
        {
            customers[i].coins[2] = customers[i].coins[2] + 1;
            change = change - 10;
        }
        while(change >= 5)
        {
            customers[i].coins[3] = customers[i].coins[3] + 1;
            change = change - 5;
        }
    }
    return;
}

int Show_menu()
{
    int option;

    /* Display menu */
    menu:
    printf("Please select an option:\n");
    printf("1. Enter name\n");
    printf("2. Exit\n");
    scanf("%d%*c", &option);

    /* Check user input is valid */
    if((option != 1 && option != 2))
    {
        printf("Invalid option.\n");
        goto menu;
    }
        return(option);
}


void Get_name(char name[])
{
    
    printf("Please enter a name: ");
    /* Check if input is valid and assign to name variable */
    if(fgets(name, NAMESIZE, stdin) != NULL)
    {
        name[strlen(name) - 1] = '\0';
        name[0] = toupper(name[0]);
    }

    return;
}


void Search_array(char name[], customerType customers[], int *indexAt)
{
    int i;
    int found = 0;
    *indexAt = -1;

    for(i = 0; ((i < ARRAYSIZE) && (found == 0)); i++)
    {
        /* Find name in customer array */
        if(strcmp(customers[i].name, name) == 0)
        {
            *indexAt = i;
            found = 1;
        }
    }

    return;
}


void Display_change(customerType customers[], int *x) // Changed name
{
    printf("Customer: %s\n", customers[*x].name);
    printf("%d cents\n", customers[*x].change);
    printf("\n");
    printf("Change:\n");
    
    /* Only print value if it does not equal 0 */
    if(customers[*x].coins[0] != 0)
    {
        printf("50 cents: %d\n", customers[*x].coins[0]);
    }
    if(customers[*x].coins[1] != 0)
    {
        printf("20 cents: %d\n", customers[*x].coins[1]);
    }
    if(customers[*x].coins[2] != 0)
    {
        printf("10 cents: %d\n", customers[*x].coins[2]);
    }
    if(customers[*x].coins[3] != 0)
    {
        printf("5 cents: %d\n", customers[*x].coins[3]);
    }

    printf("\n");

    return;
}

void Write_data(customerType customers[], FILE *dstFile)
{
    int i;

    dstFile = fopen("change.csv", "w");
    for(i = 0; i < ARRAYSIZE; i++)
    {
        fprintf(dstFile, "%s,%d,%d,%d,%d,%d\n", customers[i].name, customers[i].change, customers[i].coins[0], customers[i].coins[1], customers[i].coins[2], customers[i].coins[3]);
    }
    fclose(dstFile);

    return;
}


int main(int argc, char *argv[])
{
    FILE *srcFile; 
    FILE *dstFile;
    customerType customers[ARRAYSIZE];
    char name[NAMESIZE];
    int option; 
    int indexAt;

    /* Check for argument */
    if(argc != 2)
    {
        printf("Invalid command line argument.\n");
    }
    else
    {
        srcFile = fopen (argv[1], "r");
        /* Check file opening */
        if(srcFile == NULL)
        {
            perror("File opening failed.");
            return -1;
        }
        else
        {
            Store_data(customers, srcFile);
            Determine_coins(customers);
            do
            {
                option = Show_menu();
                switch(option)
                {
                    case 1:
                        getname:
                        Get_name(name);
                        Search_array(name, customers, &indexAt);
                        /* Check if customer exists */
                        if(indexAt == -1)
                        {
                            printf("Could not find customer in database. Please try again.\n");
                            goto getname;
                        }
                        Display_change(customers, &indexAt);
                        break;
                    case 2:
                        Write_data(customers, dstFile);
                        printf("Quitting program...\n");
                        break;
                    default:
                        printf("Invalid option.\n");
                        break;
                }
            }
            while(option != 2);
        }
    }
    return(0);
}