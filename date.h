#ifndef DATE_H_
#define DATE_H_
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

/** this data type is for the 'tin' field of Port */

typedef struct date_t {
    int Year;
    int Month;
    int Day;
    int Hour;
    int Min;
} *Date;
 

Date createDate(int year, int month, int day, int hour, int minute);// Creates a new Date object with the specified year, month, day, hour, and minute

Date getCurrentDate(); // Returns the current date and time

void destroyDate(Date date);// Destroys the Date object and frees memory

int compareDates(Date date1, Date date2); // Returns <0 if date1 < date2, 0 if equal, >0 if date1 > date2

bool isValidDate(Date date); // Checks if the date is valid (e.g., month in range, day in range for the month)

void printDate(Date date); // Prints the date in a readable format

int diffMinutes(Date d1, Date d2); // Calcs the difference in minutes between two dates

#endif // DATE_H_
