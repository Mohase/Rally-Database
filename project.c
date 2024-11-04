#include <project.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int compareDrivers(const void *a, const void *b) 
{
    const Driver *driverA = (const Driver *)a;
    const Driver *driverB = (const Driver *)b;

    // Convert total time to seconds for comparison
    int totalTimeA = driverA->total_hours * 3600 + driverA->minutes * 60 + driverA->seconds;
    int totalTimeB = driverB->total_hours * 3600 + driverB->minutes * 60 + driverB->seconds;

    return totalTimeA - totalTimeB;
}

int main()
{
    Driver *drivers = NULL;
    int numDrivers = 0;
    char command[1000], surname[1000], team[1000], filename[1000];
    int hours, minutes, seconds;
    char letter;

    while (1) 
    {
        fgets(command, sizeof(command), stdin);

        if (command[0] == 'A') 
        {
            if (sscanf(command, "%c %s %s", &letter, surname, team) == 3) 
            {
                addDriver(&drivers, &numDrivers, surname, team);
            } 
            else 
            {
                printf("Invalid command\n");
            }
        } 
        else if (command[0] == 'U') 
        {
            if (sscanf(command, "%c %s %d %d %d", &letter, surname, &hours, &minutes, &seconds) == 5) 
            {
                updateTime(drivers, numDrivers, surname, hours, minutes, seconds);
            } 
            else 
            {
                printf("Invalid command\n");
            }
        } 
        else if (command[0] == 'L') 
        {
            if (sscanf(command, "%c", &letter) == 1) 
            {
                qsort(drivers, numDrivers, sizeof(Driver), compareDrivers);
                printRace(drivers, numDrivers);
            } 
            else 
            {
                printf("Invalid command\n");
            }
        } 
        else if (command[0] == 'W') 
        {
            if (sscanf(command, "%c %s", &letter, filename) == 2) 
            {
                saveToFile(drivers, numDrivers, filename);
            } 
            else 
            {
                printf("Invalid command\n");
            }
        } 
        else if (command[0] == 'O') 
        {
            if (sscanf(command, "%c %s", &letter, filename) == 2) 
            {
                loadFromFile(&drivers, &numDrivers, filename);
            } 
            else 
            {
                printf("Invalid command\n");
            }
        } 
        else if (command[0] == 'Q') 
        {
            quitProgram(drivers, numDrivers);
            return 0;
        } 
        else 
        {
            printf("Invalid command\n");
        }
    }
    return 0;
}

void addDriver(Driver **drivers, int *numDrivers, char *surname, char *team) 
{
    // Check if the driver already exists
    for (int i = 0; i < *numDrivers; i++) 
    {
        if (strcmp((*drivers)[i].surname, surname) == 0) 
        {
            printf("Error: Driver %s already exists\n", surname);
            return;
        }
    }

    // Allocate memory for new driver
    *drivers = (Driver *)realloc(*drivers, (*numDrivers + 1) * sizeof(Driver));
    if (*drivers == NULL) 
    {
        printf("Error: Memory allocation failed\n");
        exit(1);
    }

    // Allocate memory for the surname string
    (*drivers)[*numDrivers].surname = (char *)malloc(strlen(surname) + 1);
    if ((*drivers)[*numDrivers].surname == NULL) 
    {
        printf("Error: Memory allocation failed\n");
        exit(1);
    }
    // Copy the surname string
    strcpy((*drivers)[*numDrivers].surname, surname);

    // Allocate memory for the team string
    (*drivers)[*numDrivers].team = (char *)malloc(strlen(team) + 1);
    if ((*drivers)[*numDrivers].team == NULL) 
    {
        printf("Error: Memory allocation failed\n");
        exit(1);
    }
    // Copy the team string
    strcpy((*drivers)[*numDrivers].team, team);

    // Initialize other fields
    (*drivers)[*numDrivers].total_hours = 0;
    (*drivers)[*numDrivers].minutes = 0;
    (*drivers)[*numDrivers].seconds = 0;

    (*numDrivers)++;
    printf("SUCCESS\n");
}

void updateTime(Driver *drivers, int numDrivers, char *surname, unsigned int hours, unsigned int minutes, unsigned int seconds) 
{
    if (minutes >= 60 || seconds >= 60) 
    {
        printf("Error: Hours, minutes, and seconds should be in the appropriate ranges [0,59].\n");
        return;
    }

    // Search for the driver
    for (int i = 0; i < numDrivers; i++) 
    {
        if (strcmp(drivers[i].surname, surname) == 0) 
        {
            // Update time
            drivers[i].total_hours += hours;
            drivers[i].minutes += minutes;
            drivers[i].seconds += seconds;

            // Adjust minutes and seconds
            drivers[i].minutes += drivers[i].seconds / 60;
            drivers[i].seconds %= 60;
            drivers[i].total_hours += drivers[i].minutes / 60;
            drivers[i].minutes %= 60;

            printf("SUCCESS\n");
            return;
        }
    }

    printf("Error: Driver %s not found\n", surname);
}

void printRace(Driver *drivers, int numDrivers) 
{
        for (int i = 0; i < numDrivers - 1; i++) 
        {
            for (int j = 0; j < numDrivers - i - 1; j++) 
            {
                int totalSeconds1 = drivers[j].total_hours * 3600 + drivers[j].minutes * 60 + drivers[j].seconds;
                int totalSeconds2 = drivers[j + 1].total_hours * 3600 + drivers[j + 1].minutes * 60 + drivers[j + 1].seconds;

                if (totalSeconds1 > totalSeconds2) 
                {
                    Driver temp = drivers[j];
                    drivers[j] = drivers[j + 1];
                    drivers[j + 1] = temp;
                }
            }
        }

    int zeroIndex = -1; // Initialize index to track the first occurrence of zero time
    for (int i = 0; i < numDrivers; i++) 
    {
        int totalSeconds = drivers[i].total_hours * 3600 + drivers[i].minutes * 60 + drivers[i].seconds;
        if (totalSeconds == 0 && zeroIndex == -1) 
        {
            zeroIndex = i; // Record the index of the first driver with zero time
        } 
        else if (totalSeconds != 0 && zeroIndex != -1) 
        {
            // Swap the driver with zero time to the end
            Driver temp = drivers[i];
            drivers[i] = drivers[zeroIndex];
            drivers[zeroIndex] = temp;
            zeroIndex++; // Move the zero index forward
        }
    }

    for (int i = 0; i < numDrivers; i++)
    {
        printf("%s %s %d %d %d\n", drivers[i].surname, drivers[i].team, drivers[i].total_hours, drivers[i].minutes, drivers[i].seconds);
    }

    printf("SUCCESS\n");

}

void saveToFile(Driver *drivers, int numDrivers, char *filename) 
{
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) 
    {
        printf("Error: Unable to open file %s\n", filename);
        return;
    }

    // Write to file
    for (int i = 0; i < numDrivers; i++) 
    {
        fprintf(fp, "%s %s %d %d %d\n", drivers[i].surname, drivers[i].team, drivers[i].total_hours, drivers[i].minutes, drivers[i].seconds);
    }

    fclose(fp);
    printf("SUCCESS\n");
}

void loadFromFile(Driver **drivers, int *numDrivers, char *filename) 
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) 
    {
        printf("Error: Unable to open file %s\n", filename);
        return;
    }

    // Clear existing data
    for (int i = 0; i < *numDrivers; i++) 
    {
        free((*drivers)[i].surname);
        free((*drivers)[i].team);
    }
    free(*drivers);
    *drivers = NULL;
    *numDrivers = 0;

    // Read from file
    while (1) 
    {
        char surname[100], team[100];
        int hours, minutes, seconds;

        if (fscanf(fp, "%s %s %d %d %d", surname, team, &hours, &minutes, &seconds) != 5) 
        {
            break; // End of file or invalid format
        }

        addDriver(drivers, numDrivers, surname, team);
        updateTime(*drivers, *numDrivers, surname, hours, minutes, seconds);
    }

    fclose(fp);
    printf("SUCCESS\n");
}

void quitProgram(Driver *drivers, int numDrivers) 
{
    // Free memory for surname and team strings for each driver
    for (int i = 0; i < numDrivers; i++) 
    {
        free(drivers[i].surname);
        free(drivers[i].team);
    }

    // Free memory for the drivers array
    free(drivers);

    printf("SUCCESS\n");
    exit(EXIT_SUCCESS);
}
