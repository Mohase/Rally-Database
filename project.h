#ifndef _PROJECT__H_
#define _PROJECT__H_


typedef struct 
{
    char *surname;
    char *team;
    unsigned int total_hours;
    unsigned int minutes;
    unsigned int seconds;   
} Driver;

// Function prototypes
void addDriver(Driver **drivers, int *numDrivers, char *surname, char *team);
void updateTime(Driver *drivers, int numDrivers, char *surname, unsigned int hours, unsigned int minutes, unsigned int seconds);
void printRace(Driver *drivers, int numDrivers);
void saveToFile(Driver *drivers, int numDrivers, char *filename);
void loadFromFile(Driver **drivers, int *numDrivers, char *filename);
void quitProgram(Driver *drivers, int numDrivers);

#endif //! _PROJECT__H_
