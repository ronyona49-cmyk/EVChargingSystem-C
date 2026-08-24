#include "date.h"
#include <time.h>
#include "Types.h"




Date createDate(int year, int month, int day, int hour, int minute)// Creates a new Date object by choose
{
    Date newDate = (Date)malloc(sizeof(struct date_t));
    if (newDate == NULL)// hadle memory allocation failure
    {
		printf("Memory allocation failed.\n");
		return NULL; 
    }
    newDate->Year = year;
    newDate->Month = month;
    newDate->Day = day;
    newDate->Hour = hour;
    newDate->Min = minute;
    return newDate;
}

Date getCurrentDate()// Returns the current date and time 
{
    time_t now = time(NULL);
    struct tm* tm_now = localtime(&now);

    return createDate(tm_now->tm_year + 1900, tm_now->tm_mon + 1, tm_now->tm_mday,
        tm_now->tm_hour, tm_now->tm_min);
}

void destroyDate(Date date)// Destroys the Date object and frees memory
{
    if (date != NULL) {
        free(date);
    }
}

int compareDates(Date date1, Date date2)// Compares two dates 
{
    // Compares two Date objects.
// Returns positive if date1 > date2, negative if date1 < date2, 0 if equal.
// If either date is NULL, displays error and returns 0 

    if (date1 == NULL || date2 == NULL) {
        return 0; // Handle null pointers
	}
    if (date1->Year != date2->Year) {
        return date1->Year - date2->Year;
    }
    if (date1->Month != date2->Month) {
        return date1->Month - date2->Month;
    }
    if (date1->Day != date2->Day) {
        return date1->Day - date2->Day;
    }
    if (date1->Hour != date2->Hour) {
        return date1->Hour - date2->Hour;
    }
    return date1->Min - date2->Min;
}

bool isValidDate(Date date)// Checks if a Date is valid (day fits month/year, hour/minute in range),Returns true if valid, false otherwise.
{
    if (date == NULL) return false;
    if (date->Month < 1 || date->Month > 12) return false;

    int daysInMonth[] = { 0, 31, 28 + (date->Year % 4 == 0 && (date->Year % 100 != 0 || date->Year % 400 == 0)),
                         31, 30, 31, 30, 31, 31, 30, 31, 30 };

    if (date->Day < 1 || date->Day > daysInMonth[date->Month]) return false;
    if (date->Hour < 0 || date->Hour > 23) return false;
    if (date->Min < 0 || date->Min > 59) return false;

    return true;
}

void printDate(Date date) // Prints the date in a readable format
{
    if (date != NULL) {
        printf("%04d-%02d-%02d %02d:%02d\n", date->Year, date->Month, date->Day, date->Hour, date->Min);
    }
    else {
    }
}

int diffMinutes(Date d1, Date d2) //calcs the difference in minutes between two dates
{
    // Calculates the difference in minutes between two Date objects (d1 - d2).
// Returns 0 if either date is NULL (with error message).

    if (d1 == NULL || d2 == NULL) {
        return 0; // Handle null pointers
	}
    struct tm t1 = { .tm_year = d1->Year - 1900, .tm_mon = d1->Month - 1, .tm_mday = d1->Day,
                     .tm_hour = d1->Hour, .tm_min = d1->Min, .tm_sec = 0 };
    struct tm t2 = { .tm_year = d2->Year - 1900, .tm_mon = d2->Month - 1, .tm_mday = d2->Day,
                     .tm_hour = d2->Hour, .tm_min = d2->Min, .tm_sec = 0 };

    time_t time1 = mktime(&t1);
    time_t time2 = mktime(&t2);

    return (int)difftime(time1, time2) / 60;
}

